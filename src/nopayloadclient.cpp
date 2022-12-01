#include <iostream>

#include <nlohmann/json.hpp>

#include <backend.hpp>
#include <payload.hpp>
#include <plmover.hpp>
#include <exception.hpp>


namespace nopayloadclient {


// Reading
nlohmann::json get(std::string gt_name, std::string pl_type, long long major_iov, long long minor_iov){
    try {
        std::string payloadUrl = backend::getPayloadUrl(gt_name, pl_type, major_iov, minor_iov);
        return nlohmann::json::object({{"code", 0}, {"msg", payloadUrl}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}

nlohmann::json getSize(){
    try {
        nlohmann::json j = backend::getSize();
        return nlohmann::json::object({{"code", 0}, {"msg", j}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}


// Writing
nlohmann::json createGlobalTag(std::string gt_name) {
    try {
        backend::createGlobalTag(gt_name);
        return nlohmann::json::object({{"code", 0}, {"msg", "successfully created global tag"}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}

nlohmann::json lockGlobalTag(std::string gt_name) {
    try {
        backend::lockGlobalTag(gt_name);
        return nlohmann::json::object({{"code", 0}, {"msg", "successfully locked global tag"}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}

nlohmann::json unlockGlobalTag(std::string gt_name) {
    try {
        backend::unlockGlobalTag(gt_name);
        return nlohmann::json::object({{"code", 0}, {"msg", "successfully unlocked global tag"}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}

nlohmann::json createPayloadType(std::string pl_type) {
    try {
        backend::createPayloadType(pl_type);
        return nlohmann::json::object({{"code", 0}, {"msg", "successfully created payload type"}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}

nlohmann::json insertPayload(std::string gt_name, std::string pl_type, std::string fileUrl,
                             long long major_iovStart, long long minor_iovStart){
    try {
        payload::Payload pl = payload::Payload(fileUrl, pl_type);
        plmover::prepareUploadFile(pl);
        backend::prepareInsertIov(gt_name, pl);
        plmover::uploadFile(pl);
        backend::insertIov(gt_name, pl, major_iovStart, minor_iovStart);
        return nlohmann::json::object({{"code", 0}, {"msg", "successfully inserted payload"}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}

nlohmann::json insertPayload(std::string gt_name, std::string pl_type, std::string fileUrl,
                             long long major_iovStart, long long minor_iovStart,
                             long long major_iovEnd, long long minor_iovEnd){
    try {
        payload::Payload pl = payload::Payload(fileUrl, pl_type);
        plmover::prepareUploadFile(pl);
        backend::prepareInsertIov(gt_name, pl);
        plmover::uploadFile(pl);
        backend::insertIov(gt_name, pl, major_iovStart, minor_iovStart, major_iovEnd, minor_iovEnd);
        return nlohmann::json::object({{"code", 0}, {"msg", "successfully inserted payload"}});
    }
    catch (NoPayloadException &e) {
        return nlohmann::json::object({{"code", 1}, {"msg", e.what()}});
    }
}

}
