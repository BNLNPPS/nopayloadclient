#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

#include <nopayloadclient/resthandler.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/iov.hpp>
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

    virtual json add(int a, int b) {
        std::cout << "Client::add(a=" << a << ", b=" << b << ")" << std::endl;
        return json {a+b};
    }

    // Configuration
    virtual json setGlobalTag(std::string name);
    virtual json getGlobalTag();
    virtual json override(std::string pl_type, std::string file_url);

    // Reading
    virtual json getUrlDict(ll major_iov, ll minor_iov);

    // Writing
    virtual json createPayloadType(std::string pl_type);
    virtual json createGlobalTag();
    virtual json deleteGlobalTag();
    virtual json lockGlobalTag();
    virtual json unlockGlobalTag();
    virtual json insertPayload(std::string pl_type, std::string file_url,
                       ll major_iov_start, ll minor_iov_start);
    virtual json insertPayload(std::string pl_type, std::string file_url,
                       ll major_iov_start, ll minor_iov_start,
                       ll major_iov_end, ll minor_iov_end);

    // Helper
    virtual json getSize();
    virtual json getPayloadTypes();
    virtual json getGlobalTags();
    virtual json checkConnection();
    virtual json getConfDict();
    virtual json clearCache();
    friend std::ostream& operator<<(std::ostream& os, const Client& c);
    template<typename T>
    json makeResp(T msg);

private:
    json config_;
    json override_dict_;
    RESTHandler rest_handler_;
    PLHandler pl_handler_;

    // Writing
    virtual void prepareInsertIov(Payload &pl);
    virtual void insertIov(Payload& pl, IOV& iov);
    virtual void insertPayload(Payload &pl, IOV &iov);
    virtual void createNewPll(std::string pl_type);

    // Reading
    virtual bool gtExists();
    virtual bool gtStatusExists(std::string name);
    virtual bool plTypeExists(std::string pl_type);
    virtual bool gtHasPlType(std::string pl_type);
    virtual void checkGtExists();
    virtual void checkGtStatusExists(std::string name);
    virtual void checkPlTypeExists(std::string name);
    virtual json _getUrlDict(Moment& mom);

    // Helper
    virtual bool objWithNameExists(const json& j, std::string name);
};

}
