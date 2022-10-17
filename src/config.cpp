#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>


namespace config {

nlohmann::json _rawDict;

nlohmann::json fromFile(std::string filePath){
    std::cout<<"fromFile(filePath="<<filePath<<")"<<std::endl;
    nlohmann::json j;
    std::ifstream conf_file(filePath, std::ifstream::binary);
    conf_file >> j;
    return j;
}

nlohmann::json fromFile(){
    char* confPath = std::getenv("NOPAYLOADCLIENT_CONF");
    if (confPath==NULL){
        std::cout<<"variable NOPAYLOADCLIENT_CONF not set. Using default..."<<std::endl;
        return fromFile("../../config/test.json");
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