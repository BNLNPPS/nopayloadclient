#include <nopayloadclient/backend.hpp>

// Helper
bool objWithNameExists(const json& j, std::string name) {
    for (const auto& obj: j){
        if (obj["name"] == name) return true;
    }
    return false;
}

bool Backend::gtStatusExists(std::string name){
    json j = getGlobalTagStatuses();
    return objWithNameExists(j, name);
}

bool Backend::gtExists(){
    json j = getGlobalTags();
    return objWithNameExists(j, global_tag_);
}

bool Backend::plTypeExists(std::string pl_type){
    json j = getPayloadTypes();
    return objWithNameExists(j, pl_type);
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
    return (j.contains(pl_type));
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
                 {"n_pt", getPayloadTypes().size()}};
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

void Backend::createNewPll(std::string pl_type){
    std::string pll_name = createPayloadList(pl_type);
    attachPayloadList(pll_name);
}

void Backend::prepareInsertIov(payload::Payload& pl, npc::IOV& iov){
    checkGtExists();
    checkPlTypeExists(pl.type);
    if (!gtHasPlType(pl.type)) {
        std::cout << "gt " << global_tag_ << " has no pl type " << pl.type << std::endl;
        std::cout << "attempting to attach it..." << std::endl;
        createNewPll(pl.type);
    }
}

void Backend::insertIov(payload::Payload& pl, npc::IOV& iov) {
    std::string pll_name = getPayloadListName(pl.type);
    ll piov_id = createPayloadIOV(pl, iov);
    attachPayloadIOV(pll_name, piov_id);
}
