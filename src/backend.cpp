#include <iostream>
#include <vector>
#include <algorithm>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <config.hpp>
#include <curlwrapper.hpp>
#include <plmover.hpp>
#include <exception.hpp>
#include <backend.hpp>


namespace backend {

template <typename T> class Cache {
    public:
        bool is_valid = false;
        T content;
        void update(T new_content){
          content = new_content;
          is_valid = true;
        }
};

Cache<std::vector<std::string>> cached_gt_status_names;
Cache<std::vector<std::string>> cached_gt_names;
Cache<std::vector<std::string>> cached_pt_names;
Cache<nlohmann::json> cached_pt_dict;// {gt_0: {pl_type_0: pl_type_0_3, pl_type_1: pl_type_1_6, ...}} ONLY ONE GT!

std::vector<std::string> _getItemNames(nlohmann::json j) {
    std::vector<std::string> name_list;
    for (const auto& obj: j){
        name_list.push_back(obj["name"]);
    }
    return name_list;
}

// Reading
nlohmann::json getGlobalTags() {
    return curlwrapper::get(config::api_url + "globalTags");
}

nlohmann::json getGlobalTagStatuses() {
    return curlwrapper::get(config::api_url + "gtstatus");
}

nlohmann::json getPayloadTypes() {
    return curlwrapper::get(config::api_url + "pt");
}

nlohmann::json getPayloadLists() {
    return curlwrapper::get(config::api_url + "pl");
}

nlohmann::json getPayloadIOVs() {
    return curlwrapper::get(config::api_url + "piov");
}

nlohmann::json _getPayloadLists(std::string gtName) {
    return curlwrapper::get(config::api_url + "gtPayloadLists/" + gtName);
}

nlohmann::json getGlobalTagMap(std::string gtName){
    return curlwrapper::get(config::api_url + "globalTag/" + gtName);
}

nlohmann::json getPayloadIOVs(std::string gtName, int majorIov, int minorIov){
    return curlwrapper::get(config::api_url + "payloadiovs/?gtName=" + gtName + "&majorIOV=" +
                            std::to_string(majorIov) + "&minorIOV=" + std::to_string(minorIov));
}

std::vector<std::string> getGtStatusNames(){
    if (!cached_gt_status_names.is_valid){
        cached_gt_status_names.update(_getItemNames(getGlobalTagStatuses()));
    }
    return cached_gt_status_names.content;
}

std::vector<std::string> getGtNames(){
    if (!cached_gt_names.is_valid){
        cached_gt_names.update(_getItemNames(getGlobalTags()));
    }
    return cached_gt_names.content;
}

std::vector<std::string> getPtNames(){
    if (!cached_pt_names.is_valid){
        cached_pt_names.update(_getItemNames(getPayloadTypes()));
    }
    return cached_pt_names.content;
}

nlohmann::json getPayloadLists(std::string gtName){
    if (!cached_pt_dict.is_valid || !cached_pt_dict.content.contains(gtName)) {
        cached_pt_dict.update(nlohmann::json::object({{gtName, _getPayloadLists(gtName)}}));
    }
    return cached_pt_dict.content[gtName];
}

bool gtStatusExists(std::string gtStatusName){
    std::vector<std::string> gtsns = getGtStatusNames();
    return std::find(gtsns.begin(), gtsns.end(), gtStatusName) != gtsns.end();
}

bool gtExists(std::string gtName){
    std::vector<std::string> gtns = getGtNames();
    return std::find(gtns.begin(), gtns.end(), gtName) != gtns.end();
}

bool plTypeExists(std::string plType){
    std::vector<std::string> ptns = getPtNames();
    return std::find(ptns.begin(), ptns.end(), plType) != ptns.end();
}

void checkGtStatusExists(std::string gtStatusName){
    if (!gtStatusExists(gtStatusName)){
        std::string msg = "no global tag status with name '"+gtStatusName+"' exists";
        throw NoPayloadException(msg);
    }
}

void checkGtExists(std::string gtName){
    if (!gtExists(gtName)){
        std::string msg = "no global tag with name '"+gtName+"' exists";
        throw NoPayloadException(msg);
    }
}

void checkPlTypeExists(std::string plType){
    if (!plTypeExists(plType)){
        std::string msg = "no payload type with name '"+plType+"' exists";
        throw NoPayloadException(msg);
    }
}

std::string getPayloadUrl(std::string gtName, std::string plType, int majorIov, int minorIov){
    nlohmann::json j = getPayloadIOVs(gtName, majorIov, minorIov);
    for (nlohmann::json piov : j){
        if (std::string(piov["payload_type"]) == plType){
            return piov["payload_iov"][0]["payload_url"];
        }
    }
    throw NoPayloadException("Could not find payload for type "+plType);
}

std::string getPayloadListName(std::string gtName, std::string plType){
    nlohmann::json j = getPayloadLists(gtName);
    if (!j.contains(plType)){
        std::string msg = "global tag '"+gtName+"' does not have payload type '"+plType;
        throw NoPayloadException(msg);
    }
    return j[plType];
}

int getPayloadListId(std::string gtName, std::string plType){
    nlohmann::json j = getPayloadLists(gtName);
    if (!j.contains(plType)){
        std::string msg = "global tag '"+gtName+"' does not have payload type '"+plType;
        throw NoPayloadException(msg);
    }
    return j[plType];
}

bool gtHasPlType(std::string gtName, std::string plType){
    nlohmann::json j = getPayloadLists(gtName);
    if (!j.contains(plType)) return false;
    return true;
}

nlohmann::json getSize(){
    int n_iov_attached = 0;
    int n_gt = 0;
    nlohmann::json global_tags = getGlobalTags();
    for (auto gt : global_tags){
        n_iov_attached += int(gt["payload_iov_count"]);
        n_gt += 1;
    }
    return nlohmann::json::object({{"n_global_tag", n_gt},
	                           {"n_iov_attached", n_iov_attached},
	          	           {"n_iov_tot", getPayloadIOVs().size()},
     			           {"n_pt", getPtNames().size()}});
}

// Writing
void createGlobalTagStatus(std::string status){
    nlohmann::json j;
    j["name"] = status;
    curlwrapper::post(config::api_url + "gtstatus", j);
    cached_gt_status_names.is_valid = false;
}

void createGlobalTagObject(std::string name, std::string status) {
    nlohmann::json j;
    j["status"] = status;
    j["name"] = name;
    curlwrapper::post(config::api_url + "gt", j);
    cached_gt_names.is_valid = false;
}

void createPayloadType(std::string type){
    nlohmann::json j;
    j["name"] = type;
    curlwrapper::post(config::api_url + "pt", j);
    cached_pt_names.is_valid = false;
}

std::string createPayloadList(std::string type){
    nlohmann::json j;
    j["payload_type"] = type;
    nlohmann::json res = curlwrapper::post(config::api_url + "pl", j);
    return res["name"];
}

void attachPayloadList(std::string gtName, std::string plName){
    nlohmann::json j;
    j["payload_list"] = plName;
    j["global_tag"] = gtName;
    curlwrapper::put(config::api_url + "pl_attach", j);
    cached_pt_dict.is_valid = false;
}

void lockGlobalTag(std::string name){
    curlwrapper::put(config::api_url + "gt_change_status/" + name + "/locked");
}

void unlockGlobalTag(std::string name){
    curlwrapper::put(config::api_url + "gt_change_status/" + name + "/unlocked");
}

int createPayloadIOV(std::string plUrl, int majorIov, int minorIov){
    nlohmann::json j;
    j["payload_url"] = plUrl;
    j["major_iov"] = majorIov;
    j["minor_iov"] = minorIov;
    j["checksum"] = "checksum";
    nlohmann::json res = curlwrapper::post(config::api_url + "piov", j);
    return res["id"];
}

int createPayloadIOV(std::string plUrl, int majorIov, int minorIov, int majorIovEnd, int minorIovEnd){
    nlohmann::json j;
    j["payload_url"] = plUrl;
    j["major_iov"] = majorIov;
    j["minor_iov"] = minorIov;
    j["major_iov_end"] = majorIovEnd;
    j["minor_iov_end"] = minorIovEnd;
    nlohmann::json res = curlwrapper::post(config::api_url + "piov", j);
    return res["id"];
}

void attachPayloadIOV(std::string plListName, int plIovId){
    nlohmann::json j;
    j["payload_list"] = plListName;
    j["piov_id"] = plIovId;
    nlohmann::json res = curlwrapper::put(config::api_url + "piov_attach", j);
}

void createGlobalTag(std::string name) {
    if (!gtStatusExists("unlocked")){
        createGlobalTagStatus("unlocked");
    }
    if (!gtStatusExists("locked")){
        createGlobalTagStatus("locked");
    }
    createGlobalTagObject(name, "unlocked");
}

void createNewPllForGt(std::string gtName, std::string plType){
    std::string pllName = createPayloadList(plType);
    attachPayloadList(gtName, pllName);
}

nlohmann::json extractPllWithName(nlohmann::json plLists, std::string pllName){
    for (auto pll : plLists){
        if (pll["name"] == pllName){
            return pll["payload_iov"];
        }
    }
    std::string msg = "no pll with name '" + pllName + "' exists";
    throw NoPayloadException(msg);
}

void checkIovIsFree(std::string gtName, std::string plType, int majorIovStart, int minorIovStart){
    std::string pllName = getPayloadListName(gtName, plType);
    nlohmann::json plLists = getPayloadLists();
    nlohmann::json plList = extractPllWithName(plLists, pllName);
    for (auto pIov : plList){
        if (pIov["major_iov"]==majorIovStart && pIov["minor_iov"]==minorIovStart) {
            std::string msg = "piov with same major- and minor_iov already exists";
            throw NoPayloadException(msg);
        }
    }
}

void prepareInsertIov(std::string gtName, std::string plType, std::string fileUrl,
                          int majorIovStart, int minorIovStart){
    checkGtExists(gtName);
    checkPlTypeExists(plType);
    if (!gtHasPlType(gtName, plType)) {
        createNewPllForGt(gtName, plType);
    }
    else {
        //checkIovIsFree(gtName, plType, majorIovStart, minorIovStart);
    }
}

void prepareInsertIov(std::string gtName, std::string plType){
    checkGtExists(gtName);
    checkPlTypeExists(plType);
    if (!gtHasPlType(gtName, plType)) {
        createNewPllForGt(gtName, plType);
    }
}

void insertIov(std::string gtName, std::string plType, std::string fileUrl,
                          int majorIovStart, int minorIovStart){
    std::string remoteUrl = plmover::getRemoteUrl(gtName, plType, majorIovStart, minorIovStart);
    std::string pllName = getPayloadListName(gtName, plType);
    int piovId = createPayloadIOV(remoteUrl, majorIovStart, minorIovStart);
    attachPayloadIOV(pllName, piovId);
}

}
