#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "config.h"


namespace config {

nlohmann::json _rawDict;

nlohmann::json fromFile(std::string fileName){
    std::cout<<"fromFile(fileName="<<fileName<<")"<<std::endl;
    std::cout<<"INSTALL_CONFIG_SEARCH_PATHS ="<<std::endl;
    std::cout<<INSTALL_CONFIG_SEARCH_PATHS<<std::endl;
    std::cout<<"PROJECT_CONFIG_SEARCH_PATHS ="<<std::endl;
    std::cout<<PROJECT_CONFIG_SEARCH_PATHS<<std::endl;
    std::string fullPath = PROJECT_CONFIG_SEARCH_PATHS + fileName;
    std::cout<<"fullPath = "<<fullPath<<std::endl;
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

nlohmann::json rawDict(){
    if (_rawDict.is_null()){
        _rawDict = fromFile();
    }
    return _rawDict;
}

std::string apiUrl(){
    std::string apiUrl = "http://";
    apiUrl += rawDict()["base_url"];
    apiUrl += rawDict()["api_res"];
    return apiUrl;
}

}