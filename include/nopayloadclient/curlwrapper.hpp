#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <nopayloadclient/config.hpp>
#include <nopayloadclient/exception.hpp>


namespace nopayloadclient {

using nlohmann::json;
using std::string;

class CurlWrapper {
public:
    CurlWrapper() {};
  CurlWrapper(const json& /* config */) {};
    virtual ~CurlWrapper() = default;

    // Reading
    virtual json get(const string& url) = 0;
    // Writing
    virtual json del(const string& url) = 0;
    virtual json put(const string& url) = 0;
    virtual json put(const string& url, const json& data) = 0;
    virtual json post(const string& url, const json& data) = 0;
};

}
