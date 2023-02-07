#include <nopayloadclient/backend.hpp>


std::vector<std::string> Backend::getItemNames(const json& j) {
    std::vector<std::string> name_list;
    for (const auto& obj: j){
        name_list.push_back(obj["name"]);
    }
    return name_list;
}

// Private
json Backend::get(std::string url) {
    if (!use_cache_) return curlwrapper_.get(url);
    if (!cache_dict_.contains(url)) {
        cache_dict_[url] = curlwrapper_.get(url);
    }
    return cache_dict_[url];
}

json Backend::del(std::string url, bool trash_cache) {
    if (trash_cache) cache_dict_ = json();
    return curlwrapper_.del(url);
}

json Backend::put(std::string url, bool trash_cache) {
    if (trash_cache) cache_dict_ = json();
    return curlwrapper_.put(url);
}

json Backend::put(std::string url, json data, bool trash_cache) {
    if (trash_cache) cache_dict_ = json();
    return curlwrapper_.put(url, data);
}

json Backend::post(std::string url, json data, bool trash_cache) {
    if (trash_cache) cache_dict_ = json();
    return curlwrapper_.post(url, data);
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

json Backend::getPayloadLists() {
    return get("gtPayloadLists/" + global_tag_);
}

json Backend::getPayloadIOVs(npc::Moment& mom){
    checkGtExists();
    return get("payloadiovs/?gtName=" + global_tag_ + "&majorIOV=" +
               std::to_string(mom.major) + "&minorIOV=" + std::to_string(mom.minor));
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

bool Backend::gtExists(){
    std::vector<std::string> gtns = getGtNames();
    return std::find(gtns.begin(), gtns.end(), global_tag_) != gtns.end();
}

bool Backend::plTypeExists(std::string pl_type){
    std::vector<std::string> ptns = getPtNames();
    return std::find(ptns.begin(), ptns.end(), pl_type) != ptns.end();
}

void Backend::checkGtStatusExists(std::string name){
    if (!gtStatusExists(name)){
        std::string msg = "no global tag status with name '"+name+"' exists";
        throw BaseException(msg);
    }
}

void Backend::checkGtExists(){
    if (!gtExists()){
        std::string msg = "no global tag with name '"+global_tag_+"' exists";
        throw BaseException(msg);
    }
}

void Backend::checkPlTypeExists(std::string pl_type){
    if (!plTypeExists(pl_type)){
        std::string msg = "no payload type with name '"+pl_type+"' exists";
        throw BaseException(msg);
    }
}

json Backend::getSuffixDict(npc::Moment& mom) {
    json suffix_dict;
    json j = getPayloadIOVs(mom);
    for (const json& el : j){
        std::string pl_type = el["payload_type"];
        std::string url = el["payload_iov"][0]["payload_url"];
        suffix_dict[pl_type] = url;
    }
    return suffix_dict;
}

json Backend::getUrlDict(npc::Moment& mom) {
    json j = getSuffixDict(mom);
    for (auto& el : j.items()){
        std::cout << "el = " << el << std::endl;
        std::cout << "el.key() = " << el.key() << std::endl;
        std::cout << "el.value() = " << el.value() << std::endl;
        std::cout << "(std::string)el.value() = " << (std::string)el.value() << std::endl;
        std::vector<std::string> urls;
        for (const auto dir : read_dir_list_) {
            urls.push_back(dir + (std::string)el.value());
        }

        el.value() = urls;
    }
    return j;
}

std::string Backend::getPayloadUrl(std::string pl_type, npc::Moment& mom){
    json j = getUrlDict(mom);
    if (!j.contains(pl_type)) {
        throw BaseException("Could not find payload for gt "+global_tag_+" and type "+pl_type);
    }
    return j[pl_type];
}

std::vector<std::string> Backend::getPayloadUrls(std::string pl_type,
                                                 npc::Moment& mom){
    std::string payload_url = getPayloadUrl(pl_type, mom);
    std::vector<std::string> payload_urls;
    for (const auto dir : read_dir_list_) {
        payload_urls.push_back(dir + payload_url);
    }
    return payload_urls;
}

std::string Backend::getPayloadListName(std::string pl_type){
    json j = getPayloadLists();
    if (!j.contains(pl_type)){
        std::string msg = "global tag '"+global_tag_+"' does not have payload type '"+pl_type;
        throw BaseException(msg);
    }
    return j[pl_type];
}

bool Backend::gtHasPlType(std::string pl_type){
    json j = getPayloadLists();
    if (!j.contains(pl_type)) return false;
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
    return json {{"n_global_tag", n_gt},
                 {"n_iov_attached", n_iov_attached},
                 {"n_pt", getPtNames().size()}};
}


// Writing
void Backend::createGlobalTagStatus(std::string name){
    post("gtstatus", {{"name", name}});
}

void Backend::createGlobalTagObject(std::string status) {
    json j = {
        {"name", global_tag_},
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

void Backend::attachPayloadList(std::string pl_name){
    json j = {
        {"payload_list", pl_name},
        {"global_tag", global_tag_}
    };
    put("pl_attach", j);
}

void Backend::lockGlobalTag(){
    put("gt_change_status/" + global_tag_ + "/locked");
}

void Backend::unlockGlobalTag(){
    put("gt_change_status/" + global_tag_ + "/unlocked");
}

ll Backend::createPayloadIOV(payload::Payload& pl, npc::IOV& iov){
    json j = {
        {"payload_url", pl.remote_url},
        {"major_iov", iov.start.major},
        {"minor_iov", iov.start.minor},
        {"checksum", pl.check_sum}
    };
    if (!iov.is_open){
        j["major_iov_end"] = iov.end.major;
        j["minor_iov_end"] = iov.end.minor;
    }
    json res = post("piov", j);
    return res["id"];
}

void Backend::attachPayloadIOV(std::string pll_name, ll piov_id){
    json j = {
        {"payload_list", pll_name},
        {"piov_id", piov_id}
    };
    put("piov_attach", j);
}

void Backend::createGlobalTag() {
    if (!gtStatusExists("unlocked")){
        createGlobalTagStatus("unlocked");
    }
    if (!gtStatusExists("locked")){
        createGlobalTagStatus("locked");
    }
    createGlobalTagObject("unlocked");
}

void Backend::deleteGlobalTag() {
    del("deleteGlobalTag/" + global_tag_);
}

void Backend::createNewPll(std::string pl_type){
    std::string pllName = createPayloadList(pl_type);
    attachPayloadList(pllName);
}

void Backend::prepareInsertIov(payload::Payload& pl){
    checkGtExists();
    checkPlTypeExists(pl.type);
    if (!gtHasPlType(pl.type)) {
        std::cout << "gt " << global_tag_ << " has no pl type " << pl.type << std::endl;
        std::cout << "attempting to attach it..." << std::endl;
        createNewPll(pl.type);
    }
}

void Backend::insertIov(payload::Payload& pl, npc::IOV& iov) {
    std::string remoteUrl = pl.remote_url;
    std::string pll_name = getPayloadListName(pl.type);
    ll piov_id = createPayloadIOV(pl, iov);
    attachPayloadIOV(pll_name, piov_id);
}

void Backend::setGlobalTag(std::string name) {
    global_tag_ = name;
}

std::string Backend::getGlobalTag() {
    return global_tag_;
}

Backend::Backend(const json& config) {
    std::vector<std::string> read_dir_list_ = config["read_dir_list"];
    use_cache_ = true;
    curlwrapper_ = CurlWrapper(config);
    cache_dict_ = json();
}
