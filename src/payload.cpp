#include <iostream>
#include <sys/stat.h>
#include <string>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <md5.hpp>

#include <payload.hpp>
#include <exception.hpp>
#include <config.hpp>

namespace payload{

Payload::Payload(std::string local_url_, std::string payload_type_){
    std::cout << " this be da constructor " << std::endl;
    local_url = local_url_;
    payload_type = payload_type_;
    check_sum = getCheckSum();
    remote_url = getRemoteUrl();
}

std::string Payload::getCheckSum(){
    std::ifstream inFile(local_url, std::ifstream::binary);
    inFile.seekg(0, inFile.end);
    long length = inFile.tellg();
    inFile.seekg(0, inFile.beg);
    char inFileData[length+1];
    inFile.read(inFileData, length);
    inFile.close();
    return md5(inFileData).c_str();
}

std::string Payload::getRemoteUrl() {
    std::string remote_url = config::remote_pl_dir;
    remote_url += "/" + payload_type;
    remote_url += "/" + getRemoteDirs();
    remote_url += "/" + getBareFileName();
    return remote_url;
}

std::string Payload::getRemoteDirs(){
    std::string first_dir = {check_sum[0], check_sum[1]};
    std::string second_dir = {check_sum[2], check_sum[3]};
    std::string dirs = first_dir + "/" + second_dir;
    return dirs;
}

std::string Payload::getBareFileName() {
    std::stringstream ss(local_url);
    std::string bare_file_name;
    while (!ss.eof()) {
        getline(ss, bare_file_name, '/');
    }
    return bare_file_name;
}

std::ostream& operator<<(std::ostream& os, const payload::Payload& pl) {
    os << "Payload instance with following attributes:" << std::endl;
    os << "local_url = " << pl.local_url << std::endl;
    os << "payload_type = " << pl.payload_type << std::endl;
    os << "remote_url = " << pl.remote_url << std::endl;
    os << "check_sum = " << pl.check_sum << std::endl;
    return os;
}

}

