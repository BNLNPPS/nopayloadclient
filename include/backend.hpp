#pragma once

#include <nlohmann/json.hpp>

namespace backend {
    nlohmann::json getResponse(std::string url);
}
