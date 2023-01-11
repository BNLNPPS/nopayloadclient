#include <curlwrapper.hpp>

namespace curlwrapper{

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

class CurlMession{
    private:
        Answer ans;
        CURL *curl;
        std::string url;
        struct curl_slist *slist1 = NULL;
        std::string jsonStr;
        int n_retries = config::n_retries;
    public:
        CurlMession(std::string _url){
            url = _url;
            curl = curl_easy_init();
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ans.readBuffer);
        }

        void printResults(){
            std::cout<<"res = "<<ans.res<<std::endl;
            std::cout<<"readBuffer = "<<ans.readBuffer<<std::endl;
            std::cout<<"httpCode = "<<ans.httpCode<<std::endl;
        }

        nlohmann::json try_execute(){
            nlohmann::json answer;
            for(int i = 0; i<n_retries; i++){
                try{return execute();}
                catch (std::runtime_error& e){
                    std::cout<<e.what()<<std::endl;
                    std::chrono::seconds(i*i);
                }
            }
            std::string msg = "curl failed after n=" + std::to_string(n_retries);
            msg += " tries (url: " + url + ")";
            throw BaseException(msg);
            return answer;
        }

        nlohmann::json execute(){
	        using namespace std::chrono;
            if (config::print_time_stamps) {
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
            nlohmann::json response = nlohmann::json::parse(ans.readBuffer);
            std::cout << "response = " << response << std::endl;
            if (ans.httpCode!=200){
                std::string msg;
                if (response.contains("name")) msg = response["name"][0];
                else if (response.contains("detail")) msg = response["detail"];
                else msg = response.dump();
                std::cout << "msg = " << msg << std::endl;
                throw DataBaseException(msg);
            }
            return response;
        }

        void prepareGet(){
        }

        void prepareDelete(){
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        }

        void preparePut(){
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        }

        void preparePost(nlohmann::json jsonData){
            slist1 = curl_slist_append(slist1, "Content-Type: application/json");
            jsonStr = jsonData.dump();
	    //            std::cout<<"jsonStr = "<<jsonStr<<std::endl;
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
        }

        void preparePut(nlohmann::json jsonData){
            preparePut();
            preparePost(jsonData);
        }
};

nlohmann::json del(std::string url){
    //std::cout<<"backend::get(url="<<url<<")"<<std::endl;
    CurlMession cm = CurlMession(url);
    cm.prepareDelete();
    return cm.try_execute();
}

nlohmann::json get(std::string url){
    //std::cout<<"backend::get(url="<<url<<")"<<std::endl;
    CurlMession cm = CurlMession(url);
    cm.prepareGet();
    return cm.try_execute();
}

nlohmann::json post(std::string url, nlohmann::json jsonData){
    //std::cout<<"backend::post(url="<<url<<", jsonData="<<jsonData<<")"<<std::endl;
    CurlMession cm = CurlMession(url);
    cm.preparePost(jsonData);
    return cm.try_execute();
}

nlohmann::json put(std::string url){
    //std::cout<<"backend::put(url="<<url<<")"<<std::endl;
    CurlMession cm = CurlMession(url);
    cm.preparePut();
    return cm.try_execute();
}

nlohmann::json put(std::string url, nlohmann::json jsonData){
    //std::cout<<"backend::put(url="<<url<<", jsonData="<<jsonData<<")"<<std::endl;
    CurlMession cm = CurlMession(url);
    cm.preparePut(jsonData);
    return cm.try_execute();
}

}
