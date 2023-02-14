#include <nopayloadclient/plmover.hpp>

namespace fs = std::experimental::filesystem::v1;
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
        throw BaseException(msg);
    }
    */
}

void checkLocalFileExists(std::string localUrl){
    if (!fileExists(localUrl)){
        std::string msg = "local payload file does not exist (";
        msg += localUrl + ")";
        throw BaseException(msg);
    }
}

void checkRemoteFile(std::string remoteUrl){
    if (fileExists(remoteUrl)){
        std::string msg = "remote payload file already exists (";
        msg += remoteUrl + ")";
        throw BaseException(msg);
    }
}

void checkRemoteDirExists() {
    if (!fs::exists(config::write_dir)){
        throw BaseException("remote payload directory "+config::write_dir+" does not exist");
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
        fs::copy_file(local_url, remote_url);
    }
}

void uploadFile(payload::Payload& pl){
    createDirectory(config::write_dir + pl.remote_dir);
    copyFile(pl.local_url, config::write_dir + pl.remote_url);
}

std::string getFirstGoodUrl(std::string payload_suffix) {
    for (const auto dir : config::read_dir_list) {
        std::string full_url = dir + payload_suffix;
        if (fileExists(full_url)) return full_url;
    }
    std::string text = "Could not find payload <" + payload_suffix + "> ";
    text += "in any of the following read dirs:";
    for (const auto dir : config::read_dir_list) {
        text += " " + dir;
    }
    throw BaseException(text);
}

}
