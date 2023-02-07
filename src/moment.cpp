#include <nopayloadclient/moment.hpp>

using ll = long long;

namespace npc {

Moment::Moment(ll ma, ll mi) {
    major = ma;
    minor = mi;
};

bool Moment::operator< (const Moment& that){
    if (this->major != that.major) {
        return (this->major < that.major);
    }
    return (this->minor < that.minor);
};

std::ostream& operator<< (std::ostream& os, const npc::Moment& m) {
    os << "Moment(major=" << m.major << ", minor=" << m.minor << ")" << std::endl;
    return os;
};

}
