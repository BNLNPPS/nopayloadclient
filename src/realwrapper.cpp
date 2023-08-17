#include <nopayloadclient/realwrapper.hpp>

namespace nopayloadclient {

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

RealWrapper::RealWrapper(const json& config) {
    base_url_ = "http://";
    base_url_ += config["base_url"];
    base_url_ += config["api_res"];
    n_retries_ = config["n_retries"];
}

json RealWrapper::del(const string& url){
    logging::debug("RealWrapper::del(url=" + url + ")");
    CurlSession cm = CurlSession(base_url_ + url, n_retries_);
    cm.prepareDelete();
    return cm.try_execute();
}

json RealWrapper::get(const string& url){
    logging::debug("RealWrapper::get(url=" + url + ")");
    CurlSession cm = CurlSession(base_url_ + url, n_retries_);
    cm.prepareGet();
    return cm.try_execute();
}

json RealWrapper::post(const string& url, const json& data){
    logging::debug("RealWrapper::post(url=" + url + ", data=" + data.dump() + ")");
    CurlSession cm = CurlSession(base_url_ + url, n_retries_);
    cm.preparePost(data);
    return cm.try_execute();
}

json RealWrapper::put(const string& url){
    logging::debug("RealWrapper::put(url=" + url + ")");
    CurlSession cm = CurlSession(base_url_ + url, n_retries_);
    cm.preparePut();
    return cm.try_execute();
}

json RealWrapper::put(const string& url, const json& data){
    logging::debug("RealWrapper::put(url=" + url + ", data=" + data.dump() + ")");
    CurlSession cm = CurlSession(base_url_ + url, n_retries_);
    cm.preparePut(data);
    return cm.try_execute();
}

CurlSession::CurlSession(const string& _url, int n_retries){
    n_retries_ = n_retries;
    url = _url;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ans.readBuffer);
}

void CurlSession::logResults(){
    logging::debug("res = " + std::to_string(ans.res));
    logging::debug("readBuffer = " + ans.readBuffer);
    logging::debug("httpCode = " + std::to_string(ans.httpCode));
}

json CurlSession::try_execute(){
    json answer;
    for(int i = 0; i<n_retries_; i++){
        try{return execute();}
        catch (std::runtime_error& e){
            logging::warning(e.what());
            std::chrono::seconds(i*i);
        }
    }
    std::string msg = "curl failed after n=" + std::to_string(n_retries_);
    msg += " tries (url: " + url + ")";
    throw BaseException(msg);
    return answer;
}

json CurlSession::execute(){
    using namespace std::chrono;
    logging::debug("begin curl: " + std::to_string(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()));
    ans.res = curl_easy_perform(curl);
    logging::debug("end curl: " + std::to_string(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()));
    if ( ans.res!=0 ){
        std::string const msg = "curl_easy_perform() failed with error code: " + std::to_string(ans.res);
        throw std::runtime_error(msg);
    }
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &ans.httpCode);
    curl_easy_cleanup(curl);
    logResults();
    json response = json::parse(ans.readBuffer);
    if (ans.httpCode!=200){
        std::string msg;
        if (response.contains("name")) msg = response["name"][0];
        else if (response.contains("detail")) msg = response["detail"];
        else msg = response.dump();
        throw DataBaseException(msg);
    }
    return response;
}

void CurlSession::prepareGet(){
}

void CurlSession::prepareDelete(){
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
}

void CurlSession::preparePut(){
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
}

void CurlSession::preparePost(const json& data){
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    json_str = data.dump();
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
}

void CurlSession::preparePut(const json& data){
    preparePut();
    preparePost(data);
}

}
