#pragma once

#include <vector>
#include <iostream>
#include <chrono>
#include <queue>
#include <nlohmann/json.hpp>

#include <nopayloadclient/exception.hpp>


namespace nopayloadclient {

using nlohmann::json;
using std::string;

class Cache {
public:
    Cache() {};
    Cache(const json& config);

    /// should check if it exists AND
    /// entry is less than life time old
    bool contains(const string& url);

    json get(const string& url);

    /// put in first, then removed oldest entry
    /// until size is under max size again
    void set(const string& url, json& response);
    void trash();
    friend std::ostream& operator<< (std::ostream& os, const Cache& c);

private:
    /// of the form {url_1: resp_1, url_2, resp_2, ...}
    json response_dict_;
    /// of the form {url_1: ts_1, url_2: ts_2, ...}
    json time_stamp_dict_;
    /// to find oldest element quickly
    std::queue<string> insertion_order_;
    void removeOldestEntry();
    bool isAboveMaxSize();
    double getSize();
    bool isEmpty();
    long long getNowTs();
    unsigned int life_time_;
    unsigned int max_mb_;
};

}
