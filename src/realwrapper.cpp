#include <nopayloadclient/realwrapper.hpp>

#include <cstdlib>
#include <random>

namespace nopayloadclient {

RealWrapper::RealWrapper(const json& config) {
    base_url_ = "http://";
    base_url_ += config["base_url"];
    base_url_ += config["api_res"];
    n_retries_ = config["n_retries"];
    retry_sleep_mean_ = config["retry_sleep_mean"];
}

void RealWrapper::sleep(int retry_number) {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1, retry_sleep_mean_*2);
    int n_sleep = dist(rd); // add random sleep
    logging::debug("sleeping for " + std::to_string(n_sleep) + " seconds before retrying...");
    std::this_thread::sleep_for(std::chrono::seconds(n_sleep));
}

json RealWrapper::del(const string& url){
    logging::debug("RealWrapper::del(url=" + url + ")");
    return getResponse<DeleteRequest>(url);
}

json RealWrapper::get(const string& url){
    logging::debug("RealWrapper::get(url=" + url + ")");
    return getResponse<GetRequest>(url);
}

json RealWrapper::post(const string& url, const json& data){
    logging::debug("RealWrapper::post(url=" + url + ", data=" + data.dump() + ")");
    return getResponse<PostRequest>(url, data);
}

json RealWrapper::put(const string& url){
    logging::debug("RealWrapper::put(url=" + url + ")");
    return getResponse<PutRequest>(url);
}

json RealWrapper::put(const string& url, const json& data){
    logging::debug("RealWrapper::put(url=" + url + ", data=" + data.dump() + ")");
    return getResponse<PutRequest>(url, data);
}

int CurlRequest::execute(){
    using namespace std::chrono;
    logging::debug("begin curl: " + std::to_string(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()));
    ans_.res = curl_easy_perform(curl_);
    logging::debug("end curl: " + std::to_string(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()));
    curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &ans_.httpCode);
    curl_easy_cleanup(curl_);
    logging::debug("res = " + std::to_string(ans_.res));
    logging::debug("readBuffer = " + ans_.readBuffer);
    logging::debug("httpCode = " + std::to_string(ans_.httpCode));
    if (ans_.httpCode == 504){
        logging::debug("connection timed-out.");
        return 1;
    }
    return 0;
}

json CurlRequest::parseResponse() {
    json response = json::parse(ans_.readBuffer);
    if (ans_.httpCode!=200){
        std::string msg;
        if (response.contains("name")) msg = response["name"][0];
        else if (response.contains("detail")) msg = response["detail"];
        else msg = response.dump();
        throw DataBaseException(msg);
    }
    return response;
}

}
