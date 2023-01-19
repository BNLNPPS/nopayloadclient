#include <nopayloadclient/backend.hpp>

namespace backend {

CacheGroup cache;

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

nlohmann::json getPayloadIOVs() {
    return curlwrapper::get(config::api_url + "piov");
}

nlohmann::json _getPayloadLists(std::string gt_name) {
    return curlwrapper::get(config::api_url + "gtPayloadLists/" + gt_name);
}

nlohmann::json getPayloadIOVs(std::string gt_name, long long major_iov, long long minor_iov){
    checkGtExists(gt_name);
    return curlwrapper::get(config::api_url + "payloadiovs/?gtName=" + gt_name + "&majorIOV=" +
                            std::to_string(major_iov) + "&minorIOV=" + std::to_string(minor_iov));
}

std::vector<std::string> getGtStatusNames(){
    if (!cache.gt_status_names.is_valid){
        cache.gt_status_names.update(_getItemNames(getGlobalTagStatuses()));
    }
    return cache.gt_status_names.content;
}

std::vector<std::string> getGtNames(){
    if (!cache.gt_names.is_valid){
        cache.gt_names.update(_getItemNames(getGlobalTags()));
    }
    return cache.gt_names.content;
}

std::vector<std::string> getPtNames(){
    if (!cache.pt_names.is_valid){
        cache.pt_names.update(_getItemNames(getPayloadTypes()));
    }
    return cache.pt_names.content;
}

nlohmann::json getPayloadLists(std::string gt_name){
    if (!cache.pt_dict.is_valid || !cache.pt_dict.content.contains(gt_name)) {
        cache.pt_dict.update(nlohmann::json::object({{gt_name, _getPayloadLists(gt_name)}}));
    }
    return cache.pt_dict.content[gt_name];
}

bool gtStatusExists(std::string gtStatusName){
    std::vector<std::string> gtsns = getGtStatusNames();
    return std::find(gtsns.begin(), gtsns.end(), gtStatusName) != gtsns.end();
}

bool gtExists(std::string gt_name){
    std::vector<std::string> gtns = getGtNames();
    return std::find(gtns.begin(), gtns.end(), gt_name) != gtns.end();
}

bool plTypeExists(std::string plType){
    std::vector<std::string> ptns = getPtNames();
    return std::find(ptns.begin(), ptns.end(), plType) != ptns.end();
}

void checkGtStatusExists(std::string gtStatusName){
    if (!gtStatusExists(gtStatusName)){
        std::string msg = "no global tag status with name '"+gtStatusName+"' exists";
        throw BaseException(msg);
    }
}

void checkGtExists(std::string gt_name){
    if (!gtExists(gt_name)){
        std::string msg = "no global tag with name '"+gt_name+"' exists";
        throw BaseException(msg);
    }
}

void checkPlTypeExists(std::string plType){
    if (!plTypeExists(plType)){
        std::string msg = "no payload type with name '"+plType+"' exists";
        throw BaseException(msg);
    }
}

std::string getPayloadUrl(std::string gt_name, std::string plType, long long major_iov, long long minor_iov){
    nlohmann::json j = getPayloadIOVs(gt_name, major_iov, minor_iov);
    for (nlohmann::json piov : j){
        if (std::string(piov["payload_type"]) == plType){
            return piov["payload_iov"][0]["payload_url"];
        }
    }
    throw BaseException("Could not find payload for type "+plType);
}

std::vector<std::string> getPayloadUrls(std::string gt_name, std::string plType,
                                        long long major_iov, long long minor_iov){
    std::string payload_url = getPayloadUrl(gt_name, plType, major_iov, minor_iov);
    std::vector<std::string> payload_urls;
    for (const auto dir : config::read_dir_list) {
        payload_urls.push_back(dir + payload_url);
    }
    return payload_urls;
}

std::string getPayloadListName(std::string gt_name, std::string plType){
    nlohmann::json j = getPayloadLists(gt_name);
    if (!j.contains(plType)){
        std::string msg = "global tag '"+gt_name+"' does not have payload type '"+plType;
        throw BaseException(msg);
    }
    return j[plType];
}

bool gtHasPlType(std::string gt_name, std::string plType){
    nlohmann::json j = getPayloadLists(gt_name);
    if (!j.contains(plType)) return false;
    return true;
}

std::string checkConnection(){
    getGlobalTags();
    return "connection is good";
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
	          	           //{"n_iov_tot", getPayloadIOVs().size()},
     			                   {"n_pt", getPtNames().size()}});
}


