#include <nopayloadclient/curlwrapper.hpp>

namespace npc {

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

CurlWrapper::CurlWrapper(const json& config) {
    base_url_ = "http://";
    base_url_ += config["base_url"];
    base_url_ += config["api_res"];
    n_retries_ = config["n_retries"];
    print_time_stamps_ = config["print_time_stamps"];
}

json CurlWrapper::del(std::string url){
//    std::cout<<"CurlWrapper::del(url="<<url<<")"<<std::endl;
    CurlSession cm = CurlSession(base_url_ + url, n_retries_, print_time_stamps_);
    cm.prepareDelete();
    return cm.try_execute();
}

json CurlWrapper::get(std::string url){
//    std::cout<<"CurlWrapper::get(url="<<url<<")"<<std::endl;
    CurlSession cm = CurlSession(base_url_ + url, n_retries_, print_time_stamps_);
    cm.prepareGet();
    return cm.try_execute();
}

json CurlWrapper::post(std::string url, json jsonData){
//    std::cout<<"CurlWrapper::post(url="<<url<<", jsonData="<<jsonData<<")"<<std::endl;
    CurlSession cm = CurlSession(base_url_ + url, n_retries_, print_time_stamps_);
    cm.preparePost(jsonData);
    return cm.try_execute();
}

json CurlWrapper::put(std::string url){
//    std::cout<<"CurlWrapper::put(url="<<url<<")"<<std::endl;
    CurlSession cm = CurlSession(base_url_ + url, n_retries_, print_time_stamps_);
    cm.preparePut();
    return cm.try_execute();
}

json CurlWrapper::put(std::string url, json jsonData){
//    std::cout<<"CurlWrapper::put(url="<<url<<", jsonData="<<jsonData<<")"<<std::endl;
    CurlSession cm = CurlSession(base_url_ + url, n_retries_, print_time_stamps_);
    cm.preparePut(jsonData);
    return cm.try_execute();
}

CurlSession::CurlSession(std::string _url, int n_retries, bool print_time_stamps){
    n_retries_ = n_retries;
    print_time_stamps_ = print_time_stamps;
    url = _url;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, npc::WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ans.readBuffer);
}

void CurlSession::printResults(){
    std::cout<<"res = "<<ans.res<<std::endl;
    std::cout<<"readBuffer = "<<ans.readBuffer<<std::endl;
    std::cout<<"httpCode = "<<ans.httpCode<<std::endl;
}

json CurlSession::try_execute(){
    json answer;
    for(int i = 0; i<n_retries_; i++){
        try{return execute();}
        catch (std::runtime_error& e){
            std::cout<<e.what()<<std::endl;
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
    if (print_time_stamps_) {
        std::cout << "begin curl: " << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() << '\n';
        ans.res = curl_easy_perform(curl);
        std::cout << "end curl: " << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() << '\n';
    }
    else {
        ans.res = curl_easy_perform(curl);
    }
    if ( ans.res!=0 ){
        std::string const msg = "curl_easy_perform() failed with error code: " + std::to_string(ans.res);
        throw std::runtime_error(msg);
    }
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &ans.httpCode);
    curl_easy_cleanup(curl);
    json response = json::parse(ans.readBuffer);
    //std::cout << "response = " << response << std::endl;
    if (ans.httpCode!=200){
        std::string msg;
        if (response.contains("name")) msg = response["name"][0];
        else if (response.contains("detail")) msg = response["detail"];
        else msg = response.dump();
        //std::cout << "msg = " << msg << std::endl;
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

void CurlSession::preparePost(json jsonData){
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    jsonStr = jsonData.dump();
    //std::cout<<"jsonStr = "<<jsonStr<<std::endl;
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
}

void CurlSession::preparePut(json jsonData){
    preparePut();
    preparePost(jsonData);
}

}
