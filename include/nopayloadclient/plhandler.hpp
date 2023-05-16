#pragma once

#include <iostream>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include <fstream>
#include <unistd.h>

#include <nopayloadclient/exception.hpp>
#include <nopayloadclient/config.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/payloadiov.hpp>


namespace nopayloadclient {

using nlohmann::json;
using std::string;

class PLHandler {
public:
    PLHandler() {};
    PLHandler(const json& config);
    //void compareCheckSums(const string& first_url, const string& second_url);
    void prepareUploadFile(const Payload& pl);
    void uploadFile(const Payload& pl);
    string getFirstGoodUrl(const Payload& pl);
    string getFirstGoodUrl(const string& remote_url);
    void decoratePrefixes(json& payload_iovs);
    void checkFileExists(const string& url);
private:
    string write_dir_;
    std::vector<string> read_dir_list_;
    bool fileExists(const string& url);
    void checkRemoteFile(const string& url);
    void checkRemoteDirExists();
    void createDirectory(const string& path);
    void copyFile(const string& local_url, const string& remote_url);
};

}
