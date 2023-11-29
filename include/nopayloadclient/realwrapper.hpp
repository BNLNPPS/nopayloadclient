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

class CurlSession{
    public:
        CurlSession(const string& _url, const json& data = json{}) {
            url = _url;
            curl = curl_easy_init();
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ans.readBuffer);
        };
        void executeVoid();
        Answer ans;

    protected:
        CURL *curl;
        string url;
        struct curl_slist *slist1 = NULL;
        string json_str;
        int n_retries_;
};

class GetSession: public CurlSession {
    public:
        GetSession(const string& _url, const json& data = json{}) : CurlSession(_url, data) {
        };
};

class DeleteSession: public CurlSession {
    public:
        DeleteSession(const string& _url, const json& data = json{}) : CurlSession(_url, data) {
            std::cout << "DeleteSession()" << std::endl;
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        };
};

class PostSession: public CurlSession {
    public:
        PostSession(const string& _url, const json& data = json{}) : CurlSession(_url, data) {
            slist1 = curl_slist_append(slist1, "Content-Type: application/json");
            json_str = data.dump();
            if (json_str != "") {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());
            }
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
        };
};

class PutSession: public PostSession {
    public:
        PutSession(const string& _url, const json& data = json{}) : PostSession(_url, data) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        };
};


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

    template <typename T>
    json executeTemp(const string& url, const json& data = json{}) {
        for(int i = 0; i < n_retries_; i++){
            T cm = T(base_url_ + url, data);
            cm.executeVoid();
            logging::debug("cm.ans.httpCode = " + std::to_string(cm.ans.httpCode));
            if (cm.ans.httpCode == 504){
                int n_sleep = int(std::exp(i));
                logging::debug("connection timed-out. counter: " + std::to_string(i+1));
                logging::debug("sleeping for " + std::to_string(n_sleep) + " before retrying...");
                std::this_thread::sleep_for(std::chrono::seconds(n_sleep));
                continue;
            }
            json response = json::parse(cm.ans.readBuffer);
            if (cm.ans.httpCode!=200){
                std::string msg;
                if (response.contains("name")) msg = response["name"][0];
                else if (response.contains("detail")) msg = response["detail"];
                else msg = response.dump();
                throw DataBaseException(msg);
            }
            return response;
        }
        return json{};
    }

private:
    string base_url_;
    int n_retries_;
};


} // nopayloadclient namespace
