#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>


namespace config {

    extern std::string api_url;
    extern std::string remote_pl_dir;
    extern int n_retries;
    extern bool print_time_stamps;

}

