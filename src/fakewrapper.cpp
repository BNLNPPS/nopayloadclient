#include <nopayloadclient/fakewrapper.hpp>

namespace nopayloadclient {

json FakeWrapper::get(const string& url) {
    string end_point = splitString(url, '/')[0];
    if (end_point == "globalTags") {
        return FAKE_DATABASE["globalTags"];
    }
    if (end_point == "payloadiovs") {
        long long major_iov = std::stoi(splitString(splitString(url, '=')[2], '?')[0]);
        long long minor_iov = std::stoi(splitString(url, '=')[3]);
        Moment req_mom {major_iov, minor_iov};
        if (!FAKE_IOV.contains(req_mom)) {
            return json {};
        }
        return FAKE_DATABASE["payloadiovs"];
    }
    if (!FAKE_DATABASE.contains(end_point)) {
        throw BaseException("end point " + end_point + " not implemented in fake backend");
    }

    return FAKE_DATABASE[end_point];
}

std::vector<string> FakeWrapper::splitString(const string& input, const char splitter) {
    std::stringstream stream(input);
    string segment;
    std::vector<string> seg_list;
    while(std::getline(stream, segment, splitter)) {
       seg_list.push_back(segment);
    }
    return seg_list;
}

}
