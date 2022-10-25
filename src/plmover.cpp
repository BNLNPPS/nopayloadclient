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
    std::ifstream inFile(fileUrl);
    std::string tempData;
    std::string inFileData;
    while (inFile.good()){
        std::getline(inFile, tempData);
        inFileData.append(tempData+"\n");
    }
    inFile.close();
    return md5(inFileData).c_str();
}

std::string getCheckSumOld(std::string fileUrl){
    // Weird bug: reading in a file with no newlines SOMETIMES
    // appends the file name or the its path to the buffer.
    // as it is randomly decided at runtime if this happens,
    // this method is not suited for check sum comparison
    std::ifstream inFile(fileUrl, std::ifstream::binary);
    inFile.seekg(0, inFile.end);
    long length = inFile.tellg();
    inFile.seekg(0, inFile.beg);
    char* inFileData = new char[length];
    inFile.read(inFileData, length);
    inFile.close();
    return md5(inFileData).c_str();
}

void compareCheckSums(std::string firstFileUrl, std::string secondFileUrl){
    std::string firstCheckSum = getCheckSum(firstFileUrl);
    std::string secondCheckSum = getCheckSum(secondFileUrl);
    std::cout<<"firstCheckSum = "<<firstCheckSum<<std::endl;
    std::cout<<"secondCheckSum = "<<secondCheckSum<<std::endl;
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

void createDirectory(std::string dirName){
    if (!fs::is_directory(dirName) || !fs::exists(dirName)) {
        fs::create_directory(dirName);
    }
}

void createDirectories(std::string globalTag, std::string payloadType) {
    createDirectory(basePath + "/" + globalTag);
    createDirectory(basePath + "/" + globalTag + "/" + payloadType);
}

void checkPLStores(std::string localUrl, std::string remoteUrl) {
    checkLocalFile(localUrl);
    checkRemoteFile(remoteUrl);
    if (!fs::exists(basePath)){
        throw NoPayloadException("remote payload directory "+basePath+" does not exist");
    }
}

void prepareUploadFile(std::string gtName, std::string plType, std::string fileUrl,
                       int majorIovStart, int minorIovStart) {
    std::string remoteUrl = getRemoteUrl(gtName, plType, majorIovStart, minorIovStart);
    checkPLStores(fileUrl, remoteUrl);
}

void uploadFile(std::string gtName, std::string plType, std::string fileUrl,
                int majorIovStart, int minorIovStart){
    createDirectories(gtName, plType);
    std::string remoteUrl = getRemoteUrl(gtName, plType, majorIovStart, minorIovStart);
    std::filesystem::copy_file(fileUrl, remoteUrl);
    compareCheckSums(fileUrl, remoteUrl);
}

}