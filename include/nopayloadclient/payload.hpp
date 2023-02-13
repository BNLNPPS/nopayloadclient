#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <nopayloadclient/exception.hpp>


namespace nopayloadclient {

class Payload {
public:
    Payload(std::string local_url_, std::string type_);
    std::string local_url;
    std::string type;
    std::string bare_file_name;
    std::string check_sum;
    std::string remote_dir;
    std::string remote_url;
    friend std::ostream& operator<<(std::ostream& os, const Payload& pl);
private:
    std::string getCheckSum();
    std::string getDirsFromChecksum();
    std::string getRemoteDir();
    std::string getRemoteUrl();
    std::string getBareFileName();
};

}
