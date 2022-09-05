#pragma once

#include <nlohmann/json.hpp>

namespace curlwrapper {

nlohmann::json getResponse(std::string url);

}
