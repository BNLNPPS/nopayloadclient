#include <nopayloadclient/nopayloadclient.hpp>

namespace nopayloadclient {

NoPayloadClient::NoPayloadClient() {
    config_ = config::getDict();
    rest_handler_ = RESTHandler(config_);
    pl_handler_ = PLHandler(config_);
    logging::setLogger(config_["logger"]);
    logging::setLogLevel(config_["log_level"]);
}

NoPayloadClient::NoPayloadClient(const string& gt_name) : NoPayloadClient() {
    global_tag_ = gt_name;
}
//NoPayloadClient::NoPayloadClient(string gt_name) : NoPayloadClient() {
//    global_tag_ = gt_name;
//}

// Configuring
json NoPayloadClient::setGlobalTag(const string& name) {
    NOPAYLOADCLIENT_TRY (
        global_tag_ = name;
        return makeResp("successfully changed global tag to " + name);
    )
}

json NoPayloadClient::getGlobalTag() {
    NOPAYLOADCLIENT_TRY (
        return makeResp(global_tag_);
    )
}

json NoPayloadClient::override(const string& pl_type, const string& file_url) {
    NOPAYLOADCLIENT_TRY (
        pl_handler_.checkFileExists(file_url);
        override_dict_[pl_type] = file_url;
        return makeResp("successfully overrode payload type " + pl_type);
    )
}

// Reading
json NoPayloadClient::getUrlDict(ll major_iov, ll minor_iov){
    json resp = getPayloadIOVs(major_iov, minor_iov);
    if (resp["code"] != 0) return resp;
    for (auto it = resp["msg"].begin(); it != resp["msg"].end();) {
        if (it.value()["major_iov_end"] < major_iov) {
            it = resp["msg"].erase(it);
        }
        else ++it;
    }
    for (auto& piov : resp["msg"].items()) {
        piov.value() = piov.value()["payload_url"];
    }
    return resp;
}

json NoPayloadClient::getPayloadIOVs(ll major_iov, ll minor_iov) {
    NOPAYLOADCLIENT_TRY(
        checkGtExists(global_tag_);
        Moment mom {major_iov, minor_iov};
        json payload_iovs = rest_handler_.getPayloadIOVs(global_tag_, mom);
        pl_handler_.decoratePrefixes(payload_iovs);
        applyOverriding(payload_iovs);
        return makeResp(payload_iovs);
    )
}

// Writing
json NoPayloadClient::createGlobalTag(const string& name) {
    NOPAYLOADCLIENT_TRY(
        if (!gtStatusExists("unlocked")){
            rest_handler_.createGlobalTagStatus("unlocked");
        }
        rest_handler_.createGlobalTagObject(name, "unlocked");
        return makeResp("successfully created global tag");
    )
}

json NoPayloadClient::createGlobalTag() {
    return createGlobalTag(global_tag_);
}

json NoPayloadClient::deleteGlobalTag(const string& name) {
    NOPAYLOADCLIENT_TRY(
        rest_handler_.deleteGlobalTag(name);
        return makeResp("successfully deleted global tag");
    )
}

json NoPayloadClient::deleteGlobalTag() {
    return deleteGlobalTag(global_tag_);
}

json NoPayloadClient::lockGlobalTag(const string& name) {
    NOPAYLOADCLIENT_TRY(
        if (!gtStatusExists("locked")){
            rest_handler_.createGlobalTagStatus("locked");
        }
        rest_handler_.lockGlobalTag(name);
        return makeResp("successfully locked global tag");
    )
}

json NoPayloadClient::lockGlobalTag() {
    return lockGlobalTag(global_tag_);
}

json NoPayloadClient::unlockGlobalTag(const string& name) {
    NOPAYLOADCLIENT_TRY(
        if (!gtStatusExists("unlocked")){
            rest_handler_.createGlobalTagStatus("unlocked");
        }
        rest_handler_.unlockGlobalTag(name);
        return makeResp("successfully unlocked global tag");
    )
}

json NoPayloadClient::unlockGlobalTag() {
    return unlockGlobalTag(global_tag_);
}

json NoPayloadClient::cloneGlobalTag(const string& source, const string& target) {
    NOPAYLOADCLIENT_TRY(
        checkGtExists(source);
        checkGtDoesNotExist(target);
        rest_handler_.cloneGlobalTag(source, target);
        return makeResp("successfully cloned global tag");
    )
}

json NoPayloadClient::cloneGlobalTag(const string& target) {
    return cloneGlobalTag(global_tag_, target);
}

json NoPayloadClient::createPayloadType(const string& name) {
    NOPAYLOADCLIENT_TRY(
        rest_handler_.createPayloadType(name);
        return makeResp("successfully created payload type");
    )
}

json NoPayloadClient::deletePayloadType(const string& name) {
    NOPAYLOADCLIENT_TRY(
        for (auto gt : rest_handler_.getGlobalTags()){
            json pll_dict = rest_handler_.getPayloadLists(gt["name"]);
            if (pll_dict.contains(name)) {
                logging::info("payload type " + name + " has a payload list attached for gt " + gt["name"].get<std::string>());
                logging::info("attempting to delete it...");
                rest_handler_.deletePayloadList(pll_dict[name]);
            }
        }
        rest_handler_.deletePayloadType(name);
        return makeResp("successfully deleted payload type");
    )
}

json NoPayloadClient::insertPayload(const string& pl_type, const string& file_url,
                           ll major_iov_start, ll minor_iov_start) {
    NOPAYLOADCLIENT_TRY(
        Payload pl {file_url, pl_type};
        IOV iov {major_iov_start, minor_iov_start};
        insertPayload(pl, iov);
        return makeResp("successfully inserted payload");
    )
}

json NoPayloadClient::insertPayload(const string& pl_type, const string& file_url,
                           ll major_iov_start, ll minor_iov_start,
                           ll major_iov_end, ll minor_iov_end) {
    NOPAYLOADCLIENT_TRY(
        Payload pl {file_url, pl_type};
        IOV iov {major_iov_start, minor_iov_start, major_iov_end, minor_iov_end};
        insertPayload(pl, iov);
        return makeResp("successfully inserted payload");
    )
}

json NoPayloadClient::deletePayloadIOV(const string& pl_type,
                           ll major_iov_start, ll minor_iov_start) {
    NOPAYLOADCLIENT_TRY(
        checkGtExists(global_tag_);
        Moment mom {major_iov_start, minor_iov_start};
        json payload_iovs = rest_handler_.getPayloadIOVs(global_tag_, mom);
        IOV iov {major_iov_start, minor_iov_start, payload_iovs[pl_type]["major_iov_end"], payload_iovs[pl_type]["minor_iov_end"]};
        rest_handler_.deletePayloadIOV(global_tag_, pl_type, iov);
        return makeResp("successfully deleted payload iov");
    )
}

json NoPayloadClient::deletePayloadIOV(const string& pl_type,
                           ll major_iov_start, ll minor_iov_start,
                           ll major_iov_end, ll minor_iov_end) {
    NOPAYLOADCLIENT_TRY(
        checkGtExists(global_tag_);
        IOV iov {major_iov_start, minor_iov_start, major_iov_end, minor_iov_end};
        rest_handler_.deletePayloadIOV(global_tag_, pl_type, iov);
        return makeResp("successfully deleted payload iov");
    )
}

// Helper
json NoPayloadClient::getSize(){
    NOPAYLOADCLIENT_TRY(
        int n_iov_attached = 0;
        int n_gt = 0;
        for (auto gt : rest_handler_.getGlobalTags()){
            n_iov_attached += int(gt["payload_iov_count"]);
            n_gt += 1;
        }
        json j = {{"n_global_tag", n_gt},
                  {"n_iov_attached", n_iov_attached},
                  {"n_pt", rest_handler_.getPayloadTypes().size()}};
        return makeResp(j);
    )
}

json NoPayloadClient::getPayloadTypes(){
    NOPAYLOADCLIENT_TRY(
        return makeResp(rest_handler_.getPayloadTypes());
    )
}

json NoPayloadClient::getGlobalTags(){
    NOPAYLOADCLIENT_TRY(
        return makeResp(rest_handler_.getGlobalTags());
    )
}

json NoPayloadClient::checkConnection(){
    NOPAYLOADCLIENT_TRY(
        rest_handler_.getGlobalTags();
        return makeResp("connection is good");
    )
}

json NoPayloadClient::getConfDict(){
    NOPAYLOADCLIENT_TRY(
        return makeResp(config_);
    )
}

json NoPayloadClient::clearCache() {
    NOPAYLOADCLIENT_TRY(
        rest_handler_.clearCache();
        return makeResp("successfully cleared cache");
    )
}

std::ostream& operator<<(std::ostream& os, const NoPayloadClient& c) {
    os << "nopayloadclient::NoPayloadClient instance with following attributes:" << std::endl;
    os << "config = " << c.config_ << std::endl;
    return os;
}

template<typename T>
json NoPayloadClient::makeResp(T msg) {
    return {{"code", 0}, {"msg", msg}};
}

// Private
void NoPayloadClient::insertPayload(Payload &pl, IOV &iov) {
    prepareInsertIov(pl);
    pl_handler_.prepareUploadFile(pl);
    insertIov(pl, iov);
    pl_handler_.uploadFile(pl);
}

void NoPayloadClient::prepareInsertIov(Payload &pl) {
    checkGtExists(global_tag_);
    checkPlTypeExists(pl.type);
    if (!gtHasPlType(pl.type)) {
        logging::info("gt " + global_tag_ + " has no pl type " + pl.type);
        logging::info("attempting to attach it...");
        createNewPll(pl.type);
    }
}

bool NoPayloadClient::gtStatusExists(const string& name){
    json j = rest_handler_.getGlobalTagStatuses();
    return objWithNameExists(j, name);
}

bool NoPayloadClient::gtExists(const string& name){
    json j = rest_handler_.getGlobalTags();
    return objWithNameExists(j, name);
}

bool NoPayloadClient::plTypeExists(const string& pl_type){
    json j = rest_handler_.getPayloadTypes();
    return objWithNameExists(j, pl_type);
}

void NoPayloadClient::checkGtStatusExists(const string& name){
    if (!gtStatusExists(name)){
        std::string msg = "no global tag status with name '"+name+"' exists";
        throw BaseException(msg);
    }
}

void NoPayloadClient::checkGtExists(const string& name){
    if (!gtExists(name)){
        std::string msg = "no global tag with name '"+name+"' exists";
        throw BaseException(msg);
    }
}

void NoPayloadClient::checkGtDoesNotExist(const string& name){
    if (gtExists(name)){
        std::string msg = "global tag with name '"+name+"' already exists";
        throw BaseException(msg);
    }
}

void NoPayloadClient::checkPlTypeExists(const string& pl_type){
    if (!plTypeExists(pl_type)){
        std::string msg = "no payload type with name '"+pl_type+"' exists";
        throw BaseException(msg);
    }
}

bool NoPayloadClient::gtHasPlType(const string& pl_type){
    json j = rest_handler_.getPayloadLists(global_tag_);
    return (j.contains(pl_type));
}

void NoPayloadClient::createNewPll(const string& pl_type){
    std::string pll_name = rest_handler_.createPayloadList(pl_type);
    rest_handler_.attachPayloadList(global_tag_, pll_name);
}

void NoPayloadClient::insertIov(Payload& pl, IOV& iov) {
    std::string pll_name = rest_handler_.getPayloadLists(global_tag_)[pl.type];
    ll piov_id = rest_handler_.createPayloadIOV(pl, iov);
    rest_handler_.attachPayloadIOV(pll_name, piov_id);
}

void NoPayloadClient::applyOverriding(json& payload_iovs) {
    for (auto& el : payload_iovs.items()) {
        string type = el.key();
        if (override_dict_.contains(type)) {
            el.value()["payload_url"] = override_dict_[type];
            el.value()["major_iov_start"] = 0;
            el.value()["minor_iov_start"] = 0;
            el.value()["major_iov_end"] = std::numeric_limits<long long>::max();
            el.value()["minor_iov_end"] = std::numeric_limits<long long>::max();
        }
    }
}

bool NoPayloadClient::objWithNameExists(const json& j, const string& name) {
    for (const auto& obj: j){
        if (obj["name"] == name) return true;
    }
    return false;
}

bool objWithNameExists(const json& j, const string& name) {
    for (const auto& obj: j){
        if (obj["name"] == name) return true;
    }
    return false;
}

}
