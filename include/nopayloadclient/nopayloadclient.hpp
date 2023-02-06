#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <nlohmann/json.hpp>

#include <nopayloadclient/backend.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/plhandler.hpp>
#include <nopayloadclient/config.hpp>
#include <nopayloadclient/exception.hpp>

#define TRY(...) {              \
    try {                       \
        __VA_ARGS__             \
    }                           \
    catch (BaseException &e) {  \
        return e.jsonify();     \
    }                           \
}                               \

using json = nlohmann::json;
using ll = long long;

namespace nopayloadclient {

class Client {
public:
    Client();
    Client(std::string gt_name);

    // use return of dict, not old 'get' method
    // rename: plmover -> plhandler
    // extract cache class: two dicts: {url: cont}, {url: timestamp}, use std::map for sorting
    // use virtual functions in interface
    // set global tag for reading + writing
    // use fucking smart pointers
    // use default arguments instead of overloading

    // Configuration
    json setGlobalTag(std::string name);

    // Reading
    json get(std::string pl_type, ll major_iov, ll minor_iov);
    json getTypeUrlDict(ll major_iov, ll minor_iov);

    // Writing
    json createPayloadType(std::string pl_type);
    json createGlobalTag();
    json deleteGlobalTag();
    json lockGlobalTag();
    json unlockGlobalTag();
    json insertPayload(std::string pl_type, std::string file_url,
                       ll major_iov_start, ll minor_iov_start);
    json insertPayload(std::string pl_type, std::string file_url,
                       ll major_iov_start, ll minor_iov_start,
                       ll major_iov_end, ll minor_iov_end);

    // Helper (Read-only)
    json getSize();
    json getPayloadTypes();
    json getGlobalTags();
    json checkConnection();
    json getConfDict();
    friend std::ostream& operator<<(std::ostream& os, const nopayloadclient::Client& c);

private:
    json config_;
    std::string global_tag_;
    Backend* backend_;
    PLHandler* plhandler_;

    template<typename T>
    json makeResp(T msg);
};

}
