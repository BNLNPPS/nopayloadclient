#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>


namespace nopayloadclient {

using nlohmann::json;

json getDict();

}
