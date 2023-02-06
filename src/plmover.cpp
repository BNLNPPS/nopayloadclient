#include <nopayloadclient/plmover.hpp>

namespace fs = std::experimental::filesystem::v1;

PLMover::PLMover(const json& config) {
    write_dir_ = config["write_dir"];
    std::vector<std::string> read_dir_list_ = config["read_dir_list"];
}

bool PLMover::fileExists(std::string url){
    struct stat buffer;
    return (stat (url.c_str(), &buffer) ==0);
}

void PLMover::compareCheckSums(std::string firstFileUrl, std::string secondFileUrl){
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

void PLMover::checkLocalFileExists(std::string url){
    if (!fileExists(url)){
        std::string msg = "local payload file does not exist (";
        msg += url + ")";
        throw BaseException(msg);
    }
}

void PLMover::checkRemoteFile(std::string url){
    if (fileExists(url)){
        std::string msg = "remote payload file already exists (";
        msg += url + ")";
        throw BaseException(msg);
    }
}

void PLMover::checkRemoteDirExists() {
    if (!fs::exists(write_dir_)){
        throw BaseException("remote payload directory "+write_dir_+" does not exist");
    }
}

void PLMover::createDirectory(std::string path){
    if (!fs::is_directory(path) || !fs::exists(path)) {
        fs::create_directories(path);
    }
}

void PLMover::prepareUploadFile(payload::Payload& pl) {
    checkLocalFileExists(pl.local_url);
    //checkRemoteFile(pl.remote_url);
    checkRemoteDirExists();
}

void PLMover::copyFile(std::string local_url, std::string remote_url) {
    if (!fs::exists(remote_url)) {
        fs::copy_file(local_url, remote_url);
    }
}

void PLMover::uploadFile(payload::Payload& pl){
    createDirectory(write_dir_ + pl.remote_dir);
    copyFile(pl.local_url, write_dir_ + pl.remote_url);
}

