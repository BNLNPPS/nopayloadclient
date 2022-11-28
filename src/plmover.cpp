#include <iostream>
#include <sys/stat.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <unistd.h>


#include <plmover.hpp>
#include <payload.hpp>
#include <exception.hpp>
#include <config.hpp>


namespace fs = std::filesystem;
namespace plmover {

bool fileExists(std::string fileUrl){
    struct stat buffer;
    return (stat (fileUrl.c_str(), &buffer) ==0);
}


void compareCheckSums(std::string firstFileUrl, std::string secondFileUrl){
    /*
    std::string firstCheckSum = getCheckSum(firstFileUrl);
    std::string secondCheckSum = getCheckSum(secondFileUrl);
    if (firstCheckSum != secondCheckSum){
        std::string msg = "checksums of the following two files differ: ";
        msg += firstFileUrl + ", ";
        msg += secondFileUrl;
        throw NoPayloadException(msg);
    }
    */
}

void checkLocalFile(std::string localUrl){
    if (!fileExists(localUrl)){
        std::string msg = "local payload file does not exist (";
        msg += localUrl + ")";
        throw NoPayloadException(msg);
    }
}

void checkRemoteFile(std::string remoteUrl){
    if (fileExists(remoteUrl)){
        std::string msg = "remote payload file already exists (";
        msg += remoteUrl + ")";
        throw NoPayloadException(msg);
    }
}

std::string getRemoteUrl(std::string payloadType, std::string check_sum){
    std::string remote_url = config::remote_pl_dir;
    remote_url += getDirsFromCheckSum(check_sum);
    return remote_url;
}

std::string getDirsFromCheckSum(std::string check_sum){
    std::string first_dir = {check_sum[0], check_sum[1]};
    std::string second_dir = {check_sum[2], check_sum[3]};
    std::string dirs = first_dir + "/" + second_dir;
    return dirs;
}

void createDirectory(std::string dirName){
    if (!fs::is_directory(dirName) || !fs::exists(dirName)) {
        fs::create_directory(dirName);
    }
}

void createDirectories(std::string globalTag, std::string payloadType) {
    createDirectory(config::remote_pl_dir + "/" + globalTag);
    createDirectory(config::remote_pl_dir + "/" + globalTag + "/" + payloadType);
}

void createRemoteDir(std::string check_sum) {
    std::string remote_dir = getDirsFromCheckSum(check_sum);
    createDirectory(remote_dir);
}

void checkPLStores(std::string localUrl, std::string remoteUrl) {
    checkLocalFile(localUrl);
    checkRemoteFile(remoteUrl);
    if (!fs::exists(config::remote_pl_dir)){
        throw NoPayloadException("remote payload directory "+config::remote_pl_dir+" does not exist");
    }
}

void prepareUploadFile(std::string plType, std::string fileUrl) {
    //std::string check_sum = getCheckSum(fileUrl);
    std::string check_sum = "checksum";
    std::string remoteUrl = getRemoteUrl(plType, check_sum);
    checkPLStores(fileUrl, remoteUrl);
}

//void uploadFile(std::string gtName, std::string plType, std::string fileUrl,
//                int majorIovStart, int minorIovStart){
void uploadFile(std::string plType, std::string fileUrl){
    createRemoteDir("checksum");
    std::string check_sum = "";
    std::string remoteUrl = getRemoteUrl(plType, check_sum);
    std::filesystem::copy_file(fileUrl, remoteUrl);
    compareCheckSums(fileUrl, remoteUrl);
}

}