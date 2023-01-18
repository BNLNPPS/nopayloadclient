#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>


namespace config {

    extern std::string api_url;
    extern std::string write_dir;
    extern std::vector<std::string> read_dir_list;
    extern int n_retries;
    extern bool print_time_stamps;
    nlohmann::json getDict();

}

