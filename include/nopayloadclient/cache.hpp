#pragma once

#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

#include <nopayloadclient/exception.hpp>


template <typename T>
class CachedProp {
    public:
        bool is_valid = false;
        T content;
        void update(T new_content){
          content = new_content;
          is_valid = true;
        }
};

class Cache {
    public:
        CachedProp<std::vector<std::string>> gt_status_names;
        CachedProp<std::vector<std::string>> gt_names;
        CachedProp<std::vector<std::string>> pt_names;
        CachedProp<nlohmann::json> pt_dict;// {gt_0: {pl_type_0: pl_type_0_3, pl_type_1: pl_type_1_6, ...}} ONLY ONE GT!
        void invalidate() {
            gt_status_names.is_valid = false;
            gt_names.is_valid = false;
            pt_names.is_valid = false;
            pt_dict.is_valid = false;
        }
};
