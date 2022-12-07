#pragma once

#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

#include <config.hpp>
#include <curlwrapper.hpp>
#include <payload.hpp>
#include <exception.hpp>


namespace backend {

    template <typename T> class Cache {
        public:
            bool is_valid = false;
            T content;
            void update(T new_content){
              content = new_content;
              is_valid = true;
            }
    };

    // Reading
    nlohmann::json getGlobalTags();
    nlohmann::json getSize();
    nlohmann::json getGlobalTagStatuses();
    nlohmann::json getPayloadTypes();
    nlohmann::json getPayloadLists();
    nlohmann::json getPayloadLists(std::string gt_name);
    nlohmann::json getGlobalTagMap(std::string gt_name);
    nlohmann::json getPayloadIOVs(std::string gt_name, long long major_iov, long long minor_iov);
    std::string getPayloadListName(std::string gt_name, std::string plType);
    std::vector<std::string> getPayloadUrls(std::string gt_name, std::string plType, long long major_iov, long long minor_iov);
    bool gtExists(std::string gt_name);
    bool plTypeExists(std::string plType);
    bool gtHasPlType(std::string gt_name, std::string plType);
    void checkGtExists(std::string gt_name);

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

}
