#include <nopayloadclient/config.hpp>
#include "config.h"

namespace npc {

void _checkKeys(json j){
  std::vector<std::string> keys = {"base_url", "api_res", "n_retries",
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

std::string getFilePath() {
    char* env_char = std::getenv("NOPAYLOADCLIENT_CONF");
    if (env_char==NULL){
        std::cout << "variable NOPAYLOADCLIENT_CONF not set. Using default..." << std::endl;
        return std::string(PROJECT_CONFIG_SEARCH_PATHS) + "default.json";
    }
    return std::string( env_char );
}

json getDict(){
    std::string file_path = getFilePath();
    std::ifstream conf_file(file_path, std::ifstream::binary);
    json j;
    conf_file >> j;
    _checkKeys(j);
    return j;
}

}





