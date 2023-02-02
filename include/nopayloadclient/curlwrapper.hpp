#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <nopayloadclient/config.hpp>
#include <nopayloadclient/exception.hpp>


class CurlWrapper {
public:
    CurlWrapper(const nlohmann::json& config);
    // Reading
    nlohmann::json get(std::string url);
    // Writing
    nlohmann::json del(std::string url);
    nlohmann::json put(std::string url);
    nlohmann::json put(std::string url, nlohmann::json jsonData);
    nlohmann::json post(std::string url, nlohmann::json jsonData);
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
        nlohmann::json try_execute();
        nlohmann::json execute();
        void prepareGet();
        void prepareDelete();
        void preparePut();
        void preparePost(nlohmann::json jsonData);
        void preparePut(nlohmann::json jsonData);
    private:
        Answer ans;
        CURL *curl;
        std::string url;
        struct curl_slist *slist1 = NULL;
        std::string jsonStr;
        int n_retries_;
        bool print_time_stamps_;

};