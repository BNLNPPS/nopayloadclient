#pragma once

#include <vector>
#include <nlohmann/json.hpp>

#include <payload.hpp>
#include <curlwrapper.hpp>

namespace backend {

    // Reading
    nlohmann::json getGlobalTags();
    nlohmann::json getSize();
    nlohmann::json getGlobalTagStatuses();
    nlohmann::json getPayloadTypes();
    nlohmann::json getPayloadLists();
    nlohmann::json getPayloadLists(std::string gtName);
    nlohmann::json getGlobalTagMap(std::string gtName);
    nlohmann::json getPayloadIOVs(std::string gtName, int majorIov, int minorIov);
    std::string getPayloadListName(std::string gtName, std::string plType);
    std::string getPayloadUrl(std::string gtName, std::string plType, int majorIov, int minorIov);
    bool gtExists(std::string gtName);
    bool plTypeExists(std::string plType);
    bool gtHasPlType(std::string gtName, std::string plType);
    void checkGtExists(std::string gtName);

    // Writing
    void createGlobalTagStatus(std::string status);
    void createGlobalTagObject(std::string name, std::string status);
    void createGlobalTag(std::string name);
    void createPayloadType(std::string type);
    std::string createPayloadList(std::string type);
    void attachPayloadList(std::string plName, std::string gtName);
    void unlockGlobalTag(std::string name);
    void lockGlobalTag(std::string name);
    void createNewPllForGt(std::string gtName, std::string plType);
    int createPayloadIOV(payload::Payload& pl, int majorIov, int minorIov);
    int createPayloadIOV(payload::Payload& pl, int majorIov, int minorIov, int majorIovEnd, int minorIovEnd);
    void attachPayloadIOV(std::string plListName, int plIovId);

    void prepareInsertIov(std::string gtName, payload::Payload& pl);
    void insertIov(std::string gtName, payload::Payload &pl,
                    int majorIovStart, int minorIovStart);

}
