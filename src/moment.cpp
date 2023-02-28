#include <nopayloadclient/moment.hpp>

namespace nopayloadclient {

Moment::Moment(ll ma, ll mi) {
    major_ = ma;
    minor_ = mi;
};

bool Moment::operator< (const Moment& that){
    if (this->major_ != that.major_) {
        return (this->major_ < that.major_);
    }
    return (this->minor_ < that.minor_);
};

std::ostream& operator<< (std::ostream& os, const Moment& m) {
    os << "Moment(major=" << m.major_ << ", minor=" << m.minor_ << ")" << std::endl;
    return os;
};

}
