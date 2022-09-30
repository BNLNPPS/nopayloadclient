#include <iostream>
#include <sys/stat.h>

namespace plmover {
bool fileExists(const std::string fileUrl){
    std::cout<<"plmover::fileExists(fileUrl="<<fileUrl<<")"<<std::endl;
    struct stat buffer;
    bool exists = (stat (fileUrl.c_str(), &buffer) ==0);
    std::cout<<"exists = "<<exists<<std::endl;
    return false;
}
}