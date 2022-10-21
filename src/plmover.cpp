#include <iostream>
#include <sys/stat.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <md5.hpp>

#include <plmover.hpp>
#include <exception.hpp>
#include <config.hpp>

std::string basePath = config::rawDict()["remote_pl_dir"];

namespace fs = std::filesystem;
namespace plmover {

bool fileExists(std::string fileUrl){
    struct stat buffer;
    return (stat (fileUrl.c_str(), &buffer) ==0);
}

std::string getCheckSum(std::string fileUrl){
    std::ifstream inFile;
    inFile.open(fileUrl, std::ifstream::binary);
    inFile.seekg(0, std::ios::end);
    long length = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    char* inFileData = new char[length];
    inFile.read(inFileData, length);
    return md5(inFileData).c_str();
}

void compareCheckSums(std::string firstFileUrl, std::string secondFileUrl){
    std::string firstCheckSum = getCheckSum(firstFileUrl);
    std::string secondCheckSum = getCheckSum(secondFileUrl);
    if (firstCheckSum != secondCheckSum){
        std::string msg = "checksums of the following two files differ: ";
        msg += firstFileUrl + ", ";
        msg += secondFileUrl;
        throw NoPayloadException(msg);
    }
}

void checkLocalFile(std::string localUrl){
    std::cout<<"checkLocalFile(localUrl="<<localUrl<<")"<<std::endl;
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

std::string getRemoteUrl(std::string globalTag, std::string payloadType,
                         int majorIovStart, int minorIovStart){
    std::string newPath = basePath;
    newPath += globalTag + "/";
    newPath += payloadType + "/";
    newPath += std::to_string(majorIovStart) + "_";
    newPath += std::to_string(minorIovStart);
    newPath += ".dat";
    return newPath;
}

void prepareDirectory(std::string dirName){
    if (!fs::is_directory(dirName) || !fs::exists(dirName)) {
        fs::create_directory(dirName);
    }
}

void prepareDirectories(std::string globalTag, std::string payloadType){
    prepareDirectory(basePath + "/" + globalTag);
    prepareDirectory(basePath + "/" + globalTag + "/" + payloadType);
}

void uploadFile(std::string localUrl, std::string globalTag, std::string payloadType,
                int majorIovStart, int minorIovStart){
    std::cout<<"uploadFile(localUrl="<<localUrl<<")"<<std::endl;
    std::string remoteUrl = getRemoteUrl(globalTag, payloadType, majorIovStart, minorIovStart);
    checkLocalFile(localUrl);
    checkRemoteFile(remoteUrl);
    prepareDirectories(globalTag, payloadType);
    std::filesystem::copy_file(localUrl, remoteUrl);
    compareCheckSums(localUrl, remoteUrl);
}

}