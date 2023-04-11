#pragma once

#include <nopayloadclient/moment.hpp>
#include <nopayloadclient/exception.hpp>


namespace nopayloadclient {

using ll = long long;

struct IOV {
    Moment start_;
    Moment end_;
    bool is_open_;
    IOV(Moment start);
    IOV(Moment start, Moment end);
    IOV(ll major_start, ll minor_start) :
        IOV(Moment(major_start, minor_start)) {};
    IOV(ll major_start, ll minor_start, ll major_end, ll minor_end) :
        IOV(Moment(major_start, minor_start), Moment(major_end, minor_end)) {};
    bool contains(Moment& mom);
    friend std::ostream& operator<< (std::ostream& os, const IOV& i);
};

}
