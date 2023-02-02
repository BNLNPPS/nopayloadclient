#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>


namespace config {

    nlohmann::json getDict();

}
