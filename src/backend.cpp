#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <curlwrapper.hpp>

std::string base_url = "http://localhost:8000/api/cdb_rest/";

namespace backend {

// Reading
nlohmann::json getGlobalTags() {
    return curlwrapper::get(base_url + "globalTags");
}

nlohmann::json getGlobalTagStatuses() {
    return curlwrapper::get(base_url + "gtstatus");
}

nlohmann::json getGlobalTagTypes() {
    return curlwrapper::get(base_url + "gttype");
}

nlohmann::json getPayloadTypes() {
    return curlwrapper::get(base_url + "pt");
}

nlohmann::json getPayloadLists() {
    return curlwrapper::get(base_url + "pl");
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

void attachPayloadList(std::string gtName, std::string plName){
    nlohmann::json j;
    j["payload_list"] = plName;
    j["global_tag"] = gtName;
    curlwrapper::put(base_url + "pl_attach", j);
}

void lockGlobalTag(std::string name){
    curlwrapper::put(base_url + "gt_change_status/" + name + "/locked");
}

void unlockGlobalTag(std::string name){
    curlwrapper::put(base_url + "gt_change_status/" + name + "/unlocked");
}

int createPayloadIOV(std::string plUrl, int majorIov, int minorIov){
    nlohmann::json j;
    j["payload_url"] = plUrl;
    j["major_iov"] = majorIov;
    j["minor_iov"] = minorIov;
    nlohmann::json res = curlwrapper::post(base_url + "piov", j);
    return res["id"];
}

int createPayloadIOV(std::string plUrl, int majorIov, int minorIov, int majorIovEnd, int minorIovEnd){
    nlohmann::json j;
    j["payload_url"] = plUrl;
    j["major_iov"] = majorIov;
    j["minor_iov"] = minorIov;
    j["major_iov_end"] = majorIovEnd;
    j["minor_iov_end"] = minorIovEnd;
    nlohmann::json res = curlwrapper::post(base_url + "piov", j);
    return res["id"];
}

void attachPayloadIOV(std::string plListName, int plIovId){
    nlohmann::json j;
    j["payload_list"] = plListName;
    j["piov_id"] = plIovId;
    nlohmann::json res = curlwrapper::put(base_url + "piov_attach", j);
}

}
