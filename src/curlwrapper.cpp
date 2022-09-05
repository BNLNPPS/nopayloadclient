#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <curlwrapper.hpp>

namespace curlwrapper{

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

nlohmann::json getResponse(std::string url){
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        std::cout << readBuffer << std::endl;
    }
    return nlohmann::json::parse(readBuffer);
}

void post(std::string url, nlohmann::json jsonData){
    std::cout<<"curlwrapper::post()"<<std::endl;
    std::cout<<"jsonData.dump().c_str() = "<<jsonData.dump().c_str()<<std::endl;
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *slist1;
        slist1 = NULL;
        slist1 = curl_slist_append(slist1, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.dump().c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}


}