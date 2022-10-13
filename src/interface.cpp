#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <algorithm>

#include <backend.hpp>
#include <plmover.hpp>


namespace nopayloadclient {

// Reading
std::string get(std::string gtName, std::string plType, int runNumber){
    nlohmann::json j = backend::getPayloadIOVs(gtName, 0, runNumber);
    for (nlohmann::json piov : j){
        if (std::string(piov["payload_type"]) == plType){
            return std::string(piov["payload_iov"][0]["payload_url"]);
        }
    }
    std::cout<<"Could not find payload for type "<<plType<<std::endl;
    return "";
}

// Writing
void insertPayload(std::string gtName, std::string plType, std::string fileUrl, int iovStart){
    std::cout<<"insertPayload()"<<std::endl;
    backend::checkGtExists(gtName);
    backend::checkPlTypeExists(plType);
    int piovId = backend::createPayloadIOV(fileUrl, 1, iovStart);// set major iov to 1 for now
    std::string pllName = backend::getPayloadListName(gtName, plType);
    backend::attachPayloadIOV(pllName, piovId);
    plmover::uploadFile(fileUrl, gtName, plType, iovStart);
}

}