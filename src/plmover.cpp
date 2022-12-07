#include <plmover.hpp>


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

void checkLocalFileExists(std::string localUrl){
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

void checkRemoteDirExists() {
    if (!fs::exists(config::write_dir)){
        throw NoPayloadException("remote payload directory "+config::write_dir+" does not exist");
    }
}

void createDirectory(std::string dirName){
    if (!fs::is_directory(dirName) || !fs::exists(dirName)) {
        fs::create_directories(dirName);
    }
}

void prepareUploadFile(payload::Payload& pl) {
    checkLocalFileExists(pl.local_url);
    //checkRemoteFile(pl.remote_url);
    checkRemoteDirExists();
}

void copyFile(std::string local_url, std::string remote_url) {
    if (!fs::exists(remote_url)) {
        std::filesystem::copy_file(local_url, remote_url);
    }
}

void uploadFile(payload::Payload& pl){
    createDirectory(config::write_dir + pl.remote_dir);
    copyFile(pl.local_url, config::write_dir + pl.remote_url);
}

}