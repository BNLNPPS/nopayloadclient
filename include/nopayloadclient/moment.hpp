#pragma once

#include <nopayloadclient/exception.hpp>


namespace nopayloadclient {

using ll = long long;

struct Moment {
    ll major_;
    ll minor_;
    Moment() : major_(0), minor_(0) {};
    Moment(ll ma, ll mi);
    bool operator< (const Moment& that);
    friend std::ostream& operator<< (std::ostream& os, const Moment& m);
};

}
