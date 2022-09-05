#include <iostream>
#include <vector>

#include "nlohmann/json.hpp"
#include "backend.hpp"


//using json = nlohmann::json;

namespace nopayloadclient {

std::string getGlobalTags() {
    return backend::getResponse("http://localhost:8000/api/cdb_rest/globalTags");
}


std::vector<std::string> getGlobalTagNames() {
    std::string raw_string = getGlobalTags();
    std::cout<<"raw_string = "<<raw_string<<std::endl;
    for (auto i: raw_string){std::cout<<" "<<i;}
    nlohmann::json jsonski = nlohmann::json::parse(raw_string);
    std::cout<<"jsonski = "<<jsonski<<std::endl;
    std::vector<std::string> name_list;
    name_list.push_back("NIHAO");
    name_list.push_back("CHIGGA");
    return name_list;
}


}