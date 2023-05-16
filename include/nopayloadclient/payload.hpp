#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>

#include <nopayloadclient/exception.hpp>
#include <nopayloadclient/iov.hpp>


namespace nopayloadclient {

using nlohmann::json;
using std::string;

class Payload {
public:
    Payload() {};
    Payload(const string& local_url_, const string& type_);
    string local_url;
    string type;
    string bare_file_name;
    string check_sum;
    string remote_dir;
    string remote_url;
    string full_remote_url;
    //IOV iov {0, 0};
    friend std::ostream& operator<<(std::ostream& os, const Payload& pl);
private:
    string getCheckSum();
    string getDirsFromChecksum();
    string getRemoteDir();
    string getRemoteUrl();
    string getBareFileName();
};

}
