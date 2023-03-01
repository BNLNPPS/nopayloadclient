#pragma once

#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

#include <nopayloadclient/curlwrapper.hpp>
#include <nopayloadclient/curlfaker.hpp>
#include <nopayloadclient/cache.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/iov.hpp>
#include <nopayloadclient/moment.hpp>
#include <nopayloadclient/exception.hpp>

using json = nlohmann::json;

namespace nopayloadclient {

class RESTHandler {
public:
    RESTHandler() {};
    RESTHandler(const json& config);

    // Configuring
    void clearCache();

    // Reading
    json getGlobalTags();
    json getGlobalTagStatuses();
    json getPayloadTypes();
    json getPayloadLists(std::string global_tag);
    json getPayloadIOVs(std::string global_tag, Moment& mom);

    // Writing
    void createGlobalTagStatus(std::string name);
    void createGlobalTagObject(std::string name, std::string status);
    void createPayloadType(std::string name);
    void attachPayloadList(std::string global_tag, std::string pl_name);
    void lockGlobalTag(std::string name);
    void unlockGlobalTag(std::string name);
    void deleteGlobalTag(std::string name);
    void cloneGlobalTag(std::string source, std::string target);
    void attachPayloadIOV(std::string pll_name, long long piov_id);
    std::string createPayloadList(std::string type);
    long long createPayloadIOV(Payload& pl, IOV& iov);

private:
    //CurlWrapper curlwrapper_;
    std::unique_ptr<CurlWrapper> curlwrapper_;
    Cache cache_;
    bool use_cache_;

    json get(std::string url);
    json del(std::string url, bool trash_cache=true);
    json put(std::string url, bool trash_cache=true);
    json put(std::string url, json data, bool trash_cache=true);
    json post(std::string url, json data, bool trash_cache=true);
};

}
