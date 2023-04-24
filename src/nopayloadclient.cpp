#include <nopayloadclient/nopayloadclient.hpp>

namespace nopayloadclient {

Client::Client() {
    config_ = config::getDict();
    rest_handler_ = RESTHandler(config_);
    pl_handler_ = PLHandler(config_);
}

Client::Client(const string& gt_name) : Client() {
    global_tag_ = gt_name;
}
//Client::Client(string gt_name) : Client() {
//    global_tag_ = gt_name;
//}

// Configuring
json Client::setGlobalTag(const string& name) {
    NOPAYLOADCLIENT_TRY (
        global_tag_ = name;
        return makeResp("successfully changed global tag to " + name);
    )
}

json Client::getGlobalTag() {
    NOPAYLOADCLIENT_TRY (
        return makeResp(global_tag_);
    )
}

json Client::override(const string& pl_type, const string& file_url) {
    NOPAYLOADCLIENT_TRY (
        pl_handler_.checkFileExists(file_url);
        override_dict_[pl_type] = file_url;
        return makeResp("successfully overrode payload type " + pl_type);
    )
}

// Reading
json Client::getUrlDict(ll major_iov, ll minor_iov){
    NOPAYLOADCLIENT_TRY(
        json payload_iovs;
        for (const json& piov : getPayloadIOVs(major_iov, minor_iov)) {
            // example filter
            if (piov["major_iov_end"] < 0) {
                continue;
            }
            payload_iovs.push_back(piov);
        }
        json url_dict = getUrlDict(payload_iovs);
        return makeResp(url_dict);
    )
}

// Writing
json Client::createGlobalTag(const string& name) {
    NOPAYLOADCLIENT_TRY(
        if (!gtStatusExists("unlocked")){
            rest_handler_.createGlobalTagStatus("unlocked");
        }
        rest_handler_.createGlobalTagObject(name, "unlocked");
        return makeResp("successfully created global tag");
    )
}

json Client::createGlobalTag() {
    return createGlobalTag(global_tag_);
}

json Client::deleteGlobalTag(const string& name) {
    NOPAYLOADCLIENT_TRY(
        rest_handler_.deleteGlobalTag(name);
        return makeResp("successfully deleted global tag");
    )
}

json Client::deleteGlobalTag() {
    return deleteGlobalTag(global_tag_);
}

json Client::lockGlobalTag(const string& name) {
    NOPAYLOADCLIENT_TRY(
        if (!gtStatusExists("locked")){
            rest_handler_.createGlobalTagStatus("locked");
        }
        rest_handler_.lockGlobalTag(name);
        return makeResp("successfully locked global tag");
    )
}

json Client::lockGlobalTag() {
    return lockGlobalTag(global_tag_);
}

json Client::unlockGlobalTag(const string& name) {
    NOPAYLOADCLIENT_TRY(
        if (!gtStatusExists("unlocked")){
            rest_handler_.createGlobalTagStatus("unlocked");
        }
        rest_handler_.unlockGlobalTag(name);
        return makeResp("successfully unlocked global tag");
    )
}

json Client::unlockGlobalTag() {
    return unlockGlobalTag(global_tag_);
}

json Client::cloneGlobalTag(const string& source, const string& target) {
    NOPAYLOADCLIENT_TRY(
        checkGtExists(source);
        checkGtDoesNotExist(target);
        rest_handler_.cloneGlobalTag(source, target);
        return makeResp("successfully cloned global tag");
    )
}

json Client::cloneGlobalTag(const string& target) {
    return cloneGlobalTag(global_tag_, target);
}

json Client::createPayloadType(const string& name) {
    NOPAYLOADCLIENT_TRY(
        rest_handler_.createPayloadType(name);
        return makeResp("successfully created payload type");
    )
}

json Client::insertPayload(const string& pl_type, const string& file_url,
                           ll major_iov_start, ll minor_iov_start) {
    NOPAYLOADCLIENT_TRY(
        Payload pl {file_url, pl_type};
        IOV iov {major_iov_start, minor_iov_start};
        insertPayload(pl, iov);
        return makeResp("successfully inserted payload");
    )
}

json Client::insertPayload(const string& pl_type, const string& file_url,
                           ll major_iov_start, ll minor_iov_start,
                           ll major_iov_end, ll minor_iov_end) {
    NOPAYLOADCLIENT_TRY(
        Payload pl {file_url, pl_type};
        IOV iov {major_iov_start, minor_iov_start, major_iov_end, minor_iov_end};
        insertPayload(pl, iov);
        return makeResp("successfully inserted payload");
    )
}

// Helper
json Client::getSize(){
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

json Client::getPayloadTypes(){
    NOPAYLOADCLIENT_TRY(
        return makeResp(rest_handler_.getPayloadTypes());
    )
}

json Client::getGlobalTags(){
    NOPAYLOADCLIENT_TRY(
        return makeResp(rest_handler_.getGlobalTags());
    )
}

json Client::checkConnection(){
    NOPAYLOADCLIENT_TRY(
        rest_handler_.getGlobalTags();
        return makeResp("connection is good");
    )
}

json Client::getConfDict(){
    NOPAYLOADCLIENT_TRY(
        return makeResp(config_);
    )
}

json Client::clearCache() {
    NOPAYLOADCLIENT_TRY(
        rest_handler_.clearCache();
        return makeResp("successfully cleared cache");
    )
}

std::ostream& operator<<(std::ostream& os, const Client& c) {
    os << "nopayloadclient::Client instance with following attributes:" << std::endl;
    os << "config = " << c.config_ << std::endl;
    return os;
}

template<typename T>
json Client::makeResp(T msg) {
    return {{"code", 0}, {"msg", msg}};
}

// Private
void Client::insertPayload(Payload &pl, IOV &iov) {
    prepareInsertIov(pl);
    pl_handler_.prepareUploadFile(pl);
    insertIov(pl, iov);
    pl_handler_.uploadFile(pl);
}

void Client::prepareInsertIov(Payload &pl) {
    checkGtExists(global_tag_);
    checkPlTypeExists(pl.type);
    if (!gtHasPlType(pl.type)) {
        std::cout << "gt " << global_tag_ << " has no pl type " << pl.type << std::endl;
        std::cout << "attempting to attach it..." << std::endl;
        createNewPll(pl.type);
    }
}

bool Client::gtStatusExists(const string& name){
    json j = rest_handler_.getGlobalTagStatuses();
    return objWithNameExists(j, name);
}

bool Client::gtExists(const string& name){
    json j = rest_handler_.getGlobalTags();
    return objWithNameExists(j, name);
}

bool Client::plTypeExists(const string& pl_type){
    json j = rest_handler_.getPayloadTypes();
    return objWithNameExists(j, pl_type);
}

void Client::checkGtStatusExists(const string& name){
    if (!gtStatusExists(name)){
        std::string msg = "no global tag status with name '"+name+"' exists";
        throw BaseException(msg);
    }
}

void Client::checkGtExists(const string& name){
    if (!gtExists(name)){
        std::string msg = "no global tag with name '"+name+"' exists";
        throw BaseException(msg);
    }
}

void Client::checkGtDoesNotExist(const string& name){
    if (gtExists(name)){
        std::string msg = "global tag with name '"+name+"' already exists";
        throw BaseException(msg);
    }
}

void Client::checkPlTypeExists(const string& pl_type){
    if (!plTypeExists(pl_type)){
        std::string msg = "no payload type with name '"+pl_type+"' exists";
        throw BaseException(msg);
    }
}

bool Client::gtHasPlType(const string& pl_type){
    json j = rest_handler_.getPayloadLists(global_tag_);
    return (j.contains(pl_type));
}

void Client::createNewPll(const string& pl_type){
    std::string pll_name = rest_handler_.createPayloadList(pl_type);
    rest_handler_.attachPayloadList(global_tag_, pll_name);
}

void Client::insertIov(Payload& pl, IOV& iov) {
    std::string pll_name = rest_handler_.getPayloadLists(global_tag_)[pl.type];
    ll piov_id = rest_handler_.createPayloadIOV(pl, iov);
    rest_handler_.attachPayloadIOV(pll_name, piov_id);
}

json Client::getPayloadIOVs(ll major_iov, ll minor_iov) {
    checkGtExists(global_tag_);
    Moment mom {major_iov, minor_iov};
    json payload_iovs = json::array();
    for (const json& el : rest_handler_.getPayloadIOVs(global_tag_, mom)) {
        if (el.type()==json::value_t::array) {
            payload_iovs.push_back({
                {"type", el[0]},
                {"payload_url", el[1]},
                {"checksum", el[2]},
                {"major_iov_start", el[3]},
                {"minor_iov_start", el[4]},
                {"major_iov_end", el[5]},
                {"minor_iov_end", el[6]},
            });
        }
        else {
            payload_iovs.push_back({
                {"type", el["payload_type"]},
                {"payload_url", el["payload_iov"][0]["payload_url"]},
                {"checksum", el["payload_iov"][0]["checksum"]},
                {"major_iov_start", el["payload_iov"][0]["major_iov"]},
                {"minor_iov_start", el["payload_iov"][0]["minor_iov"]},
                {"major_iov_end", el["payload_iov"][0]["major_iov_end"]},
                {"minor_iov_end", el["payload_iov"][0]["minor_iov_end"]},
            });
        }
    }
    return payload_iovs;
}

/*
std::vector<PayloadIOV> Client::getPayloadIOVs(ll major_iov, ll minor_iov) {
    checkGtExists(global_tag_);
    Moment mom {major_iov, minor_iov};
    std::vector<PayloadIOV> payload_iovs;
    for (const json& el : rest_handler_.getPayloadIOVs(global_tag_, mom)) {
        payload_iovs.push_back(PayloadIOV(el));
    }
    return payload_iovs;
}
*/
/*
json Client::getUrlDict(const std::vector<PayloadIOV>& payload_iovs) {
    json url_dict;
    for (const PayloadIOV& piov : payload_iovs) {
        Payload pl = piov.payload_;
        if (override_dict_.contains(pl.type)) {
            url_dict[pl.type] = override_dict_[pl.type];
            continue;
        }
        url_dict[pl.type] = pl_handler_.getFirstGoodUrl(pl);
    }
    return url_dict;
}
*/

json Client::getUrlDict(const json& payload_iovs) {
    json url_dict;
    for (const auto& piov : payload_iovs) {
        string type = piov["type"];
        if (override_dict_.contains(type)) {
            url_dict[type] = override_dict_[type];
            continue;
        }
        url_dict[type] = pl_handler_.getFirstGoodUrl(piov["payload_url"]);
    }
    return url_dict;
}


bool Client::objWithNameExists(const json& j, const string& name) {
    for (const auto& obj: j){
        if (obj["name"] == name) return true;
    }
    return false;
}

}
