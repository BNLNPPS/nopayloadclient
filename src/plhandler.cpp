#include <nopayloadclient/plhandler.hpp>

namespace fs = std::experimental::filesystem::v1;

namespace nopayloadclient {

PLHandler::PLHandler(const json& config) {
    write_dir_ = config["write_dir"];
    for (auto dir : config["read_dir_list"]) {
        read_dir_list_.push_back(dir);
    }
}

bool PLHandler::fileExists(std::string url) {
    struct stat buffer;
    return (stat (url.c_str(), &buffer) ==0);
}

std::string PLHandler::getFirstGoodUrl(Payload& pl) {
     for (const auto dir : read_dir_list_) {
         std::string full_url = dir + pl.remote_url;
         if (fileExists(full_url)) return full_url;
     }
     std::string text = "Could not find payload <" + pl.remote_url + "> ";
     text += "in any of the following read dirs:";
     for (const auto dir : read_dir_list_) {
         text += " " + dir;
     }
     throw BaseException(text);
}

void PLHandler::compareCheckSums(std::string firstFileUrl, std::string secondFileUrl){
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

void PLHandler::checkFileExists(std::string url){
    if (!fileExists(url)){
        std::string msg = "payload file does not exist (";
        msg += url + ")";
        throw BaseException(msg);
    }
}

void PLHandler::checkRemoteFile(std::string url){
    if (fileExists(url)){
        std::string msg = "remote payload file already exists (";
        msg += url + ")";
        throw BaseException(msg);
    }
}

void PLHandler::checkRemoteDirExists() {
    if (!fs::exists(write_dir_)){
        throw BaseException("remote payload directory "+write_dir_+" does not exist");
    }
}

void PLHandler::createDirectory(std::string path){
    if (!fs::is_directory(path) || !fs::exists(path)) {
        fs::create_directories(path);
    }
}

void PLHandler::prepareUploadFile(Payload& pl) {
    checkFileExists(pl.local_url);
    //checkRemoteFile(pl.remote_url);
    checkRemoteDirExists();
}

void PLHandler::copyFile(std::string local_url, std::string remote_url) {
    if (!fs::exists(remote_url)) {
        fs::copy_file(local_url, remote_url);
    }
}

void PLHandler::uploadFile(Payload& pl){
    createDirectory(write_dir_ + pl.remote_dir);
    copyFile(pl.local_url, write_dir_ + pl.remote_url);
}

}
