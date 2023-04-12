#include <nopayloadclient/payload.hpp>
#include <rsa/md5.hpp>

namespace nopayloadclient {

Payload::Payload(const string& local_url_, const string& type_){
    local_url = local_url_;
    type = type_;
    bare_file_name = getBareFileName();
    check_sum = getCheckSum();
    remote_dir = getRemoteDir();
    remote_url = getRemoteUrl();
}

Payload::Payload(const json& raw_response) {
    if (raw_response.type()==json::value_t::array) {
        type = raw_response[0];
        remote_url = raw_response[1];
        check_sum = raw_response[2];
    }
    else {
        type = raw_response["payload_type"];
        remote_url = raw_response["payload_iov"][0]["payload_url"];
        check_sum = raw_response["payload_iov"][0]["checksum"];
    }
}

string Payload::getCheckSum() {
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

string Payload::getRemoteUrl() {
  string local_remote_url = remote_dir;
    local_remote_url += "/" + check_sum + "_" + bare_file_name;
    while ( local_remote_url.find("//") != string::npos ) {
        local_remote_url.replace(local_remote_url.find("//"), 2, "/");
    }
    return local_remote_url;
}

string Payload::getDirsFromChecksum() {
    string first_dir = {check_sum[0], check_sum[1]};
    string second_dir = {check_sum[2], check_sum[3]};
    string dirs = first_dir + "/" + second_dir;
    return dirs;
}

string Payload::getRemoteDir() {
    string local_remote_dir = type;
    local_remote_dir += "/" + getDirsFromChecksum();
    while ( local_remote_dir.find("//") != string::npos ) {
        local_remote_dir.replace(local_remote_dir.find("//"), 2, "/");
    }
    return local_remote_dir;
}

string Payload::getBareFileName() {
    std::stringstream ss(local_url);
    string local_bare_file_name;
    while (!ss.eof()) {
        getline(ss, local_bare_file_name, '/');
    }
    return local_bare_file_name;
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

