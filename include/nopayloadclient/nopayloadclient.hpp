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


namespace nopayloadclient {

class Client {
public:
    Client();

    // Reading
    nlohmann::json get(std::string gt_name, std::string pl_type,
                       long long major_iov, long long minor_iov);

    // Writing
    nlohmann::json createPayloadType(std::string pl_type);
    nlohmann::json createGlobalTag(std::string gt_name);
    nlohmann::json deleteGlobalTag(std::string gt_name);
    nlohmann::json lockGlobalTag(std::string gt_name);
    nlohmann::json unlockGlobalTag(std::string gt_name);
    nlohmann::json insertPayload(std::string gt_name, std::string pl_type, std::string file_url,
                                 long long major_iov_start, long long minor_iov_start);
    nlohmann::json insertPayload(std::string gt_name, std::string pl_type, std::string file_url,
                                 long long major_iov_start, long long minor_iov_start,
                                 long long major_iov_end, long long minor_iov_end);

    // Helper (Read-only)
    nlohmann::json getSize();
    nlohmann::json getPayloadTypes();
    nlohmann::json getGlobalTags();
    nlohmann::json checkConnection();
    nlohmann::json getConfDict();
    friend std::ostream& operator<<(std::ostream& os, const nopayloadclient::Client& c);

private:
    nlohmann::json config_;
    Backend* backend_;
    PLMover* plmover_;
    template<typename T>
    nlohmann::json makeResp(T msg);
};

}
