#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>

#include <nopayloadclient/exception.hpp>
#include <nopayloadclient/iov.hpp>
#include <nopayloadclient/payload.hpp>


namespace nopayloadclient {

using nlohmann::json;
using std::string;

struct PayloadIOV {
    PayloadIOV(const json& raw_response);
    Payload payload_;
    IOV iov_;
    friend std::ostream& operator<<(std::ostream& os, const PayloadIOV& piov);
};

}
