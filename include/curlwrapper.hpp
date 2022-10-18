#pragma once

#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace curlwrapper {

struct Answer {
  CURLcode res;
  std::string readBuffer;
  long http_code = 0;
};

// Reading
nlohmann::json get(std::string url);

// Writing
nlohmann::json put(std::string url);
nlohmann::json put(std::string url, nlohmann::json jsonData);
nlohmann::json post(std::string url, nlohmann::json jsonData);

}
