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

nlohmann::json _getPayloadLists(std::string gt_name) {
    return curlwrapper::get(config::api_url + "gtPayloadLists/" + gt_name);
}

nlohmann::json getGlobalTagMap(std::string gt_name){
    return curlwrapper::get(config::api_url + "globalTag/" + gt_name);
}

nlohmann::json getPayloadIOVs(std::string gt_name, long long major_iov, long long minor_iov){
    return curlwrapper::get(config::api_url + "payloadiovs/?gtName=" + gt_name + "&majorIOV=" +
                            std::to_string(major_iov) + "&minorIOV=" + std::to_string(minor_iov));
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

nlohmann::json getPayloadLists(std::string gt_name){
    if (!cached_pt_dict.is_valid || !cached_pt_dict.content.contains(gt_name)) {
        cached_pt_dict.update(nlohmann::json::object({{gt_name, _getPayloadLists(gt_name)}}));
    }
    return cached_pt_dict.content[gt_name];
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
        throw NoPayloadException(msg);
    }
}

void checkGtExists(std::string gt_name){
    if (!gtExists(gt_name)){
        std::string msg = "no global tag with name '"+gt_name+"' exists";
        throw NoPayloadException(msg);
    }
}

void checkPlTypeExists(std::string plType){
    if (!plTypeExists(plType)){
        std::string msg = "no payload type with name '"+plType+"' exists";
        throw NoPayloadException(msg);
    }
}

std::string getPayloadUrl(std::string gt_name, std::string plType, long long major_iov, long long minor_iov){
    nlohmann::json j = getPayloadIOVs(gt_name, major_iov, minor_iov);
    for (nlohmann::json piov : j){
        if (std::string(piov["payload_type"]) == plType){
            return piov["payload_iov"][0]["payload_url"];
        }
    }
    throw NoPayloadException("Could not find payload for type "+plType);
}

std::string getPayloadListName(std::string gt_name, std::string plType){
    nlohmann::json j = getPayloadLists(gt_name);
    if (!j.contains(plType)){
        std::string msg = "global tag '"+gt_name+"' does not have payload type '"+plType;
        throw NoPayloadException(msg);
    }
    return j[plType];
}

long long getPayloadListId(std::string gt_name, std::string plType){
    nlohmann::json j = getPayloadLists(gt_name);
    if (!j.contains(plType)){
        std::string msg = "global tag '"+gt_name+"' does not have payload type '"+plType;
        throw NoPayloadException(msg);
    }
    return j[plType];
}

bool gtHasPlType(std::string gt_name, std::string plType){
    nlohmann::json j = getPayloadLists(gt_name);
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
    j["author"] = std::getenv("USER");
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

void attachPayloadList(std::string gt_name, std::string plName){
    nlohmann::json j;
    j["payload_list"] = plName;
    j["global_tag"] = gt_name;
    curlwrapper::put(config::api_url + "pl_attach", j);
    cached_pt_dict.is_valid = false;
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

void createNewPllForGt(std::string gt_name, std::string plType){
    std::string pllName = createPayloadList(plType);
    attachPayloadList(gt_name, pllName);
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
