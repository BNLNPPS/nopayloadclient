#pragma once

#include <nlohmann/json.hpp>

namespace curlwrapper {

// Reading
nlohmann::json getResponse(std::string url);

// Writing
//void post(std::string url, nlohmann::json jsonData);
void post(std::string url, const char* jsonCstr);


}
