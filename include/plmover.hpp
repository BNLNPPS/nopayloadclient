#pragma once


namespace plmover {
    bool fileExists(std::string fileUrl);
    void checkLocalFile(std::string localUrl);
    void checkRemoteFile(std::string remoteUrl);
    std::string getRemoteUrl(std::string globalTag, std::string payloadType,
                             int majorIovStart, int minorIovStart);
    void uploadFile(std::string localUrl, std::string globalTag,
                    std::string payloadType, int majorIovStart, int minorIovStart);
    void prepareDirectories(std::string globalTag, std::string payloadType);
    void prepareDirectory(std::string dirName);
    std::string getCheckSum(std::string fileUrl);
}
