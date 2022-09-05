#include <iostream>
#include "backend.hpp"

namespace nopayloadclient {

std::string getGlobalTags() {
    return backend::getResponse("http://localhost:8000/api/cdb_rest/globalTags");
}

/*
std::vector<std::string> getGlobalTagNames() {
    std::string raw_string = getGlobalTags();
    std::vector<std::string> name_list;
    return name_list;
}
*/

}