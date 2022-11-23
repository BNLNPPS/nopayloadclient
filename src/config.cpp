#include <iostream>
#include <fstream>
#include <vector>
#include <config.hpp>
#include <nlohmann/json.hpp>

#include "config.h"

namespace config {

std::vector keys = {"base_url", "api_res", "remote_pl_dir", "n_retries"};

void _checkKeys(nlohmann::json j){
  for(auto key : keys){
    if (!j.contains(key)){
        std::cerr<<"config does not contain key <"<<key<<">"<<std::endl;
        exit(1);
    }
  }
}

nlohmann::json fromFile(std::string fileName){
    std::string fullPath = PROJECT_CONFIG_SEARCH_PATHS + fileName;
    std::ifstream conf_file(fullPath, std::ifstream::binary);
    nlohmann::json j;
    conf_file >> j;
    _checkKeys(j);
    std::string api_url = "http://";
    api_url += j["base_url"];
    api_url += j["api_res"];
    j["api_url"] = api_url;
    return j;
}

nlohmann::json fromFile(){
    char* confPath = std::getenv("NOPAYLOADCLIENT_CONF");
    if (confPath==NULL){
        std::cout<<"variable NOPAYLOADCLIENT_CONF not set. Using default.json..."<<std::endl;
        return fromFile("default.json");
    }
    return fromFile(confPath);
}


nlohmann::json dict = fromFile();
std::string api_url = (std::string) dict["api_url"];
std::string remote_pl_dir = (std::string) dict["remote_pl_dir"];
int n_retries = dict["n_retries"];
bool print_time_stamps = dict["print_time_stamps"];

}
