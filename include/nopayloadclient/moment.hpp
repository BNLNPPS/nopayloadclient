#pragma once

#include <nopayloadclient/exception.hpp>

using ll = long long;

namespace npc {

struct Moment {
    ll major;
    ll minor;
    Moment() : major(0), minor(0) {};
    Moment(ll ma, ll mi);
    bool operator< (const Moment& that);
    friend std::ostream& operator<< (std::ostream& os, const Moment& m);
};

}
