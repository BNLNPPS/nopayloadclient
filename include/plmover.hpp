#pragma once


namespace plmover {

    void compareCheckSums(std::string firstFileUrl, std::string secondFileUrl);
    std::string getCheckSum(std::string fileUrl);
    std::string getCheckSumOld(std::string fileUrl);

    std::string getRemoteUrl(std::string globalTag, std::string payloadType,
                             int majorIovStart, int minorIovStart);

    void prepareUploadFile(std::string gtName, std::string plType, std::string fileUrl,
                           int majorIovStart, int minorIovStart);
    void uploadFile(std::string gtName, std::string plType, std::string fileUrl,
                    int majorIovStart, int minorIovStart);

}
