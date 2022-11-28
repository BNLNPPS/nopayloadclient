#pragma once

#include <iostream>


namespace payload {
    class Payload {
    public:
        Payload(std::string local_url_, std::string payload_type_);
        std::string local_url;
        std::string payload_type;
        std::string check_sum;
        std::string remote_url;
        friend std::ostream& operator<<(std::ostream& os, const payload::Payload& pl);
    private:
        std::string getCheckSum();
        std::string getRemoteDirs();
        std::string getRemoteUrl();
        std::string getBareFileName();
    };
}


