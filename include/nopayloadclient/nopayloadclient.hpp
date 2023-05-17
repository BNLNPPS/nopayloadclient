#pragma once

#include <iostream>
#include <limits>
#include <nlohmann/json.hpp>

#include <nopayloadclient/resthandler.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/iov.hpp>
//#include <nopayloadclient/payloadiov.hpp>
#include <nopayloadclient/plhandler.hpp>
#include <nopayloadclient/config.hpp>
#include <nopayloadclient/exception.hpp>

#define NOPAYLOADCLIENT_TRY(...) {              \
    try {                       \
        __VA_ARGS__             \
    }                           \
    catch (BaseException &e) {  \
        return e.jsonify();     \
    }                           \
}                               \

namespace nopayloadclient {

using nlohmann::json;
using std::string;
using ll = long long;

class NoPayloadClient {
public:
    NoPayloadClient();
    NoPayloadClient(const string& gt_name);

    virtual json add(int a, int b) {
        std::cout << "NoPayloadClient::add(a=" << a << ", b=" << b << ")" << std::endl;
        return json {a+b};
    }

    // Configuration
    virtual json setGlobalTag(const string& name);
    virtual json getGlobalTag();
    virtual json override(const string& pl_type, const string& file_url);

    // Reading
    virtual json getUrlDict(ll major_iov, ll minor_iov);
    //virtual std::vector<PayloadIOV> getPayloadIOVs(ll major_iov, ll minor_iov);
    virtual json getPayloadIOVs(ll major_iov, ll minor_iov);

    // Writing
    virtual json createPayloadType(const string& pl_type);
    virtual json createGlobalTag(const string& name);
    virtual json createGlobalTag();
    virtual json deleteGlobalTag(const string& name);
    virtual json deleteGlobalTag();
    virtual json lockGlobalTag(const string& name);
    virtual json lockGlobalTag();
    virtual json unlockGlobalTag(const string& name);
    virtual json unlockGlobalTag();
    virtual json cloneGlobalTag(const string& source, const string& target);
    virtual json cloneGlobalTag(const string& target);
    virtual json insertPayload(const string& pl_type, const string& file_url,
                       ll major_iov_start, ll minor_iov_start);
    virtual json insertPayload(const string& pl_type, const string& file_url,
                       ll major_iov_start, ll minor_iov_start,
                       ll major_iov_end, ll minor_iov_end);

    // Helper
    virtual json getSize();
    virtual json getPayloadTypes();
    virtual json getGlobalTags();
    virtual json checkConnection();
    virtual json getConfDict();
    virtual json clearCache();
    friend std::ostream& operator<<(std::ostream& os, const NoPayloadClient& c);
    template<typename T>
    json makeResp(T msg);


private:
    json config_;
    json override_dict_;
    string global_tag_;
    RESTHandler rest_handler_;
    PLHandler pl_handler_;

    // Writing
    void prepareInsertIov(Payload &pl);
    void insertIov(Payload& pl, IOV& iov);
    void insertPayload(Payload &pl, IOV &iov);
    void createNewPll(const string& pl_type);

    // Reading
    bool gtExists(const string& name);
    bool gtStatusExists(const string& name);
    bool plTypeExists(const string& pl_type);
    bool gtHasPlType(const string& pl_type);
    void checkGtExists(const string& name);
    void checkGtDoesNotExist(const string& name);
    void checkGtStatusExists(const string& name);
    void checkPlTypeExists(const string& name);
    void applyOverriding(json& payload_iovs);
    //virtual json getUrlDict(const std::vector<PayloadIOV>& payload_iovs);
    json getUrlDict(const json& payload_iovs);

    // Helper
    bool objWithNameExists(const json& j, const string& name);
};

}
