#include <nopayloadclient/iov.hpp>

namespace nopayloadclient {

IOV::IOV(Moment start) {
    start_ = start;
    is_open_ = true;
};

IOV::IOV(Moment start, Moment end) {
    if (end < start) {
        throw IOVException("End of IOV comes before start");
    }
    start_ = start;
    end_ = end;
    is_open_ = false;
};

bool IOV::contains(Moment& mom) {
    if (is_open_) {
        return !(mom < start_);
    }
    return !(mom < start_) && (mom < end_);
}

std::ostream& operator<< (std::ostream& os, const IOV& i) {
    os << "IOV(" << std::endl;
    os << "  start = " << i.start_;
    os << "  end = " << i.end_;
    os << ")" << std::endl;
    return os;
};

}
