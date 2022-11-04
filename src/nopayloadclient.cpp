#include <iostream>

#include <nlohmann/json.hpp>

#include <backend.hpp>
#include <plmover.hpp>
#include <exception.hpp>


namespace nopayloadclient {


// Reading
nlohmann::json get(std::string gtName, std::string plType, int majorIov, int minorIov){
    try {
        std::string payloadUrl = backend::getPayloadUrl(gtName, plType, majorIov, minorIov);
        return nlohmann::json::object({{"code", 0}, {"msg", payloadUrl}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}

nlohmann::json getSize(){
    try {
        //nlohmann::json j = nlohmann::json::object({{"code", 0}, {"msg", "DINGELING"}});
        nlohmann::json j = backend::getSize();
        return nlohmann::json::object({{"code", 0}, {"msg", j}});
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
        return nlohmann::json::object({{"code", 0}, {"msg", "successfully inserted payload"}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}

}