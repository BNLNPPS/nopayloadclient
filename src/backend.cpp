#include <nopayloadclient/backend.hpp>


std::vector<std::string> _getItemNames(nlohmann::json j) {
    std::vector<std::string> name_list;
    for (const auto& obj: j){
        name_list.push_back(obj["name"]);
    }
    return name_list;
}

// Private
nlohmann::json Backend::get(std::string url) {
    if (!use_cache_) return curlwrapper_->get(url);
    if (!cache_dict_.contains(url)) {
        cache_dict_[url] = curlwrapper_->get(url);
    }
    return cache_dict_[url];
}

nlohmann::json Backend::del(std::string url, bool trash_cache) {
    if (trash_cache) cache_dict_ = nlohmann::json();
    return curlwrapper_->del(url);
}

nlohmann::json Backend::put(std::string url, bool trash_cache) {
    if (trash_cache) cache_dict_ = nlohmann::json();
    return curlwrapper_->put(url);
}

nlohmann::json Backend::put(std::string url, nlohmann::json data, bool trash_cache) {
    if (trash_cache) cache_dict_ = nlohmann::json();
    return curlwrapper_->put(url, data);
}

nlohmann::json Backend::post(std::string url, nlohmann::json data, bool trash_cache) {
    if (trash_cache) cache_dict_ = nlohmann::json();
    return curlwrapper_->post(url, data);
}


// Reading

nlohmann::json Backend::getGlobalTags() {
    return get("globalTags");
}

nlohmann::json Backend::getGlobalTagStatuses() {
    return get("gtstatus");
}

nlohmann::json Backend::getPayloadTypes() {
    return get("pt");
}

nlohmann::json Backend::getPayloadLists(std::string gt_name) {
    return get("gtPayloadLists/" + gt_name);
}

nlohmann::json Backend::getPayloadIOVs(std::string gt_name, long long major_iov, long long minor_iov){
    checkGtExists(gt_name);
    return get("payloadiovs/?gtName=" + gt_name + "&majorIOV=" +
               std::to_string(major_iov) + "&minorIOV=" + std::to_string(minor_iov));
}

std::vector<std::string> Backend::getGtStatusNames(){
    return _getItemNames(getGlobalTagStatuses());
}

std::vector<std::string> Backend::getGtNames(){
    return _getItemNames(getGlobalTags());
}

std::vector<std::string> Backend::getPtNames(){
    return _getItemNames(getPayloadTypes());
}

bool Backend::gtStatusExists(std::string gtStatusName){
    std::vector<std::string> gtsns = getGtStatusNames();
    return std::find(gtsns.begin(), gtsns.end(), gtStatusName) != gtsns.end();
}

bool Backend::gtExists(std::string gt_name){
    std::vector<std::string> gtns = getGtNames();
    return std::find(gtns.begin(), gtns.end(), gt_name) != gtns.end();
}

bool Backend::plTypeExists(std::string plType){
    std::vector<std::string> ptns = getPtNames();
    return std::find(ptns.begin(), ptns.end(), plType) != ptns.end();
}

void Backend::checkGtStatusExists(std::string gtStatusName){
    if (!gtStatusExists(gtStatusName)){
        std::string msg = "no global tag status with name '"+gtStatusName+"' exists";
        throw BaseException(msg);
    }
}

void Backend::checkGtExists(std::string gt_name){
    if (!gtExists(gt_name)){
        std::string msg = "no global tag with name '"+gt_name+"' exists";
        throw BaseException(msg);
    }
}

void Backend::checkPlTypeExists(std::string plType){
    if (!plTypeExists(plType)){
        std::string msg = "no payload type with name '"+plType+"' exists";
        throw BaseException(msg);
    }
}

std::string Backend::getPayloadUrl(std::string gt_name, std::string plType, long long major_iov, long long minor_iov){
    nlohmann::json j = getPayloadIOVs(gt_name, major_iov, minor_iov);
    for (nlohmann::json piov : j){
        if (std::string(piov["payload_type"]) == plType){
            return piov["payload_iov"][0]["payload_url"];
        }
    }
    throw BaseException("Could not find payload for type "+plType);
}

std::vector<std::string> Backend::getPayloadUrls(std::string gt_name, std::string plType,
                                        long long major_iov, long long minor_iov){
    std::string payload_url = getPayloadUrl(gt_name, plType, major_iov, minor_iov);
    std::vector<std::string> payload_urls;
    for (const auto dir : read_dir_list_) {
        payload_urls.push_back(dir + payload_url);
    }
    return payload_urls;
}

std::string Backend::getPayloadListName(std::string gt_name, std::string plType){
    nlohmann::json j = getPayloadLists(gt_name);
    if (!j.contains(plType)){
        std::string msg = "global tag '"+gt_name+"' does not have payload type '"+plType;
        throw BaseException(msg);
    }
    return j[plType];
}

bool Backend::gtHasPlType(std::string gt_name, std::string plType){
    nlohmann::json j = getPayloadLists(gt_name);
    if (!j.contains(plType)) return false;
    return true;
}

