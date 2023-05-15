#pragma once

#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

#include <nopayloadclient/curlwrapper.hpp>
#include <nopayloadclient/realwrapper.hpp>
#include <nopayloadclient/fakewrapper.hpp>
#include <nopayloadclient/cache.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/iov.hpp>
#include <nopayloadclient/moment.hpp>
#include <nopayloadclient/exception.hpp>


namespace nopayloadclient {

using nlohmann::json;
using std::string;

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
    json getPayloadLists(const string& global_tag);
    json getPayloadIOVs(const string& global_tag, Moment& mom);

    // Writing
    void createGlobalTagStatus(const string& name);
    void createGlobalTagObject(const string& name, const string& status);
    void createPayloadType(const string& name);
    void attachPayloadList(const string& global_tag, const string& pl_name);
    void lockGlobalTag(const string& name);
    void unlockGlobalTag(const string& name);
    void deleteGlobalTag(const string& name);
    void cloneGlobalTag(const string& source, const string& target);
    void attachPayloadIOV(const string& pll_name, long long piov_id);
    string createPayloadList(const string& type);
    long long createPayloadIOV(const Payload& pl, IOV& iov);

private:
    //CurlWrapper curlwrapper_;
    std::unique_ptr<CurlWrapper> curlwrapper_;
    Cache cache_;
    bool use_cache_;

    json get(const string& url);
    json del(const string& url, bool trash_cache=true);
    json put(const string& url, bool trash_cache=true);
    json put(const string& url, const json& data, bool trash_cache=true);
    json post(const string& url, const json& data, bool trash_cache=true);

    json get_piov_array(const json& raw_response);

};

}
