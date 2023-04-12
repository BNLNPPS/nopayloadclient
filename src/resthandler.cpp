#include <nopayloadclient/resthandler.hpp>

namespace nopayloadclient {

// Config
RESTHandler::RESTHandler(const json& config) {
    //curlwrapper_ = CurlWrapper(config);
    if (config["use_fake_backend"]) {
        curlwrapper_.reset(new CurlFaker(config));
    }
    else {
        curlwrapper_.reset(new CurlWrapper(config));
    }
    cache_ = Cache(config);
    use_cache_ = true;
}

void RESTHandler::clearCache() {
    cache_.trash();
}

// Reading
json RESTHandler::getGlobalTags() {
    return get("globalTags");
}

json RESTHandler::getGlobalTagStatuses() {
    return get("gtstatus");
}

json RESTHandler::getPayloadTypes() {
    return get("pt");
}

json RESTHandler::getPayloadLists(const string& global_tag) {
    return get("gtPayloadLists/" + global_tag);
}

json RESTHandler::getPayloadIOVs(const string& global_tag, Moment& mom){
    return get("payloadiovs/?gtName=" + global_tag + "&majorIOV=" +
               std::to_string(mom.major_) + "&minorIOV=" + std::to_string(mom.minor_));
}

json RESTHandler::getPayloadIOVsSQL(const string& global_tag, Moment& mom){
    return get("payloadiovssql/?gtName=" + global_tag + "&majorIOV=" +
               std::to_string(mom.major_) + "&minorIOV=" + std::to_string(mom.minor_));
}

// Writing
void RESTHandler::createGlobalTagStatus(const string& name){
    post("gtstatus", {{"name", name}});
}

void RESTHandler::createGlobalTagObject(const string& name, const string& status) {
    json j = {
        {"name", name},
        {"status", status},
        {"author", std::getenv("USER")}
    };
    post("gt", j);
}

void RESTHandler::createPayloadType(const string& name){
    post("pt", {{"name", name}});
}

string RESTHandler::createPayloadList(const string& type) {
    json res = post("pl", {{"payload_type", type}});
    return res["name"];
}

void RESTHandler::attachPayloadList(const string& global_tag, const string& pl_name) {
    json j = {
        {"payload_list", pl_name},
        {"global_tag", global_tag}
    };
    put("pl_attach", j);
}

void RESTHandler::lockGlobalTag(const string& name) {
    put("gt_change_status/" + name + "/locked");
}

void RESTHandler::unlockGlobalTag(const string& name){
    put("gt_change_status/" + name + "/unlocked");
}

void RESTHandler::cloneGlobalTag(const string& source, const string& target) {
    string url = "cloneGlobalTag/" + source + "/" + target;
    post(url, {});
}

ll RESTHandler::createPayloadIOV(Payload& pl, IOV& iov){
    json j = {
        {"payload_url", pl.remote_url},
        {"major_iov", iov.start_.major_},
        {"minor_iov", iov.start_.minor_},
        {"checksum", pl.check_sum}
    };
    if (!iov.is_open_){
        j["major_iov_end"] = iov.end_.major_;
        j["minor_iov_end"] = iov.end_.minor_;
    }
    json res = post("piov", j);
    return res["id"];
}

void RESTHandler::attachPayloadIOV(const string& pll_name, ll piov_id){
    json j = {
        {"payload_list", pll_name},
        {"piov_id", piov_id}
    };
    put("piov_attach", j);
}

void RESTHandler::deleteGlobalTag(const string& name) {
    del("deleteGlobalTag/" + name);
}

// Private
json RESTHandler::get(const string& url) {
    if (!use_cache_) return curlwrapper_->get(url);
    if (!cache_.contains(url)) {
        json resp = curlwrapper_->get(url);
        cache_.set(url, resp);
    }
    return cache_.get(url);
}

json RESTHandler::del(const string& url, bool trash_cache) {
    if (trash_cache) cache_.trash();
    return curlwrapper_->del(url);
}

json RESTHandler::put(const string& url, bool trash_cache) {
    if (trash_cache) cache_.trash();
    return curlwrapper_->put(url);
}

json RESTHandler::put(const string& url, json data, bool trash_cache) {
    if (trash_cache) cache_.trash();
    return curlwrapper_->put(url, data);
}

json RESTHandler::post(const string& url, json data, bool trash_cache) {
    if (trash_cache) cache_.trash();
    return curlwrapper_->post(url, data);
}

}
