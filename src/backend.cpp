#include <nopayloadclient/backend.hpp>


std::vector<std::string> Backend::getItemNames(json j) {
    std::vector<std::string> name_list;
    for (const auto& obj: j){
        name_list.push_back(obj["name"]);
    }
    return name_list;
}

// Private
json Backend::get(std::string url) {
    if (!use_cache_) return curlwrapper_->get(url);
    if (!cache_dict_.contains(url)) {
        cache_dict_[url] = curlwrapper_->get(url);
    }
    return cache_dict_[url];
}

json Backend::del(std::string url, bool trash_cache) {
    if (trash_cache) cache_dict_ = json();
    return curlwrapper_->del(url);
}

json Backend::put(std::string url, bool trash_cache) {
    if (trash_cache) cache_dict_ = json();
    return curlwrapper_->put(url);
}

json Backend::put(std::string url, json data, bool trash_cache) {
    if (trash_cache) cache_dict_ = json();
    return curlwrapper_->put(url, data);
}

json Backend::post(std::string url, json data, bool trash_cache) {
    if (trash_cache) cache_dict_ = json();
    return curlwrapper_->post(url, data);
}


// Reading
json Backend::getGlobalTags() {
    return get("globalTags");
}

json Backend::getGlobalTagStatuses() {
    return get("gtstatus");
}

json Backend::getPayloadTypes() {
    return get("pt");
}

json Backend::getPayloadLists(std::string gt_name) {
    return get("gtPayloadLists/" + gt_name);
}

json Backend::getPayloadIOVs(std::string gt_name, long long major_iov, long long minor_iov){
    checkGtExists(gt_name);
    return get("payloadiovs/?gtName=" + gt_name + "&majorIOV=" +
               std::to_string(major_iov) + "&minorIOV=" + std::to_string(minor_iov));
}

std::vector<std::string> Backend::getGtStatusNames(){
    return getItemNames(getGlobalTagStatuses());
}

std::vector<std::string> Backend::getGtNames(){
    return getItemNames(getGlobalTags());
}

std::vector<std::string> Backend::getPtNames(){
    return getItemNames(getPayloadTypes());
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
    json j = getPayloadIOVs(gt_name, major_iov, minor_iov);
    for (json piov : j){
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
    json j = getPayloadLists(gt_name);
    if (!j.contains(plType)){
        std::string msg = "global tag '"+gt_name+"' does not have payload type '"+plType;
        throw BaseException(msg);
    }
    return j[plType];
}

bool Backend::gtHasPlType(std::string gt_name, std::string plType){
    json j = getPayloadLists(gt_name);
    if (!j.contains(plType)) return false;
    return true;
}

std::string Backend::checkConnection(){
    getGlobalTags();
    return "connection is good";
}

json Backend::getSize(){
    int n_iov_attached = 0;
    int n_gt = 0;
    json global_tags = getGlobalTags();
    for (auto gt : global_tags){
        n_iov_attached += int(gt["payload_iov_count"]);
        n_gt += 1;
    }
    return json::object({{"n_global_tag", n_gt},
	                               {"n_iov_attached", n_iov_attached},
     			                   {"n_pt", getPtNames().size()}});
}


// Writing
void Backend::createGlobalTagStatus(std::string name){
    post("gtstatus", {{"name", name}});
}

void Backend::createGlobalTagObject(std::string name, std::string status) {
    json j = {
        {"name", name},
        {"status", status},
        {"author", std::getenv("USER")}
    };
    post("gt", j);
}

void Backend::createPayloadType(std::string name){
    post("pt", {{"name", name}});
}

std::string Backend::createPayloadList(std::string type){
    json res = post("pl", {{"payload_type", type}});
    return res["name"];
}

void Backend::attachPayloadList(std::string gt_name, std::string plName){
    json j = {
        {"payload_list", plName},
        {"global_tag", gt_name}
    };
    put("pl_attach", j);
}

void Backend::lockGlobalTag(std::string name){
    put("gt_change_status/" + name + "/locked");
}

void Backend::unlockGlobalTag(std::string name){
    put("gt_change_status/" + name + "/unlocked");
}

long long Backend::createPayloadIOV(payload::Payload& pl, long long major_iov, long long minor_iov){
    json j = {
        {"payload_url", pl.remote_url},
        {"major_iov", major_iov},
        {"minor_iov", minor_iov},
        {"checksum", pl.check_sum}
    };
    json res = post("piov", j);
    return res["id"];
}

long long Backend::createPayloadIOV(payload::Payload& pl, long long major_iov, long long minor_iov, long long major_iovEnd, long long minor_iovEnd){
    json j = {
        {"payload_url", pl.remote_url},
        {"major_iov", major_iov},
        {"minor_iov", minor_iov},
        {"major_iov_end", major_iovEnd},
        {"minor_iov_end", minor_iovEnd},
        {"checksum", pl.check_sum}
    };
    json res = post("piov", j);
    return res["id"];
}

void Backend::attachPayloadIOV(std::string plListName, long long plIovId){
    json j = {
        {"payload_list", plListName},
        {"piov_id", plIovId}
    };
    put("piov_attach", j);
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
                        long long major_iov_start, long long minor_iov_start){
    std::string remoteUrl = pl.remote_url;
    std::string pllName = getPayloadListName(gt_name, pl.type);
    long long piovId = createPayloadIOV(pl, major_iov_start, minor_iov_start);
    attachPayloadIOV(pllName, piovId);
}

void Backend::insertIov(std::string gt_name, payload::Payload& pl,
                        long long major_iov_start, long long minor_iov_start,
                        long long major_iov_end, long long minor_iov_end){
    std::string remoteUrl = pl.remote_url;
    std::string pllName = getPayloadListName(gt_name, pl.type);
    long long piov_id = createPayloadIOV(pl, major_iov_start, minor_iov_start, major_iov_end, minor_iov_end);
    attachPayloadIOV(pllName, piov_id);
}

Backend::Backend(const json& config) {
    std::vector<std::string> read_dir_list_ = config["read_dir_list"];
    use_cache_ = true;
    curlwrapper_ = new CurlWrapper(config);
    cache_dict_ = json();
}
