#include <config.hpp>
#include "config.h"

namespace config {


void _checkKeys(nlohmann::json j){
  std::vector keys = {"base_url", "api_res", "n_retries",
                      "write_dir", "read_dir_list"};
  for(auto key : keys){
    if (!j.contains(key)){
        std::cerr<<"config does not contain key <"<<key<<">"<<std::endl;
        exit(1);
    }
  }
}

std::string getDefaultConfPath() {
    std::string path = PROJECT_CONFIG_SEARCH_PATHS;
    path += "default.json";
    return path;
}

nlohmann::json fromFile(std::string path_to_file){
    std::ifstream conf_file(path_to_file, std::ifstream::binary);
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
        std::string default_path = getDefaultConfPath();
        std::cout << "variable NOPAYLOADCLIENT_CONF not set. Using " << default_path << std::endl;
        return fromFile(default_path);
    }
    return fromFile(confPath);
}

}

nlohmann::json dict = config::fromFile();
std::string config::api_url = (std::string) dict["api_url"];
std::string config::write_dir = (std::string) dict["write_dir"];
std::vector<std::string> config::read_dir_list = dict["read_dir_list"];
int config::n_retries = dict["n_retries"];
bool config::print_time_stamps = dict["print_time_stamps"];
nlohmann::json config::getDict() {return dict;}
