#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace nopayloadclient {

    json getDict();

}
