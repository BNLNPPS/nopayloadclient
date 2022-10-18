#pragma once

#include <nlohmann/json.hpp>

namespace config {

nlohmann::json fromFile(std::string filePath);
nlohmann::json fromFile();

nlohmann::json rawDict();
std::string apiUrl();

void _checkKeys(nlohmann::json j);

}
