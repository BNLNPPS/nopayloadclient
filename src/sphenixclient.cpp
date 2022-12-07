#include <sphenixclient.hpp>

namespace sphenixclient {

// Reading
nlohmann::json get(std::string gtName, std::string plType, int iov){
    return nopayloadclient::get(gtName, plType, 0, iov);
}


// Writing
nlohmann::json createGlobalTag(std::string gtName) {
    return nopayloadclient::createGlobalTag(gtName);
}

nlohmann::json lockGlobalTag(std::string gtName) {
    return nopayloadclient::lockGlobalTag(gtName);
}

nlohmann::json unlockGlobalTag(std::string gtName) {
    return nopayloadclient::unlockGlobalTag(gtName);
}

nlohmann::json deleteGlobalTag(std::string gtName) {
    return nopayloadclient::deleteGlobalTag(gtName);
}

nlohmann::json createPayloadType(std::string plType) {
    return nopayloadclient::createPayloadType(plType);
}

nlohmann::json insertPayload(std::string gtName, std::string plType, std::string fileUrl,
                             int iovStart){
    return nopayloadclient::insertPayload(gtName, plType, fileUrl, 0, iovStart);
}

nlohmann::json insertPayload(std::string gtName, std::string plType, std::string fileUrl,
                             int iovStart, int iovEnd){
    return nopayloadclient::insertPayload(gtName, plType, fileUrl, 0, iovStart, 0, iovEnd);
}

}
