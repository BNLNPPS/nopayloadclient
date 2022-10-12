#pragma once

#include <nlohmann/json.hpp>

namespace nopayloadclient {
    // Reading
    std::string get(std::string gtName, std::string plType, int runNumber);
    std::vector<std::string> getGlobalTagNames();

    // Writing
    void insertPayload(std::string gtName, std::string plType, std::string fileUrl, int iovStart);

}
