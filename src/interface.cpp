#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <algorithm>

#include <backend.hpp>
#include <plmover.hpp>


namespace nopayloadclient {

// Reading



// Writing
void insertPayload(std::string gtName, std::string gtType, std::string fileUrl, int iovStart){
    std::cout<<"insertPayload()"<<std::endl;
    if (!backend::gtExists(gtName)){
        std::cout<<"No global tag with name "<<gtName<<" exists."<<std::endl;
        return;
    }
    if (!backend::plTypeExists(gtType)){
        std::cout<<"No payload type "<<gtType<<" exists."<<std::endl;
        return;
    }
    if (!plmover::fileExists(fileUrl)){
        std::cout<<"File "<<fileUrl<<" does not exist."<<std::endl;
        return;
    }

    /*
    std::vector<std::string> globalTagNames = getGlobalTagNames();
    for (auto i: globalTagNames){
        std::cout<<"i = "<<i<<std::endl;
    }
    bool found = (std::find(globalTagNames.begin(), globalTagNames.end(), gtName) != globalTagNames.end());
    std::cout<<"found = "<<found<<std::endl;
    */
//    backend::createGlobalTagObject(gtName, "locked", gtType);
}


}