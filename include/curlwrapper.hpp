#pragma once

#include <nlohmann/json.hpp>

namespace curlwrapper {

// Reading
nlohmann::json get(std::string url);

// Writing
nlohmann::json put(std::string url);
nlohmann::json put(std::string url, nlohmann::json jsonData);
nlohmann::json post(std::string url, nlohmann::json jsonData);

}
