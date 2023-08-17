#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <nopayloadclient/config.hpp>
#include <nopayloadclient/exception.hpp>
#include <nopayloadclient/curlwrapper.hpp>


namespace nopayloadclient {

using nlohmann::json;
using std::string;

class RealWrapper : public CurlWrapper {
public:
    RealWrapper() {};
    RealWrapper(const json& config);

    // Reading
    json get(const string& url);
    // Writing
    json del(const string& url);
    json put(const string& url);
    json put(const string& url, const json& data);
    json post(const string& url, const json& data);


private:
    string base_url_;
    int n_retries_;
    bool print_time_stamps_;
};

struct Answer {
  CURLcode res;
  string readBuffer;
  long httpCode = 0;
};

class CurlSession{
    public:
        CurlSession(const string& _url, int n_retries);
        void logResults();
        json try_execute();
        json execute();
        void prepareGet();
        void prepareDelete();
        void preparePut();
        void preparePost(const json& data);
        void preparePut(const json& data);
    private:
        Answer ans;
        CURL *curl;
        string url;
        struct curl_slist *slist1 = NULL;
        string json_str;
        int n_retries_;
};

}
