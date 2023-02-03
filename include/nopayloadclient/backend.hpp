#pragma once

#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

#include <nopayloadclient/config.hpp>
#include <nopayloadclient/curlwrapper.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/exception.hpp>


class Backend {
public:
    Backend(const nlohmann::json& config);
    // Reading
    nlohmann::json getGlobalTags();
    nlohmann::json getSize();
    nlohmann::json getGlobalTagStatuses();
    nlohmann::json getPayloadTypes();
    nlohmann::json _getPayloadLists(std::string gt_name);
    nlohmann::json getPayloadLists(std::string gt_name);
    nlohmann::json getPayloadIOVs(std::string gt_name, long long major_iov, long long minor_iov);
    std::string checkConnection();
    std::string getPayloadListName(std::string gt_name, std::string plType);
    std::vector<std::string> getPayloadUrls(std::string gt_name, std::string plType, long long major_iov, long long minor_iov);
    std::string getPayloadUrl(std::string gt_name, std::string plType, long long major_iov, long long minor_iov);
    std::vector<std::string> getGtStatusNames();
    std::vector<std::string> getGtNames();
    std::vector<std::string> getPtNames();
    bool gtExists(std::string gt_name);
    bool gtStatusExists(std::string name);
    bool plTypeExists(std::string plType);
    bool gtHasPlType(std::string gt_name, std::string plType);
    void checkStatusExists(std::string gtStatusName);
    void checkGtExists(std::string gt_name);
    void checkGtStatusExists(std::string name);
    void checkPlTypeExists(std::string name);

    // Writing
    void createGlobalTagStatus(std::string status);
    void createGlobalTagObject(std::string name, std::string status);
    void createGlobalTag(std::string name);
    void deleteGlobalTag(std::string name);
    void createPayloadType(std::string type);
    std::string createPayloadList(std::string type);
    void attachPayloadList(std::string plName, std::string gt_name);
    void unlockGlobalTag(std::string name);
    void lockGlobalTag(std::string name);
    void createNewPllForGt(std::string gt_name, std::string plType);
    long long createPayloadIOV(payload::Payload& pl, long long major_iov, long long minor_iov);
    long long createPayloadIOV(payload::Payload& pl, long long major_iov, long long minor_iov, long long major_iovEnd, long long minor_iovEnd);
    void attachPayloadIOV(std::string plListName, long long plIovId);
    void prepareInsertIov(std::string gt_name, payload::Payload& pl);
    void insertIov(std::string gt_name, payload::Payload &pl,
                    long long major_iovStart, long long minor_iovStart);
    void insertIov(std::string gt_name, payload::Payload &pl,
                    long long major_iovStart, long long minor_iovStart,
                    long long major_iovEnd, long long minor_iovEnd);

private:
    std::vector<std::string> read_dir_list_;
    CurlWrapper* curlwrapper_;
    bool use_cache_;
    //Cache* cache_;
    nlohmann::json cache_dict_;
    nlohmann::json get(std::string url);
    nlohmann::json del(std::string url, bool trash_cache=true);
    nlohmann::json put(std::string url, bool trash_cache=true);
    nlohmann::json put(std::string url, nlohmann::json data, bool trash_cache=true);
    nlohmann::json post(std::string url, nlohmann::json data, bool trash_cache=true);
};
