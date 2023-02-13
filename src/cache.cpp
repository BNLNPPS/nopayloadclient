#include <nopayloadclient/cache.hpp>

namespace nopayloadclient {

Cache::Cache(const json& config) {
    life_time_ = config["cache_life_time"];
    max_mb_ = config["cache_max_mb"];
}

bool Cache::contains(std::string url) {
    if (!response_dict_.contains(url)) {
        return false;
    }
    long long ts = time_stamp_dict_[url];
    return ((getNowTs() - ts) < life_time_ * 1e6);
}

json Cache::get(std::string url) {
    return response_dict_[url];
}

void Cache::set(std::string url, json& response) {
    response_dict_[url] = response;
    time_stamp_dict_[url] = getNowTs();
    insertion_order_.push(url);
    while (isAboveMaxSize() && !isEmpty()) {
        removeOldestEntry();
    }
}

void Cache::removeOldestEntry() {
    std::string oldest_url = insertion_order_.front();
    insertion_order_.pop();
    time_stamp_dict_.erase(oldest_url);
    response_dict_.erase(oldest_url);
}

bool Cache::isEmpty() {
    return (insertion_order_.size() == 0);
}

bool Cache::isAboveMaxSize() {
    return (getSize() > max_mb_);
}

double Cache::getSize() {
    return response_dict_.dump().size() / 1e6;
}

void Cache::trash() {
    response_dict_.clear();
    time_stamp_dict_.clear();
    insertion_order_ = std::queue<std::string>();
}

long long Cache::getNowTs() {
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
}

std::ostream& operator<< (std::ostream& os, const Cache& c) {
    os << "response dict:" << std::endl;
    os << c.response_dict_ << std::endl;
    os << "time stamp dict:" << std::endl;
    os << c.time_stamp_dict_;
    return os;
};

}
