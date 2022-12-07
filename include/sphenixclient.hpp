#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient.hpp>

namespace sphenixclient {
    // Reading
    nlohmann::json get(std::string gtName, std::string plType, int iov);

    // Writing
    nlohmann::json createPayloadType(std::string plType);
    nlohmann::json createGlobalTag(std::string gtName);
    nlohmann::json lockGlobalTag(std::string gtName);
    nlohmann::json unlockGlobalTag(std::string gtName);
    nlohmann::json deleteGlobalTag(std::string gtName);
    nlohmann::json insertPayload(std::string gtName, std::string plType, std::string fileUrl,
                                 int iovStart);
    nlohmann::json insertPayload(std::string gtName, std::string plType, std::string fileUrl,
                                 int iovStart, int iovEnd);
}
