#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

#include "config.h"


namespace config {

nlohmann::json _rawDict;
std::vector keys = {"base_url", "api_res", "remote_pl_dir", "n_retries"};

nlohmann::json fromFile(std::string fileName){
    std::string fullPath = PROJECT_CONFIG_SEARCH_PATHS + fileName;
    std::ifstream conf_file(fullPath, std::ifstream::binary);
    nlohmann::json j;
    conf_file >> j;
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

void _checkKeys(nlohmann::json j){
  for(auto key : keys){
    if (!j.contains(key)){
        std::cerr<<"config does not contain key <"<<key<<">"<<std::endl;
        exit(1);
    }
  }
}

nlohmann::json rawDict(){
    if (_rawDict.is_null()){
        _rawDict = fromFile();
    }
    _checkKeys(_rawDict);
    return _rawDict;
}

std::string apiUrl(){
    std::string apiUrl = "http://";
    apiUrl += rawDict()["base_url"];
    apiUrl += rawDict()["api_res"];
    return apiUrl;
}

}