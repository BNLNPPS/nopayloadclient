#pragma once


namespace plmover {

    void compareCheckSums(std::string firstFileUrl, std::string secondFileUrl);
    std::string getCheckSum(std::string fileUrl);
    std::string getDirsFromCheckSum(std::string check_sum);

    std::string getRemoteUrl(std::string payloadType, std::string fileUrl);

    void prepareUploadFile(std::string payloadType, std::string fileUrl);

    void uploadFile(std::string payloadType, std::string fileUrl);

}
