#pragma once

#include <nlohmann/json.hpp>

namespace curlwrapper {

// Reading
nlohmann::json getResponse(std::string url);

// Writing
nlohmann::json put(std::string url);
nlohmann::json post(std::string url, nlohmann::json jsonData);

}
