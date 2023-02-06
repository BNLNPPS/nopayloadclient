#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <nlohmann/json.hpp>

#include <nopayloadclient/backend.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/plmover.hpp>
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

namespace nopayloadclient {

class Client {
public:
    Client();

    // Reading
    json get(std::string gt_name, std::string pl_type,
                       long long major_iov, long long minor_iov);

    // Writing
    json createPayloadType(std::string pl_type);
    json createGlobalTag(std::string gt_name);
    json deleteGlobalTag(std::string gt_name);
    json lockGlobalTag(std::string gt_name);
    json unlockGlobalTag(std::string gt_name);
    json insertPayload(std::string gt_name, std::string pl_type, std::string file_url,
                                 long long major_iov_start, long long minor_iov_start);
    json insertPayload(std::string gt_name, std::string pl_type, std::string file_url,
                                 long long major_iov_start, long long minor_iov_start,
                                 long long major_iov_end, long long minor_iov_end);

    // Helper (Read-only)
    json getSize();
    json getPayloadTypes();
    json getGlobalTags();
    json checkConnection();
    json getConfDict();
    friend std::ostream& operator<<(std::ostream& os, const nopayloadclient::Client& c);

private:
    json config_;
    Backend* backend_;
    PLMover* plmover_;
    template<typename T>
    json makeResp(T msg);
};

}
