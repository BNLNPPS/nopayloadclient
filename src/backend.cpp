#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <curlwrapper.hpp>

namespace backend {


nlohmann::json getGlobalTags() {
    return curlwrapper::getResponse("http://localhost:8000/api/cdb_rest/globalTags");
}


}
