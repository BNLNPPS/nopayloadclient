#pragma once

#include <nlohmann/json.hpp>
#include <curlwrapper.hpp>

namespace backend {
    // Reading
    nlohmann::json getGlobalTags();
    nlohmann::json getGlobalTagTypes();
    nlohmann::json getGlobalTagStatuses();
    nlohmann::json getPayloadTypes();
    nlohmann::json getPayloadLists();


    // Writing
    void createGlobalTagType(std::string type);
    void createGlobalTagStatus(std::string status);
    void createGlobalTagObject(std::string name, std::string status, std::string type);
    void createPayloadType(std::string type);
    std::string createPayloadList(std::string type);
    void attachPayloadList(std::string plName, std::string gtName);
    void unlockGlobalTag(std::string name);
    void lockGlobalTag(std::string name);
    int createPayloadIOV(std::string plUrl, int majorIov, int minorIov);
    int createPayloadIOV(std::string plUrl, int majorIov, int minorIov, int majorIovEnd, int minorIovEnd);
    void attachPayloadIOV(std::string plListName, int plIovId);
}
