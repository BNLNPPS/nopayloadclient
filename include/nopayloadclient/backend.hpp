#pragma once

#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

#include <nopayloadclient/config.hpp>
#include <nopayloadclient/curlwrapper.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/exception.hpp>

using json = nlohmann::json;
using ll = long long;


class Backend {
public:
    Backend(const json& config);

    // Reading
    json getGlobalTags();
    json getSize();
    json getGlobalTagStatuses();
    json getPayloadTypes();
    json getTypeUrlDict(std::string gt_name, ll major_iov, ll minor_iov);

    std::string checkConnection();
    std::vector<std::string> getPayloadUrls(std::string gt_name, std::string pl_type,
                                            ll major_iov, ll minor_iov);
    std::vector<std::string> getGtStatusNames();
    std::vector<std::string> getGtNames();
    std::vector<std::string> getPtNames();

    // Writing
    void createGlobalTagStatus(std::string status);
    void createGlobalTagObject(std::string name, std::string status);
    void createGlobalTag(std::string name);
    void deleteGlobalTag(std::string name);
    void createPayloadType(std::string type);
    void unlockGlobalTag(std::string name);
    void lockGlobalTag(std::string name);
    ll createPayloadIOV(payload::Payload& pl,
                        ll major_iov, ll minor_iov,
                        ll major_iov_end=-1, ll minor_iov_end=-1);
    void prepareInsertIov(std::string gt_name, payload::Payload& pl);
    void insertIov(std::string gt_name, payload::Payload &pl,
                    ll major_iov_start, ll minor_iov_start,
                    ll major_iov_end, ll minor_iov_end);

private:
    // Member variables
    std::vector<std::string> read_dir_list_;
    CurlWrapper* curlwrapper_;
    bool use_cache_;
    json cache_dict_;

    // Reading
    std::string getPayloadUrl(std::string gt_name, std::string pl_type,
                              ll major_iov, ll minor_iov);
    std::string getPayloadListName(std::string gt_name, std::string pl_type);
    json getPayloadLists(std::string gt_name);
    json getPayloadIOVs(std::string gt_name, ll major_iov, ll minor_iov);
    bool gtExists(std::string gt_name);
    bool gtStatusExists(std::string name);
    bool pl_typeExists(std::string pl_type);
    bool gtHasPlType(std::string gt_name, std::string pl_type);
    void checkStatusExists(std::string name);
    void checkGtExists(std::string name);
    void checkGtStatusExists(std::string name);
    void checkPlTypeExists(std::string name);

    // Writing
    void createNewPllForGt(std::string gt_name, std::string pl_type);
    void attachPayloadIOV(std::string plListName, ll plIovId);
    void attachPayloadList(std::string plName, std::string gt_name);
    std::string createPayloadList(std::string type);

    json get(std::string url);
    json del(std::string url, bool trash_cache=true);
    json put(std::string url, bool trash_cache=true);
    json put(std::string url, json data, bool trash_cache=true);
    json post(std::string url, json data, bool trash_cache=true);

    // Helper
    std::vector<std::string> getItemNames(const json& j);
};
