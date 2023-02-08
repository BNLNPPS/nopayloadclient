#include <nopayloadclient/iov.hpp>

namespace npc {

IOV::IOV(npc::Moment start_) {
    start = start_;
    is_open = true;
};

IOV::IOV(npc::Moment start_, npc::Moment end_) {
    if (end_ < start_) {
        throw IOVException("End of IOV comes before start");
    }
    start = start_;
    end = end_;
    is_open = false;
};

std::ostream& operator<< (std::ostream& os, const npc::IOV& i) {
    os << "IOV(" << std::endl;
    os << "  start = " << i.start;
    os << "  end = " << i.end;
    os << ")" << std::endl;
    return os;
};

}
