#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <config.hpp>
#include <exception.hpp>


namespace curlwrapper {

struct Answer {
  CURLcode res;
  std::string readBuffer;
  long httpCode = 0;
};

// Reading
nlohmann::json get(std::string url);

// Writing
nlohmann::json put(std::string url);
nlohmann::json put(std::string url, nlohmann::json jsonData);
nlohmann::json post(std::string url, nlohmann::json jsonData);

}
