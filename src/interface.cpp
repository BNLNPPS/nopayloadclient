#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <algorithm>

#include <backend.hpp>
#include <plmover.hpp>


namespace nopayloadclient {

// Reading
std::string get(std::string gtName, std::string plType, int runNumber){
    nlohmann::json j = backend::getGlobalTagMap(gtName);
    nlohmann::json payloadLists = j["payload_lists"];
    for (auto pll : payloadLists){
        std::cout<<"pll = "<<pll.dump(4)<<std::endl;
    }
    std::cout<<"j = "<<j.dump(4)<<std::endl;
    std::cout<<"payLoadLists = "<<payloadLists.dump(4)<<std::endl;
    return "hello fren";
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