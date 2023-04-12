#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>

#include <nopayloadclient/exception.hpp>


namespace nopayloadclient {

using nlohmann::json;
using std::string;

class Payload {
public:
    Payload(const string& local_url_, const string& type_);
    Payload(const json& raw_response);
    string local_url;
    string type;
    string bare_file_name;
    string check_sum;
    string remote_dir;
    string remote_url;
    friend std::ostream& operator<<(std::ostream& os, const Payload& pl);
private:
    string getCheckSum();
    string getDirsFromChecksum();
    string getRemoteDir();
    string getRemoteUrl();
    string getBareFileName();
};

}
