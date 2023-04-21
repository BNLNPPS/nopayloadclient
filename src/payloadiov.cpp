#include <nopayloadclient/payloadiov.hpp>

namespace nopayloadclient {


PayloadIOV::PayloadIOV(const json& resp) {
    std::cout << "PayloadIOV::PayloadIOV()" << std::endl;
    if (resp.type()==json::value_t::array) {
        std::cout << "PayloadIOV::fromArrayResp()" << std::endl;
        payload_.type = resp[0];
        payload_.remote_url = resp[1];
        payload_.check_sum = resp[2];
        iov_ = IOV(resp[3], resp[4], resp[5], resp[6]);
    }
    else {
        std::cout << "PayloadIOV::fromJsonResp()" << std::endl;
        payload_.type = resp["payload_type"];
        payload_.remote_url = resp["payload_iov"][0]["payload_url"];
        payload_.check_sum = resp["payload_iov"][0]["checksum"];
        iov_ = IOV(resp["payload_iov"][0]["major_iov_start"],
                  resp["payload_iov"][0]["minor_iov_start"],
                  resp["payload_iov"][0]["major_iov_end"],
                  resp["payload_iov"][0]["minor_iov_end"]);
    }
}


std::ostream& operator<<(std::ostream& os, const PayloadIOV& piov) {
    os << "PayloadIOV instance with following attributes:" << std::endl;
    os << "payload = " << piov.payload_ << std::endl;
    os << "iov = " << piov.iov_ << std::endl;
    return os;
}


}
