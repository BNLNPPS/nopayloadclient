#pragma once

#include <nlohmann/json.hpp>

namespace nopayloadclient {
    std::string getGlobalTags();
    std::vector<std::string> getGlobalTagNames();
    nlohmann::json getJson();
}
