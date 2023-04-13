#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <nlohmann/json.hpp>

#include <nopayloadclient/exception.hpp>
#include <nopayloadclient/curlwrapper.hpp>
#include <nopayloadclient/iov.hpp>


namespace nopayloadclient {

using nlohmann::json;
using std::string;

class FakeWrapper : public CurlWrapper {

public:
    FakeWrapper() : CurlWrapper() {};
    FakeWrapper(const json& config) : CurlWrapper(config) {};

    json get(const string& url);

    json del(const string& /* url */) {
        throw BaseException("no writing implemented in fake backend");
    };
    json put(const string& /* url */) {
        throw BaseException("no writing implemented in fake backend");
    };
    json put(const string& /* url */, const json& /* data */) {
        throw BaseException("no writing implemented in fake backend");
    };
    json post(const string& /* url */, const json& /* data */) {
        throw BaseException("no writing implemented in fake backend");
    };


private:
    std::vector<string> splitString(const string& input, const char splitter);
    IOV FAKE_IOV {42, 42};
    json FAKE_GLOBAL_TAGS = json{{
                   {"id", 162},
                   {"name", "ExampleGT"},
                   {"status", "unlocked"},
                   {"payload_lists_count", 2},
                   {"payload_iov_count", 3}
                  }};

    json FAKE_PAYLOAD_IOVS = json {
    {
        {"id", 154},
        {"name", "ExamplePT1_154"},
        {"global_tag", "ExampleGT"},
        {"payload_type", "ExamplePT1"},
        {"payload_iov", {{{"id", 589},
                         {"payload_url", "ExamplePT1/8d/77/8d777f385d3dfec8815d20f7496026dc_example_file.dat"},
                         {"checksum", "8d777f385d3dfec8815d20f7496026dc"},
                         {"major_iov", FAKE_IOV.start_.major_},
                         {"minor_iov", FAKE_IOV.start_.minor_},
                         {"major_iov_end", 9223372036854775807},
                         {"minor_iov_end", 9223372036854775807},
                         {"payload_list", "ExamplePT1_154"}
                        }}
         }
    },
    {
        {"id", 157},
        {"name", "ExamplePT2_157"},
        {"global_tag", "ExampleGT"},
        {"payload_type", "ExamplePT2"},
        {"payload_iov", {{{"id", 611},
                         {"payload_url", "ExamplePT2/8d/77/8d777f385d3dfec8815d20f7496026dc_example_file.dat"},
                         {"checksum", "8d777f385d3dfec8815d20f7496026dc"},
                         {"major_iov", FAKE_IOV.start_.major_},
                         {"minor_iov", FAKE_IOV.start_.minor_},
                         {"major_iov_end", 9223372036854775807},
                         {"minor_iov_end", 9223372036854775807},
                         {"payload_list", "ExamplePT2_157"}
                        }}
        }
    }
    };

    json FAKE_DATABASE = json {{"globalTags", FAKE_GLOBAL_TAGS},
                               {"payloadiovs", FAKE_PAYLOAD_IOVS}};
};

}
