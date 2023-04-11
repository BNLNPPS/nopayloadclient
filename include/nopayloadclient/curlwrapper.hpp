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

class CurlWrapper {
public:
    CurlWrapper() {};
    CurlWrapper(const json& config);
    // Reading
    virtual json get(std::string url);
    // Writing
    virtual json del(std::string url);
    virtual json put(std::string url);
    virtual json put(std::string url, json jsonData);
    virtual json post(std::string url, json jsonData);
private:
    std::string base_url_;
    int n_retries_;
    bool print_time_stamps_;
};

struct Answer {
  CURLcode res;
  std::string readBuffer;
  long httpCode = 0;
};

class CurlSession{
    public:
        CurlSession(std::string _url, int n_retries, bool print_time_stamps);
        void printResults();
        json try_execute();
        json execute();
        void prepareGet();
        void prepareDelete();
        void preparePut();
        void preparePost(json jsonData);
        void preparePut(json jsonData);
    private:
        Answer ans;
        CURL *curl;
        std::string url;
        struct curl_slist *slist1 = NULL;
        std::string jsonStr;
        int n_retries_;
        bool print_time_stamps_;
};

}
