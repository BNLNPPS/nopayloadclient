#include <nopayloadclient/nopayloadclient.hpp>


namespace nopayloadclient {

Client::Client() {
    config_ = config::getDict();
    backend_ = new Backend(config_);
    plhandler_ = new PLHandler(config_);
}

Client::Client(std::string gt_name) {
//    Client();
    global_tag_ = gt_name;
    config_ = config::getDict();
    backend_ = new Backend(config_);
    plhandler_ = new PLHandler(config_);
}

// Configuring
json Client::setGlobalTag(std::string name) {
    TRY (
        global_tag_ = name;
        return "successfully changed global tag to " + name;
    )
}

// Reading
json Client::get(std::string pl_type, ll major_iov, ll minor_iov) {
    TRY(
        return makeResp(backend_->getPayloadUrls(global_tag_, pl_type, major_iov, minor_iov));
    )
}

json Client::getTypeUrlDict(ll major_iov, ll minor_iov){
    TRY(
        return makeResp(backend_->getTypeUrlDict(global_tag_, major_iov, minor_iov));
    )
}

// Writing
json Client::createGlobalTag() {
    TRY(
        backend_->createGlobalTag(global_tag_);
        return makeResp("successfully created global tag");
    )
}

json Client::deleteGlobalTag() {
    TRY(
        backend_->deleteGlobalTag(global_tag_);
        return makeResp("successfully deleted global tag");
    )
}

json Client::lockGlobalTag() {
    TRY(
        backend_->lockGlobalTag(global_tag_);
        return makeResp("successfully locked global tag");
    )
}

json Client::unlockGlobalTag() {
    TRY(
        backend_->unlockGlobalTag(global_tag_);
        return makeResp("successfully unlocked global tag");
    )
}

json Client::createPayloadType(std::string name) {
    TRY(
        backend_->createPayloadType(name);
        return makeResp("successfully created payload type");
    )
}

json Client::insertPayload(std::string pl_type, std::string file_url,
                           ll major_iov_start, ll minor_iov_start){
    return insertPayload(pl_type, file_url,
                         major_iov_start, minor_iov_start, -1, -1);
}

json Client::insertPayload(std::string pl_type, std::string file_url,
                           ll major_iov_start, ll minor_iov_start,
                           ll major_iov_end, ll minor_iov_end){
    TRY(
        payload::Payload pl = payload::Payload(file_url, pl_type);
        plhandler_->prepareUploadFile(pl);
        backend_->prepareInsertIov(global_tag_, pl);
        plhandler_->uploadFile(pl);
        backend_->insertIov(global_tag_, pl,
                            major_iov_start, minor_iov_start,
                            major_iov_end, minor_iov_end);
        return makeResp("successfully inserted payload");
    )
}

// Helper
json Client::getSize(){
    TRY(
        return makeResp(backend_->getSize());
    )
}

json Client::getPayloadTypes(){
    TRY(
        return makeResp(backend_->getPayloadTypes());
    )
}

json Client::getGlobalTags(){
    TRY(
        return makeResp(backend_->getGlobalTags());
    )
}

json Client::checkConnection(){
    TRY(
        return makeResp(backend_->checkConnection());
    )
}

json Client::getConfDict(){
    TRY(
        return makeResp(config::getDict());
    )
}

std::ostream& operator<<(std::ostream& os, const nopayloadclient::Client& c) {
    os << "Client instance with following attributes:" << std::endl;
    os << "global tag = " << c.global_tag_ << std::endl;
    os << "config = " << c.config_ << std::endl;
    return os;
}

template<typename T>
json Client::makeResp(T msg) {
    return {{"code", 0}, {"msg", msg}};
}

}
