#include <nopayloadclient/nopayloadclient.hpp>


namespace nopayloadclient {


// Reading
nlohmann::json get(std::string gt_name, std::string pl_type, long long major_iov, long long minor_iov){
    return makeResponse(backend::getPayloadUrls)(gt_name, pl_type, major_iov, minor_iov);
}


// Writing
std::string _createGlobalTag(std::string gt_name) {
    backend::createGlobalTag(gt_name);
    return "successfully created global tag";
}
nlohmann::json createGlobalTag(std::string gt_name) {
    return makeResponse(_createGlobalTag)(gt_name);
}

std::string _deleteGlobalTag(std::string gt_name) {
    backend::deleteGlobalTag(gt_name);
    return "successfully deleted global tag";
}
nlohmann::json deleteGlobalTag(std::string gt_name) {
    return makeResponse(_deleteGlobalTag)(gt_name);
}

std::string _lockGlobalTag(std::string gt_name) {
    backend::lockGlobalTag(gt_name);
    return "successfully locked global tag";
}
nlohmann::json lockGlobalTag(std::string gt_name) {
    return makeResponse(_lockGlobalTag)(gt_name);
}

std::string _unlockGlobalTag(std::string gt_name) {
    backend::unlockGlobalTag(gt_name);
    return "successfully unlocked global tag";
}
nlohmann::json unlockGlobalTag(std::string gt_name) {
    return makeResponse(_unlockGlobalTag)(gt_name);
}

std::string _createPayloadType(std::string pl_type) {
    backend::createPayloadType(pl_type);
    return "successfully created payload type";
}
nlohmann::json createPayloadType(std::string pl_type) {
    return makeResponse(_createPayloadType)(pl_type);
}

std::string _insertPayload(std::string gt_name, std::string pl_type, std::string fileUrl,
                             long long major_iovStart, long long minor_iovStart) {
    payload::Payload pl = payload::Payload(fileUrl, pl_type);
    plmover::prepareUploadFile(pl);
    backend::prepareInsertIov(gt_name, pl);
    plmover::uploadFile(pl);
    backend::insertIov(gt_name, pl, major_iovStart, minor_iovStart);
    return "successfully inserted payload";
}
nlohmann::json insertPayload(std::string gt_name, std::string pl_type, std::string fileUrl,
                             long long major_iovStart, long long minor_iovStart){
    return makeResponse(_insertPayload)(gt_name, pl_type, fileUrl, major_iovStart, minor_iovStart);
}

std::string _insertPayload2(std::string gt_name, std::string pl_type, std::string fileUrl,
                             long long major_iovStart, long long minor_iovStart,
                             long long major_iovEnd, long long minor_iovEnd){
    payload::Payload pl = payload::Payload(fileUrl, pl_type);
    plmover::prepareUploadFile(pl);
    backend::prepareInsertIov(gt_name, pl);
    plmover::uploadFile(pl);
    backend::insertIov(gt_name, pl, major_iovStart, minor_iovStart, major_iovEnd, minor_iovEnd);
    return "successfully inserted payload";
}

nlohmann::json insertPayload(std::string gt_name, std::string pl_type, std::string fileUrl,
                             long long major_iovStart, long long minor_iovStart,
                             long long major_iovEnd, long long minor_iovEnd){
    return makeResponse(_insertPayload2)(gt_name, pl_type, fileUrl, major_iovStart, minor_iovStart,
                                       major_iovEnd, minor_iovEnd);
}

// Helper
nlohmann::json getSize(){
    return makeResponse(backend::getSize)();
}

nlohmann::json getPayloadTypes(){
    return makeResponse(backend::getPayloadTypes)();
}

nlohmann::json getGlobalTags(){
    return makeResponse(backend::getGlobalTags)();
}

nlohmann::json checkConnection(){
    return makeResponse(backend::checkConnection)();
}

nlohmann::json getConfDict(){
    return makeResponse(config::getDict)();
}

}
