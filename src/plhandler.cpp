#include <nopayloadclient/plhandler.hpp>

namespace fs = std::experimental::filesystem::v1;

namespace nopayloadclient {

PLHandler::PLHandler(const json& config) {
    write_dir_ = config["write_dir"];
    for (auto dir : config["read_dir_list"]) {
        read_dir_list_.push_back(dir);
    }
}

bool PLHandler::fileExists(const string& url) {
    struct stat buffer;
    return (stat (url.c_str(), &buffer) ==0);
}

string PLHandler::getFirstGoodUrl(Payload& pl) {
     for (const auto dir : read_dir_list_) {
         string full_url = dir + pl.remote_url;
         if (fileExists(full_url)) return full_url;
     }
     string text = "Could not find payload <" + pl.remote_url + "> ";
     text += "in any of the following read dirs:";
     for (const auto dir : read_dir_list_) {
         text += " " + dir;
     }
     throw BaseException(text);
}

void PLHandler::compareCheckSums(const string& firstFileUrl, const string& secondFileUrl){
    /*
    string firstCheckSum = getCheckSum(firstFileUrl);
    string secondCheckSum = getCheckSum(secondFileUrl);
    if (firstCheckSum != secondCheckSum){
        string msg = "checksums of the following two files differ: ";
        msg += firstFileUrl + ", ";
        msg += secondFileUrl;
        throw BaseException(msg);
    }
    */
}

void PLHandler::checkFileExists(const string& url){
    if (!fileExists(url)){
        string msg = "payload file does not exist (";
        msg += url + ")";
        throw BaseException(msg);
    }
}

void PLHandler::checkRemoteFile(const string& url){
    if (fileExists(url)){
        string msg = "remote payload file already exists (";
        msg += url + ")";
        throw BaseException(msg);
    }
}

void PLHandler::checkRemoteDirExists() {
    if (!fs::exists(write_dir_)){
        throw BaseException("remote payload directory "+write_dir_+" does not exist");
    }
}

void PLHandler::createDirectory(const string& path){
    if (!fs::is_directory(path) || !fs::exists(path)) {
        fs::create_directories(path);
    }
}

void PLHandler::prepareUploadFile(Payload& pl) {
    checkFileExists(pl.local_url);
    //checkRemoteFile(pl.remote_url);
    checkRemoteDirExists();
}

void PLHandler::copyFile(const string& local_url, const string& remote_url) {
    if (!fs::exists(remote_url)) {
        fs::copy_file(local_url, remote_url);
    }
}

void PLHandler::uploadFile(Payload& pl){
    createDirectory(write_dir_ + pl.remote_dir);
    copyFile(pl.local_url, write_dir_ + pl.remote_url);
}

}
