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
void createGlobalTagObject(std::string name, std::string status, std::string type) {
    /*
    nlohmann::json j;
    j["status"] = status;
    j["name"] = name;
    j["type"] = type;
    */
    std::string jsonstr;
    jsonstr = "{\"name\":\"" + name + "\"";
    jsonstr += ",\"status\":\"" + status + "\"";
    jsonstr += ",\"type\":\"" + type + "\"}";
    curlwrapper::post(base_url + "gt", jsonstr.c_str());
}


}
