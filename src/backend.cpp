#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <curlwrapper.hpp>

std::string base_url = "http://localhost:8000/api/cdb_rest/";

namespace backend {

// Reading
nlohmann::json getGlobalTags() {
    return curlwrapper::getResponse(base_url + "globalTags");
}

// Writing
void createGlobalTagType(std::string type){
    nlohmann::json j;
    j["name"] = type;
    curlwrapper::post(base_url + "gttype", j);
}

void createGlobalTagStatus(std::string status){
    nlohmann::json j;
    j["name"] = status;
    curlwrapper::post(base_url + "gtstatus", j);
}

void createGlobalTagObject(std::string name, std::string status, std::string type) {
    nlohmann::json j;
    j["status"] = status;
    j["name"] = name;
    j["type"] = type;
    curlwrapper::post(base_url + "gt", j);
}

void createPayloadType(std::string type){
    nlohmann::json j;
    j["name"] = type;
    curlwrapper::post(base_url + "pt", j);
}

std::string createPayloadList(std::string type){
    nlohmann::json j;
    j["payload_type"] = type;
    nlohmann::json res = curlwrapper::post(base_url + "pl", j);
    return res["name"];
}

void attachPayloadList(std::string plName, std::string gtName){
    nlohmann::json j;
    j["payload_list"] = plName;
    j["global_tag"] = gtName;
    curlwrapper::post(base_url + "pl_attach", j);
}


}