std::string Backend::checkConnection(){
    getGlobalTags();
    return "connection is good";
}

nlohmann::json Backend::getSize(){
    int n_iov_attached = 0;
    int n_gt = 0;
    nlohmann::json global_tags = getGlobalTags();
    for (auto gt : global_tags){
        n_iov_attached += int(gt["payload_iov_count"]);
        n_gt += 1;
    }
    return nlohmann::json::object({{"n_global_tag", n_gt},
	                               {"n_iov_attached", n_iov_attached},
     			                   {"n_pt", getPtNames().size()}});
}


// Writing
void Backend::createGlobalTagStatus(std::string status){
    nlohmann::json j;
    j["name"] = status;
    post("gtstatus", j);
}

void Backend::createGlobalTagObject(std::string name, std::string status) {
    nlohmann::json j;
    j["status"] = status;
    j["name"] = name;
    j["author"] = std::getenv("USER");
    post("gt", j);
}

void Backend::createPayloadType(std::string type){
    nlohmann::json j;
    j["name"] = type;
    post("pt", j);
}

std::string Backend::createPayloadList(std::string type){
    nlohmann::json j;
    j["payload_type"] = type;
    nlohmann::json res = post("pl", j);
    return res["name"];
}

void Backend::attachPayloadList(std::string gt_name, std::string plName){
    nlohmann::json j;
    j["payload_list"] = plName;
    j["global_tag"] = gt_name;
    put("pl_attach", j);
}

void Backend::lockGlobalTag(std::string name){
    put("gt_change_status/" + name + "/locked");
}

void Backend::unlockGlobalTag(std::string name){
    put("gt_change_status/" + name + "/unlocked");
}

long long Backend::createPayloadIOV(payload::Payload& pl, long long major_iov, long long minor_iov){
    nlohmann::json j;
    j["payload_url"] = pl.remote_url;
    j["major_iov"] = major_iov;
    j["minor_iov"] = minor_iov;
    j["checksum"] = pl.check_sum;
    nlohmann::json res = post("piov", j);
    return res["id"];
}

long long Backend::createPayloadIOV(payload::Payload& pl, long long major_iov, long long minor_iov, long long major_iovEnd, long long minor_iovEnd){
    nlohmann::json j;
    j["payload_url"] = pl.remote_url;
    j["major_iov"] = major_iov;
    j["minor_iov"] = minor_iov;
    j["major_iov_end"] = major_iovEnd;
    j["minor_iov_end"] = minor_iovEnd;
    j["checksum"] = pl.check_sum;
    nlohmann::json res = post("piov", j);
    return res["id"];
}

void Backend::attachPayloadIOV(std::string plListName, long long plIovId){
    nlohmann::json j;
    j["payload_list"] = plListName;
    j["piov_id"] = plIovId;
    nlohmann::json res = put("piov_attach", j);
}

void Backend::createGlobalTag(std::string name) {
    if (!gtStatusExists("unlocked")){
        createGlobalTagStatus("unlocked");
    }
    if (!gtStatusExists("locked")){
        createGlobalTagStatus("locked");
    }
    createGlobalTagObject(name, "unlocked");
}

void Backend::deleteGlobalTag(std::string name) {
    del("deleteGlobalTag/" + name);
}

void Backend::createNewPllForGt(std::string gt_name, std::string plType){
    std::string pllName = createPayloadList(plType);
    attachPayloadList(gt_name, pllName);
}

void Backend::prepareInsertIov(std::string gt_name, payload::Payload& pl){
    std::cout << "Backend::prepareInsertIov()" << std::endl;
    checkGtExists(gt_name);
    checkPlTypeExists(pl.type);
    if (!gtHasPlType(gt_name, pl.type)) {
        std::cout << "gt " << gt_name << " has no pl type " << pl.type << std::endl;
        std::cout << "attempting to attach it..." << std::endl;
        createNewPllForGt(gt_name, pl.type);
    }
}

void Backend::insertIov(std::string gt_name, payload::Payload& pl,
               long long major_iovStart, long long minor_iovStart){
    std::cout << "insertIov()" << std::endl;
    std::string remoteUrl = pl.remote_url;
    std::string pllName = getPayloadListName(gt_name, pl.type);
    long long piovId = createPayloadIOV(pl, major_iovStart, minor_iovStart);
    attachPayloadIOV(pllName, piovId);
}

void Backend::insertIov(std::string gt_name, payload::Payload& pl,
               long long major_iovStart, long long minor_iovStart,
               long long major_iovEnd, long long minor_iovEnd){
    std::string remoteUrl = pl.remote_url;
    std::string pllName = getPayloadListName(gt_name, pl.type);
    long long piov_id = createPayloadIOV(pl, major_iovStart, minor_iovStart, major_iovEnd, minor_iovEnd);
    attachPayloadIOV(pllName, piov_id);
}

Backend::Backend(const nlohmann::json& config) {
    std::vector<std::string> read_dir_list_ = config["read_dir_list"];
    use_cache_ = true;
    curlwrapper_ = new CurlWrapper(config);
    cache_dict_ = nlohmann::json();
}
