#include <nopayloadclient/payload.hpp>
#include <rsa/md5.hpp>

namespace nopayloadclient {

Payload::Payload(std::string local_url_, std::string type_){
    local_url = local_url_;
    type = type_;
    bare_file_name = getBareFileName();
    check_sum = getCheckSum();
    remote_dir = getRemoteDir();
    remote_url = getRemoteUrl();
}

Payload::Payload(const json& raw_response) {
    type = raw_response["payload_type"];
    remote_url = raw_response["payload_iov"][0]["payload_url"];
    check_sum = raw_response["payload_iov"][0]["checksum"];
}

std::string Payload::getCheckSum() {
     FILE* filp = fopen(local_url.c_str(), "rb");
     if (!filp) {
        throw PayloadException("Could not open file " + local_url);
     }
     const int buffer_size = 4096;
     char buffer[buffer_size];
     MD5 md5 {};
     while (int bytes = fread(&buffer, sizeof(char), buffer_size, filp)) {
         md5.update(buffer, bytes);
     }
     fclose(filp);
     md5.finalize();
     return md5.hexdigest();
}

std::string Payload::getRemoteUrl() {
    std::string remote_url = remote_dir;
    remote_url += "/" + check_sum + "_" + bare_file_name;
    while ( remote_url.find("//") != std::string::npos ) {
        remote_url.replace(remote_url.find("//"), 2, "/");
    }
    return remote_url;
}

std::string Payload::getDirsFromChecksum() {
    std::string first_dir = {check_sum[0], check_sum[1]};
    std::string second_dir = {check_sum[2], check_sum[3]};
    std::string dirs = first_dir + "/" + second_dir;
    return dirs;
}

std::string Payload::getRemoteDir() {
    std::string remote_dir = type;
    remote_dir += "/" + getDirsFromChecksum();
    while ( remote_dir.find("//") != std::string::npos ) {
        remote_dir.replace(remote_dir.find("//"), 2, "/");
    }
    return remote_dir;
}

std::string Payload::getBareFileName() {
    std::stringstream ss(local_url);
    std::string bare_file_name;
    while (!ss.eof()) {
        getline(ss, bare_file_name, '/');
    }
    return bare_file_name;
}

std::ostream& operator<<(std::ostream& os, const Payload& pl) {
    os << "Payload instance with following attributes:" << std::endl;
    os << "local_url = " << pl.local_url << std::endl;
    os << "bare_file_name = " << pl.bare_file_name << std::endl;
    os << "type = " << pl.type << std::endl;
    os << "remote_dir = " << pl.remote_dir << std::endl;
    os << "remote_url = " << pl.remote_url << std::endl;
    os << "check_sum = " << pl.check_sum << std::endl;
    return os;
}

}

