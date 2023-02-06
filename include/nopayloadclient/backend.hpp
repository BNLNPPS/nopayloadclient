#pragma once

#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

#include <nopayloadclient/config.hpp>
#include <nopayloadclient/curlwrapper.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/exception.hpp>

using json = nlohmann::json;


class Backend {
public:
    Backend(const json& config);

    // Reading
    json getGlobalTags();
    json getSize();
    json getGlobalTagStatuses();
    json getPayloadTypes();
    std::string checkConnection();
    std::vector<std::string> getPayloadUrls(std::string gt_name, std::string plType, long long major_iov, long long minor_iov);
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
    long long createPayloadIOV(payload::Payload& pl, long long major_iov, long long minor_iov);
    long long createPayloadIOV(payload::Payload& pl, long long major_iov, long long minor_iov,
                               long long major_iovEnd, long long minor_iovEnd);
    void prepareInsertIov(std::string gt_name, payload::Payload& pl);
    void insertIov(std::string gt_name, payload::Payload &pl,
                    long long major_iov_start, long long minor_iov_start);
    void insertIov(std::string gt_name, payload::Payload &pl,
                    long long major_iov_start, long long minor_iov_start,
                    long long major_iov_end, long long minor_iov_end);

private:
    // Member variables
    std::vector<std::string> read_dir_list_;
    CurlWrapper* curlwrapper_;
    bool use_cache_;
    json cache_dict_;

    // Reading
    std::string getPayloadUrl(std::string gt_name, std::string plType,
                              long long major_iov, long long minor_iov);
    std::string getPayloadListName(std::string gt_name, std::string plType);
    json getPayloadLists(std::string gt_name);
    json getPayloadIOVs(std::string gt_name, long long major_iov, long long minor_iov);
    bool gtExists(std::string gt_name);
    bool gtStatusExists(std::string name);
    bool plTypeExists(std::string plType);
    bool gtHasPlType(std::string gt_name, std::string plType);
    void checkStatusExists(std::string gtStatusName);
    void checkGtExists(std::string gt_name);
    void checkGtStatusExists(std::string name);
    void checkPlTypeExists(std::string name);

    // Writing
    void createNewPllForGt(std::string gt_name, std::string plType);
    void attachPayloadIOV(std::string plListName, long long plIovId);
    void attachPayloadList(std::string plName, std::string gt_name);
    std::string createPayloadList(std::string type);

    json get(std::string url);
    json del(std::string url, bool trash_cache=true);
    json put(std::string url, bool trash_cache=true);
    json put(std::string url, json data, bool trash_cache=true);
    json post(std::string url, json data, bool trash_cache=true);

    // Helper
    std::vector<std::string> getItemNames(json j);
};
