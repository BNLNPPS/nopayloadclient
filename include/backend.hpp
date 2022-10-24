#pragma once

#include <nlohmann/json.hpp>
#include <curlwrapper.hpp>

namespace backend {
    // Reading
    nlohmann::json getGlobalTags();
    nlohmann::json getGlobalTagStatuses();
    nlohmann::json getPayloadTypes();
    nlohmann::json getPayloadLists();
    nlohmann::json getPayloadLists(std::string gtName);
    nlohmann::json getGlobalTagMap(std::string gtName);
    nlohmann::json getPayloadIOVs(std::string gtName, int majorIov, int minorIov);
    std::string getPayloadListName(std::string gtName, std::string plType);
    bool gtExists(std::string gtName);
    bool plTypeExists(std::string plType);
    bool gtHasPlType(std::string gtName, std::string plType);
    void checkGtExists(std::string gtName);

    // Writing
    void createGlobalTagStatus(std::string status);
    void createGlobalTagObject(std::string name, std::string status);
    void createPayloadType(std::string type);
    std::string createPayloadList(std::string type);
    void attachPayloadList(std::string plName, std::string gtName);
    void unlockGlobalTag(std::string name);
    void lockGlobalTag(std::string name);
    int createPayloadIOV(std::string plUrl, int majorIov, int minorIov);
    int createPayloadIOV(std::string plUrl, int majorIov, int minorIov, int majorIovEnd, int minorIovEnd);
    void attachPayloadIOV(std::string plListName, int plIovId);

    void prepareInsertIov(std::string gtName, std::string plType, std::string fileUrl,
                           int majorIovStart, int minorIovStart);
    void insertIov(std::string gtName, std::string plType, std::string fileUrl,
                    int majorIovStart, int minorIovStart);

}
