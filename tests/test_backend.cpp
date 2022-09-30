#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "backend.hpp"


nlohmann::json getItemWithName(nlohmann::json j, std::string name){
  for (auto gts : j){
    if (gts["name"] == name) return gts;
  }
  return false;
}

bool hasItemWithName(nlohmann::json j, std::string name){
  nlohmann::json item = getItemWithName(j, name);
  if (item["name"] == name) return true;
  return false;
}


int main()
{
  //system("sh ../../scripts/reset_db.sh");

  backend::createGlobalTagStatus("locked");
  if (!hasItemWithName(backend::getGlobalTagStatuses(), "locked")){
    return 1;
  }
  backend::createGlobalTagStatus("unlocked");
  if (!hasItemWithName(backend::getGlobalTagStatuses(), "unlocked")){
    return 1;
  }
  backend::createGlobalTagType("my_gt_type");
  if (!hasItemWithName(backend::getGlobalTagTypes(), "my_gt_type")){
    return 1;
  }
  backend::createGlobalTagObject("my_gt", "unlocked", "my_gt_type");
  if (!hasItemWithName(backend::getGlobalTags(), "my_gt")){
    return 1;
  }

  backend::createPayloadType("my_pl_type");
  if (!hasItemWithName(backend::getPayloadTypes(), "my_pl_type")){
    return 1;
  }

  std::string pll_name = backend::createPayloadList("my_pl_type");
  if (!hasItemWithName(backend::getPayloadLists(), pll_name)){
    return 1;
  }

  backend::unlockGlobalTag("my_gt");
  nlohmann::json my_gt = getItemWithName(backend::getGlobalTags(), "my_gt");
  if (my_gt["status"]!="unlocked"){
    return 1;
  }

  backend::attachPayloadList("my_gt", pll_name);
  nlohmann::json my_pl = getItemWithName(backend::getPayloadLists(), pll_name);
  if (my_pl["global_tag"]!="my_gt"){
    return 1;
  }

  backend::lockGlobalTag("my_gt");
  my_gt = getItemWithName(backend::getGlobalTags(), "my_gt");
  if (my_gt["status"]!="locked"){
    return 1;
  }

  int pll_id = backend::createPayloadIOV("data/prompt/data.dat", 1, 1);
  int pll_id_2 = backend::createPayloadIOV("data/prompt/file.dat", 1, 1, 2, 2);
  backend::attachPayloadIOV(pll_name, pll_id);
  my_gt = getItemWithName(backend::getGlobalTags(), "my_gt");
  std::cout<<backend::getGlobalTags()<<std::endl;
  return 0;
}

