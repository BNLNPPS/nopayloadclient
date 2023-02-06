#pragma once

#include <iostream>
#include <sys/stat.h>
#include <string>
#include <experimental/filesystem>
#include <fstream>
#include <unistd.h>

#include <nopayloadclient/exception.hpp>
#include <nopayloadclient/config.hpp>
#include <nopayloadclient/payload.hpp>

using json = nlohmann::json;

class PLHandler {
public:
    PLHandler(const json& config);
    void compareCheckSums(std::string first_url, std::string second_url);
    void prepareUploadFile(payload::Payload& pl);
    void uploadFile(payload::Payload& pl);
private:
    std::string write_dir_;
    std::vector<std::string> read_dir_list_;
    bool fileExists(std::string url);
    void checkLocalFileExists(std::string url);
    void checkRemoteFile(std::string url);
    void checkRemoteDirExists();
    void createDirectory(std::string path);
    void copyFile(std::string local_url, std::string remote_url);

};