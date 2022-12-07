#include <config.hpp>
#include "config.h"

namespace config {

std::vector keys = {"base_url", "api_res", "n_retries",
                    "write_dir", "read_dir_list"};

void _checkKeys(nlohmann::json j){
  for(auto key : keys){
    if (!j.contains(key)){
        std::cerr<<"config does not contain key <"<<key<<">"<<std::endl;
        exit(1);
    }
  }
}

nlohmann::json fromFile(std::string file_name){
    std::string fullPath = PROJECT_CONFIG_SEARCH_PATHS + file_name;
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
        std::cout << "variable NOPAYLOADCLIENT_CONF not set. Using default.json..." << std::endl;
        return fromFile("default.json");
    }
    return fromFile(confPath);
}


nlohmann::json dict = fromFile();
std::string api_url = (std::string) dict["api_url"];
std::string write_dir = (std::string) dict["write_dir"];
std::vector<std::string> read_dir_list = dict["read_dir_list"];
int n_retries = dict["n_retries"];
bool print_time_stamps = dict["print_time_stamps"];

}
