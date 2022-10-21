#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <algorithm>

#include <backend.hpp>
#include <plmover.hpp>


namespace nopayloadclient {

// Reading
nlohmann::json get(std::string gtName, std::string plType, int majorIov, int minorIov){
    nlohmann::json j = backend::getPayloadIOVs(gtName, majorIov, minorIov);
    for (nlohmann::json piov : j){
        if (std::string(piov["payload_type"]) == plType){
            std::string remoteUrl = piov["payload_iov"][0]["payload_url"];
            return nlohmann::json::object({{"code", 200}, {"msg", remoteUrl}});
        }
    }
    return nlohmann::json::object({{"code", 404}, {"msg", "Could not find payload for type "+plType}});
}

// Writing
nlohmann::json insertPayload(std::string gtName, std::string plType, std::string fileUrl,
                             int majorIovStart, int minorIovStart){
    backend::checkGtExists(gtName);
    backend::checkPlTypeExists(plType);
    int piovId = backend::createPayloadIOV(fileUrl, majorIovStart, minorIovStart);
    std::string pllName = backend::getPayloadListName(gtName, plType);
    backend::attachPayloadIOV(pllName, piovId);
    try {
        plmover::uploadFile(fileUrl, gtName, plType, majorIovStart, minorIovStart);
    }
    catch (const char* msg) {
        std::cout<< msg <<std::endl;
    }
}

}