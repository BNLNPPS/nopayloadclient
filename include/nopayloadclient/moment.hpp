#pragma once

#include <nopayloadclient/exception.hpp>

using ll = long long;

namespace nopayloadclient {

struct Moment {
    ll major_;
    ll minor_;
    Moment() : major_(0), minor_(0) {};
    Moment(ll ma, ll mi);
    bool operator< (const Moment& that);
    friend std::ostream& operator<< (std::ostream& os, const Moment& m);
};

}
