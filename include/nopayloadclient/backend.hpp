#pragma once

#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

#include <nopayloadclient/config.hpp>
#include <nopayloadclient/curlwrapper.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/iov.hpp>
#include <nopayloadclient/exception.hpp>

using json = nlohmann::json;
using ll = long long;


class Backend {
public:
    Backend() {};
    Backend(const json& config);

    // Reading
    json getGlobalTags();
    json getSize();
    json getGlobalTagStatuses();
    json getPayloadTypes();
    json getSuffixDict(npc::Moment& mom);
    json getUrlDict(npc::Moment& mom);
    std::string checkConnection();
    std::vector<std::string> getPayloadUrls(std::string pl_type,
                                            npc::Moment& mom);
    std::vector<std::string> getGtStatusNames();
    std::vector<std::string> getGtNames();
    std::vector<std::string> getPtNames();

    // Writing
    void createGlobalTagStatus(std::string status);
    void createGlobalTagObject(std::string status);
    void createGlobalTag();
    void deleteGlobalTag();
    void createPayloadType(std::string type);
    void unlockGlobalTag();
    void lockGlobalTag();
    void prepareInsertIov(payload::Payload& pl);
    void insertIov(payload::Payload& pl, npc::IOV& iov);
    ll createPayloadIOV(payload::Payload& pl, npc::IOV& iov);

    // Configuring
    void setGlobalTag(std::string name);
    std::string getGlobalTag();

private:
    // Member variables
    std::vector<std::string> read_dir_list_;
    CurlWrapper curlwrapper_;
    std::string global_tag_;
    bool use_cache_;
    json cache_dict_;

    // Reading
    std::string getPayloadUrl(std::string pl_type, npc::Moment& mom);
    std::string getPayloadListName(std::string pl_type);
    json getPayloadLists();
    json getPayloadIOVs(npc::Moment& mom);
    bool gtExists();
    bool gtStatusExists(std::string name);
    bool plTypeExists(std::string pl_type);
    bool gtHasPlType(std::string pl_type);
    void checkStatusExists(std::string name);
    void checkGtExists();
    void checkGtStatusExists(std::string name);
    void checkPlTypeExists(std::string name);

    // Writing
    void createNewPll(std::string pl_type);
    void attachPayloadIOV(std::string pll_name, ll piov_id);
    void attachPayloadList(std::string pl_name);
    std::string createPayloadList(std::string type);

    json get(std::string url);
    json del(std::string url, bool trash_cache=true);
    json put(std::string url, bool trash_cache=true);
    json put(std::string url, json data, bool trash_cache=true);
    json post(std::string url, json data, bool trash_cache=true);

    // Helper
    std::vector<std::string> getItemNames(const json& j);
};
