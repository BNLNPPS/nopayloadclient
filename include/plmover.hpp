#pragma once

#include <iostream>
#include <sys/stat.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <unistd.h>

#include <exception.hpp>
#include <config.hpp>
#include <payload.hpp>

namespace plmover {

    void compareCheckSums(std::string firstFileUrl, std::string secondFileUrl);
    void prepareUploadFile(payload::Payload& pl);
    void uploadFile(payload::Payload& pl);

}
