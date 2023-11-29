#include <nopayloadclient/realwrapper.hpp>

namespace nopayloadclient {

RealWrapper::RealWrapper(const json& config) {
    base_url_ = "http://";
    base_url_ += config["base_url"];
    base_url_ += config["api_res"];
    n_retries_ = config["n_retries"];
}

json RealWrapper::del(const string& url){
    logging::debug("RealWrapper::del(url=" + url + ")");
    return executeTemp<DeleteSession>(url);
}

json RealWrapper::get(const string& url){
    logging::debug("RealWrapper::get(url=" + url + ")");
    return executeTemp<GetSession>(url);
}

json RealWrapper::post(const string& url, const json& data){
    logging::debug("RealWrapper::post(url=" + url + ", data=" + data.dump() + ")");
    return executeTemp<PostSession>(url, data);
}

json RealWrapper::put(const string& url){
    logging::debug("RealWrapper::put(url=" + url + ")");
    return executeTemp<PutSession>(url);
}

json RealWrapper::put(const string& url, const json& data){
    logging::debug("RealWrapper::put(url=" + url + ", data=" + data.dump() + ")");
    return executeTemp<PutSession>(url, data);
}

void CurlSession::executeVoid(){
    using namespace std::chrono;
    logging::debug("begin curl: " + std::to_string(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()));
    ans.res = curl_easy_perform(curl);
    logging::debug("end curl: " + std::to_string(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()));
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &ans.httpCode);
    curl_easy_cleanup(curl);
    logging::debug("res = " + std::to_string(ans.res));
    logging::debug("readBuffer = " + ans.readBuffer);
    logging::debug("httpCode = " + std::to_string(ans.httpCode));
}

}