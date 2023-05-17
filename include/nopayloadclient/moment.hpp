#pragma once

#include <nopayloadclient/exception.hpp>


namespace nopayloadclient {

using ll = long long;

struct Moment {
    ll major_;
    ll minor_;
    Moment() : major_(-1), minor_(-1) {};
    Moment(ll ma, ll mi);
    bool operator< (const Moment& that);
    friend std::ostream& operator<< (std::ostream& os, const Moment& m);
};

}
