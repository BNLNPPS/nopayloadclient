#pragma once

#include <nlohmann/json.hpp>

namespace nopayloadclient {
    // Reading
    std::vector<std::string> getGlobalTagNames();

    // Writing
    void insertPayload(std::string gtName, std::string gtType, std::string fileUrl, int iovStart);

}
