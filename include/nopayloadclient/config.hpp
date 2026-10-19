#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <nlohmann/json.hpp>
#include <sys/stat.h>


namespace nopayloadclient {

using nlohmann::json;

namespace config {

    json getDict();

}

}
