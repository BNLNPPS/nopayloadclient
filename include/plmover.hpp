#pragma once


namespace plmover {
    bool fileExists(std::string fileUrl);
    void checkLocalFile(std::string localUrl);
    void checkRemoteFile(std::string remoteUrl);
    std::string getRemoteUrl(std::string globalTag, std::string payloadType, int iovStart);
    void uploadFile(std::string localUrl, std::string globalTag,
                    std::string payloadType, int iovStart);
    void prepareDirectories(std::string globalTag, std::string payloadType);
    void prepareDirectory(std::string dirName);
}


