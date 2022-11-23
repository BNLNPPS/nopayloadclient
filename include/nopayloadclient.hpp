#pragma once

#include <nlohmann/json.hpp>

namespace nopayloadclient {
    // Reading
    nlohmann::json get(std::string gtName, std::string plType, int majorIov, int minorIov);
    nlohmann::json getSize();

    // Writing
    nlohmann::json createPayloadType(std::string plType);
    nlohmann::json createGlobalTag(std::string gtName);
    nlohmann::json lockGlobalTag(std::string gtName);
    nlohmann::json unlockGlobalTag(std::string gtName);
    nlohmann::json insertPayload(std::string gtName, std::string plType, std::string fileUrl,
                                 int majorIovStart, int minorIovStart);
    nlohmann::json insertPayload(std::string gtName, std::string plType, std::string fileUrl,
                                 int majorIovStart, int minorIovStart,
                                 int majorIovEnd, int minorIovEnd);
}
