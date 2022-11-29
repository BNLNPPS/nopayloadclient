#pragma once

#include <payload.hpp>

namespace plmover {

    void compareCheckSums(std::string firstFileUrl, std::string secondFileUrl);
    void prepareUploadFile(payload::Payload& pl);
    void uploadFile(payload::Payload& pl);

}
