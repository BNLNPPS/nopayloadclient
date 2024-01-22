#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <cmath>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <nopayloadclient/config.hpp>
#include <nopayloadclient/exception.hpp>
#include <nopayloadclient/curlwrapper.hpp>


namespace nopayloadclient {

using nlohmann::json;
using std::string;

struct Answer {
  CURLcode res;
  string readBuffer;
  long httpCode = 0;
};

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

class CurlRequest{
    public:
        CurlRequest(const string& url, const json& data = json{}) {
            url_ = url;
            json_str_ = data.dump();
            curl_ = curl_easy_init();
            curl_easy_setopt(curl_, CURLOPT_URL, url_.c_str());
            curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &ans_.readBuffer);
        };
        json parseResponse();
        int execute();
        Answer ans_;

    protected:
        CURL *curl_;
        string url_;
        string json_str_;
};

class GetRequest: public CurlRequest {
    public:
        GetRequest(const string& url, const json& data = json{}) : CurlRequest(url, data) {
        };
};

class DeleteRequest: public CurlRequest {
    public:
        DeleteRequest(const string& url, const json& data = json{}) : CurlRequest(url, data) {
            curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "DELETE");
        };
};

class PostRequest: public CurlRequest {
    public:
        PostRequest(const string& _url, const json& data = json{}) : CurlRequest(_url, data) {
            struct curl_slist *slist = NULL;
            slist = curl_slist_append(slist, "Content-Type: application/json");
            curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, slist);
            curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, json_str_.c_str());
        };
};

class PutRequest: public PostRequest {
    public:
        PutRequest(const string& url, const json& data = json{}) : PostRequest(url, data) {
            curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "PUT");
        };
};


class RealWrapper : public CurlWrapper {
    private:
        void sleep(int retry_number);
        string base_url_;
        int n_retries_;
        int retry_sleep_mean_;

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

        template <typename Request>
        json getResponse(const string& url, const json& data = json{}) {
            for(int i=0; i<n_retries_; i++) {
                Request req = Request(base_url_ + url, data);
                if (req.execute() == 0) return req.parseResponse();
                sleep(i);
            }
            throw DataBaseException("Request failed after " + std::to_string(n_retries_) + " tries");
        }

};


} // nopayloadclient namespace
