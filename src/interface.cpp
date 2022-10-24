#include <iostream>

#include <nlohmann/json.hpp>

#include <backend.hpp>
#include <plmover.hpp>
#include <exception.hpp>


namespace nopayloadclient {

/*
void _insertPayload(std::string gtName, std::string plType, std::string fileUrl,
                    int majorIovStart, int minorIovStart){
    std::string remoteUrl = plmover::getRemoteUrl(gtName, plType, majorIovStart, minorIovStart);
    plmover::checkPLStores(fileUrl, remoteUrl);
    backend::checkGtExists(gtName);
    std::string pllName;
    if (backend::gtHasPlType(gtName, plType)) {
        pllName = backend::getPayloadListName(gtName, plType);
    }
    else {
        pllName = backend::createPayloadList(plType);
        backend::attachPayloadList(gtName, pllName);
    }
    int piovId = backend::createPayloadIOV(remoteUrl, majorIovStart, minorIovStart);
    backend::attachPayloadIOV(pllName, piovId);
    plmover::uploadFile(fileUrl, gtName, plType, majorIovStart, minorIovStart);
}
*/

std::string _getPayloadUrl(std::string gtName, std::string plType, int majorIov, int minorIov){
    nlohmann::json j = backend::getPayloadIOVs(gtName, majorIov, minorIov);
    for (nlohmann::json piov : j){
        if (std::string(piov["payload_type"]) == plType){
            return piov["payload_iov"][0]["payload_url"];
        }
    }
    throw NoPayloadException("Could not find payload for type "+plType);
}


// Reading
nlohmann::json get(std::string gtName, std::string plType, int majorIov, int minorIov){
    try {
        std::string payloadUrl = _getPayloadUrl(gtName, plType, majorIov, minorIov);
        return nlohmann::json::object({{"code", 0}, {"msg", payloadUrl}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}


// Writing
nlohmann::json createGlobalTag(std::string gtName) {
    try {
        backend::createGlobalTagStatus("unlocked");
    }
    catch (NoPayloadException &e) {}
    try {
        backend::createGlobalTagObject(gtName, "unlocked");
        return nlohmann::json::object({{"code", 0}, {"msg", "successfully created global tag"}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}

nlohmann::json createPayloadType(std::string plType) {
    try {
        backend::createPayloadType(plType);
        return nlohmann::json::object({{"code", 0}, {"msg", "successfully created payload type"}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}

nlohmann::json insertPayload(std::string gtName, std::string plType, std::string fileUrl,
                             int majorIovStart, int minorIovStart){
    try {
        plmover::prepareUploadFile(gtName, plType, fileUrl, majorIovStart, minorIovStart);
        backend::prepareInsertIov(gtName, plType, fileUrl, majorIovStart, minorIovStart);
        plmover::uploadFile(gtName, plType, fileUrl, majorIovStart, minorIovStart);
        backend::insertIov(gtName, plType, fileUrl, majorIovStart, minorIovStart);
        //_insertPayload(gtName, plType, fileUrl, majorIovStart, minorIovStart);
        return nlohmann::json::object({{"code", 0}, {"msg", "successfully inserted payload"}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}

}