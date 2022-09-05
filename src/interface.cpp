#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

#include <backend.hpp>


namespace nopayloadclient {

nlohmann::json getGlobalTags() {
    return backend::getResponse("http://localhost:8000/api/cdb_rest/globalTags");
}

std::vector<std::string> getGlobalTagNames() {
    std::vector<std::string> name_list;
    for (const auto& obj: getGlobalTags()){
        name_list.push_back(obj["name"]);
    }
    return name_list;
}


}