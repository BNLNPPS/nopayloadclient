#pragma once

#include <iostream>
#include <sys/stat.h>
#include <string>
//#include <filesystem>
#include <experimental/filesystem>
#include <fstream>
#include <unistd.h>

#include <nopayloadclient/exception.hpp>
#include <nopayloadclient/config.hpp>
#include <nopayloadclient/payload.hpp>

namespace plmover {

    void compareCheckSums(std::string firstFileUrl, std::string secondFileUrl);
    void prepareUploadFile(payload::Payload& pl);
    void uploadFile(payload::Payload& pl);

}