// Writing
void createGlobalTagStatus(std::string status){
    nlohmann::json j;
    j["name"] = status;
    curlwrapper::post(config::api_url + "gtstatus", j);
    cache.invalidate();
}

void createGlobalTagObject(std::string name, std::string status) {
    nlohmann::json j;
    j["status"] = status;
    j["name"] = name;
    j["author"] = std::getenv("USER");
    curlwrapper::post(config::api_url + "gt", j);
    cache.invalidate();
}

void createPayloadType(std::string type){
    nlohmann::json j;
    j["name"] = type;
    curlwrapper::post(config::api_url + "pt", j);
    cache.invalidate();
}

std::string createPayloadList(std::string type){
    nlohmann::json j;
    j["payload_type"] = type;
    nlohmann::json res = curlwrapper::post(config::api_url + "pl", j);
    return res["name"];
}

void attachPayloadList(std::string gt_name, std::string plName){
    nlohmann::json j;
    j["payload_list"] = plName;
    j["global_tag"] = gt_name;
    curlwrapper::put(config::api_url + "pl_attach", j);
    cache.invalidate();
}

void lockGlobalTag(std::string name){
    curlwrapper::put(config::api_url + "gt_change_status/" + name + "/locked");
}

void unlockGlobalTag(std::string name){
    curlwrapper::put(config::api_url + "gt_change_status/" + name + "/unlocked");
}

long long createPayloadIOV(payload::Payload& pl, long long major_iov, long long minor_iov){
    nlohmann::json j;
    j["payload_url"] = pl.remote_url;
    j["major_iov"] = major_iov;
    j["minor_iov"] = minor_iov;
    j["checksum"] = pl.check_sum;
    nlohmann::json res = curlwrapper::post(config::api_url + "piov", j);
    return res["id"];
}

long long createPayloadIOV(payload::Payload& pl, long long major_iov, long long minor_iov, long long major_iovEnd, long long minor_iovEnd){
    nlohmann::json j;
    j["payload_url"] = pl.remote_url;
    j["major_iov"] = major_iov;
    j["minor_iov"] = minor_iov;
    j["major_iov_end"] = major_iovEnd;
    j["minor_iov_end"] = minor_iovEnd;
    j["checksum"] = pl.check_sum;
    nlohmann::json res = curlwrapper::post(config::api_url + "piov", j);
    return res["id"];
}

void attachPayloadIOV(std::string plListName, long long plIovId){
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

void deleteGlobalTag(std::string name) {
    curlwrapper::del(config::api_url + "deleteGlobalTag/" + name);
    cache.invalidate();
}

void createNewPllForGt(std::string gt_name, std::string plType){
    std::string pllName = createPayloadList(plType);
    attachPayloadList(gt_name, pllName);
}

void prepareInsertIov(std::string gt_name, payload::Payload& pl){
    checkGtExists(gt_name);
    checkPlTypeExists(pl.type);
    if (!gtHasPlType(gt_name, pl.type)) {
        createNewPllForGt(gt_name, pl.type);
    }
}

void insertIov(std::string gt_name, payload::Payload& pl,
               long long major_iovStart, long long minor_iovStart){
    std::string remoteUrl = pl.remote_url;
    std::string pllName = getPayloadListName(gt_name, pl.type);
    long long piovId = createPayloadIOV(pl, major_iovStart, minor_iovStart);
    attachPayloadIOV(pllName, piovId);
}

void insertIov(std::string gt_name, payload::Payload& pl,
               long long major_iovStart, long long minor_iovStart,
               long long major_iovEnd, long long minor_iovEnd){
    std::string remoteUrl = pl.remote_url;
    std::string pllName = getPayloadListName(gt_name, pl.type);
    long long piovId = createPayloadIOV(pl, major_iovStart, minor_iovStart, major_iovEnd, minor_iovEnd);
    attachPayloadIOV(pllName, piovId);
}

}
