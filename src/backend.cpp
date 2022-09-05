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
    //std::string data = '\{"name": "'+name+'", "status": "locked", "type": "testtype"\}';
    std::string data = "\{'name': 'LinosGT2', 'status': 'locked', 'type': 'testtype'\}";
    nlohmann::json j;
    j["status"] = "locked";
    curlwrapper::post(base_url + "gt", j);
}


}
