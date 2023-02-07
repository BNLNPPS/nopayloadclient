#include <nopayloadclient/nopayloadclient.hpp>

namespace nopayloadclient {

Client::Client() {
    config_ = config::getDict();
    backend_ = Backend(config_);
    plhandler_ = PLHandler(config_);
}

Client::Client(std::string gt_name) : Client() {
    backend_.setGlobalTag(gt_name);
}

// Configuring
json Client::setGlobalTag(std::string name) {
    TRY (
        backend_.setGlobalTag(name);
        return makeResp("successfully changed global tag to " + name);
    )
}

json Client::getGlobalTag() {
    TRY (
        return makeResp(backend_.getGlobalTag());
    )
}

// Reading
json Client::get(std::string pl_type, ll major_iov, ll minor_iov) {
    TRY(
        npc::Moment mom {major_iov, minor_iov};
        return makeResp(backend_.getPayloadUrls(pl_type, mom));
    )
}

json Client::getUrlDict(ll major_iov, ll minor_iov){
    TRY(
        npc::Moment mom {major_iov, minor_iov};
        return makeResp(backend_.getUrlDict(mom));
    )
}

// Writing
json Client::createGlobalTag() {
    TRY(
        backend_.createGlobalTag();
        return makeResp("successfully created global tag");
    )
}

json Client::deleteGlobalTag() {
    TRY(
        backend_.deleteGlobalTag();
        return makeResp("successfully deleted global tag");
    )
}

json Client::lockGlobalTag() {
    TRY(
        backend_.lockGlobalTag();
        return makeResp("successfully locked global tag");
    )
}

json Client::unlockGlobalTag() {
    TRY(
        backend_.unlockGlobalTag();
        return makeResp("successfully unlocked global tag");
    )
}

json Client::createPayloadType(std::string name) {
    TRY(
        backend_.createPayloadType(name);
        return makeResp("successfully created payload type");
    )
}

json Client::insertPayload(std::string pl_type, std::string file_url,
                           ll major_iov_start, ll minor_iov_start) {
    TRY(
        payload::Payload pl {file_url, pl_type};
        npc::IOV iov {major_iov_start, minor_iov_start};
        insertPayload(pl, iov);
        return makeResp("successfully inserted payload");
    )
}

json Client::insertPayload(std::string pl_type, std::string file_url,
                           ll major_iov_start, ll minor_iov_start,
                           ll major_iov_end, ll minor_iov_end) {
    TRY(
        payload::Payload pl {file_url, pl_type};
        npc::IOV iov {major_iov_start, minor_iov_start, major_iov_end, minor_iov_end};
        insertPayload(pl, iov);
        return makeResp("successfully inserted payload");
    )
}

void Client::insertPayload(payload::Payload &pl, npc::IOV &iov) {
    plhandler_.prepareUploadFile(pl);
    backend_.prepareInsertIov(pl);
    plhandler_.uploadFile(pl);
    backend_.insertIov(pl, iov);
}

// Helper
json Client::getSize(){
    TRY(
        return makeResp(backend_.getSize());
    )
}

json Client::getPayloadTypes(){
    TRY(
        return makeResp(backend_.getPayloadTypes());
    )
}

json Client::getGlobalTags(){
    TRY(
        return makeResp(backend_.getGlobalTags());
    )
}

json Client::checkConnection(){
    TRY(
        return makeResp(backend_.checkConnection());
    )
}

json Client::getConfDict(){
    TRY(
        return makeResp(config::getDict());
    )
}

std::ostream& operator<<(std::ostream& os, const nopayloadclient::Client& c) {
    os << "Client instance with following attributes:" << std::endl;
    os << "config = " << c.config_ << std::endl;
    return os;
}

template<typename T>
json Client::makeResp(T msg) {
    return {{"code", 0}, {"msg", msg}};
}

}
