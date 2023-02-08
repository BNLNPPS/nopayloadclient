#pragma once

#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

#include <nopayloadclient/config.hpp>
#include <nopayloadclient/curlwrapper.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/iov.hpp>
#include <nopayloadclient/exception.hpp>
#include <nopayloadclient/resthandler.hpp>

using json = nlohmann::json;
using ll = long long;


class Backend {
public:
    Backend() {};
    Backend(const json& config);

    // Reading
    json getSize();
    json getSuffixDict(npc::Moment& mom);
    json getUrlDict(npc::Moment& mom);
    std::string checkConnection();
    std::vector<std::string> getPayloadUrls(std::string pl_type,
                                            npc::Moment& mom);

    // Writing
    void createGlobalTag();
    void prepareInsertIov(payload::Payload& pl, npc::IOV& iov);
    void insertIov(payload::Payload& pl, npc::IOV& iov);

    // Configuring
    void setGlobalTag(std::string name);
    std::string getGlobalTag();

private:
    // Member variables
    std::vector<std::string> read_dir_list_;

    // Reading
    std::string getPayloadUrl(std::string pl_type, npc::Moment& mom);
    std::string getPayloadListName(std::string pl_type);
    bool gtExists();
    bool gtStatusExists(std::string name);
    bool plTypeExists(std::string pl_type);
    bool gtHasPlType(std::string pl_type);
    void checkGtExists();
    void checkGtStatusExists(std::string name);
    void checkPlTypeExists(std::string name);

    // Writing
    void createNewPll(std::string pl_type);
    void attachPayloadList(std::string pl_name);

    // Helper
    friend bool objWithNameExists(const json& j, std::string name);
};
