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

bool IOV::dict_has_ending(const json& iov_dict) {
    if (iov_dict.contains("major_start") && iov_dict.contains("minor_start")) {
        if (iov_dict.size() == 2) return false;
        if (iov_dict.contains("major_end") && iov_dict.contains("minor_end")) {
            if (iov_dict.size() == 4) return true;
        }
    }
    throw IOVException("Invalid IOV dict");
}

std::ostream& operator<< (std::ostream& os, const IOV& i) {
    os << "IOV(" << std::endl;
    os << "  start = " << i.start_;
    os << "  end = " << i.end_;
    os << "  is_open = " << i.is_open_ << std::endl;
    os << ")" << std::endl;
    return os;
};

}
