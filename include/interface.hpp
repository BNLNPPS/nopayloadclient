#pragma once

#include <nlohmann/json.hpp>

namespace nopayloadclient {
    nlohmann::json getGlobalTags();
    std::vector<std::string> getGlobalTagNames();
    nlohmann::json getJson();
}
