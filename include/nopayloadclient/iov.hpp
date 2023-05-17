#pragma once

#include <nlohmann/json.hpp>

#include <nopayloadclient/moment.hpp>
#include <nopayloadclient/exception.hpp>


namespace nopayloadclient {

using ll = long long;
using nlohmann::json;

struct IOV {
    Moment start_;
    Moment end_;
    bool is_open_;
    IOV() {};
    IOV(Moment start);
    IOV(Moment start, Moment end);
    IOV(ll major_start, ll minor_start) :
        IOV(Moment(major_start, minor_start)) {};
    IOV(ll major_start, ll minor_start, ll major_end, ll minor_end) :
        IOV(Moment(major_start, minor_start), Moment(major_end, minor_end)) {};
    bool dict_has_ending(const json& iov_dict);
    IOV(const json& iov_dict) :
        IOV(dict_has_ending(iov_dict) ?
         IOV(iov_dict["major_start"], iov_dict["minor_start"], iov_dict["major_end"], iov_dict["minor_end"]) :
          IOV(iov_dict["major_start"], iov_dict["minor_start"])) {};
    bool contains(Moment& mom);
    friend std::ostream& operator<< (std::ostream& os, const IOV& i);
};

}
