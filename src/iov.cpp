#include <nopayloadclient/iov.hpp>

namespace npc {

IOV::IOV(ll major_start, ll minor_start) {
        start = Moment(major_start, minor_start);
        is_open = true;
};

IOV::IOV(ll major_start, ll minor_start, ll major_end, ll minor_end) {
    start = Moment(major_start, minor_start);
    end = Moment(major_end, minor_end);
    if (end < start) {
        throw IOVException("End of IOV comes before start");
    }
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
