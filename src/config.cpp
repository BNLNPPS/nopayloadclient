#include <nopayloadclient/config.hpp>
#include "config.h"

namespace nopayloadclient {

namespace config {

void _checkKeys(json j){
  std::vector<std::string> keys = {"base_url", "api_res", "n_retries", "retry_sleep_mean",
                                   "write_dir", "read_dir_list",
                                   "cache_life_time", "cache_max_mb",
                                   "print_time_stamps", "use_fake_backend",
                                   "logger", "log_level"};
  for(auto key : keys){
    if (!j.contains(key)){
        std::cerr<<"config does not contain key <"<<key<<">"<<std::endl;
        exit(1);
    }
  }
}

bool fileExists(const std::string& url) {
    struct stat buffer;
    return (stat (url.c_str(), &buffer) ==0);
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
    if (!fileExists(file_path)) {
        std::cerr << "config file does not exist: " << file_path << std::endl;
        exit(1);
    }
    std::ifstream conf_file(file_path, std::ifstream::binary);
    json j = json::parse(conf_file);
    _checkKeys(j);
    return j;
}

}

}




