#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

#include <backend.hpp>
#include <payload.hpp>
#include <plmover.hpp>
#include <exception.hpp>


namespace nopayloadclient {
    // Reading
    nlohmann::json get(std::string gt_name, std::string pl_type,
                       long long major_iov, long long minor_iov);
    nlohmann::json getSize();

    // Writing
    nlohmann::json createPayloadType(std::string pl_type);
    nlohmann::json createGlobalTag(std::string gt_name);
    nlohmann::json lockGlobalTag(std::string gt_name);
    nlohmann::json unlockGlobalTag(std::string gt_name);
    nlohmann::json insertPayload(std::string gt_name, std::string pl_type, std::string file_url,
                                 long long major_iov_start, long long minor_iov_start);
    nlohmann::json insertPayload(std::string gt_name, std::string pl_type, std::string file_url,
                                 long long major_iov_start, long long minor_iov_start,
                                 long long major_iov_end, long long minor_iov_end);
}
