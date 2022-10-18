#include <iostream>
#include <sys/stat.h>
#include <string>
#include <filesystem>
#include <unistd.h>

#include <config.hpp>

std::string basePath = config::rawDict()["remote_pl_dir"];

namespace fs = std::filesystem;
namespace plmover {

bool fileExists(std::string fileUrl){
    struct stat buffer;
    return (stat (fileUrl.c_str(), &buffer) ==0);
}

void checkLocalFile(std::string localUrl){
    std::cout<<"checkLocalFile(localUrl="<<localUrl<<")"<<std::endl;
    if (!fileExists(localUrl)){
        std::cout<<"Local payload file does not exist. Exiting..."<<std::endl;
        exit(1);
    }
}

void checkRemoteFile(std::string remoteUrl){
    if (fileExists(remoteUrl)){
        std::cout<<"Remote payload file already exists. Exiting..."<<std::endl;
        exit(1);
    }
}

std::string getRemoteUrl(std::string globalTag, std::string payloadType, int iovStart){
    std::string newPath = basePath;
    newPath += globalTag + "/";
    newPath += payloadType + "/";
    newPath += std::to_string(iovStart);
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

void uploadFile(std::string localUrl, std::string globalTag,
                std::string payloadType, int iovStart){
    std::cout<<"uploadFile(localUrl="<<localUrl<<")"<<std::endl;
    std::string remoteUrl = getRemoteUrl(globalTag, payloadType, iovStart);
    checkLocalFile(localUrl);
    checkRemoteFile(remoteUrl);
    prepareDirectories(globalTag, payloadType);
    std::filesystem::copy_file(localUrl, remoteUrl);
}

}