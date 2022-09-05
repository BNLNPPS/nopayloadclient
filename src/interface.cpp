#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

#include <backend.hpp>


namespace nopayloadclient {

// Reading

std::vector<std::string> getGlobalTagNames() {
    std::vector<std::string> name_list;
    for (const auto& obj: backend::getGlobalTags()){
        name_list.push_back(obj["name"]);
    }
    return name_list;
}

// Writing

void insertPayload(std::string gtName, std::string gtType, std::string fileUrl, int iovStart){
    backend::createGlobalTagObject(gtName, "locked", gtType);
    std::cout<<"insertPayload()"<<std::endl;
}


}