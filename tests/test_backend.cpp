#include <iostream>
#include <vector>
#include "backend.hpp"

int main()
{
  //system("ls");

//  backend::createGlobalTagStatus("locked");
//  backend::createGlobalTagStatus("unlocked");
//  backend::createGlobalTagType("my_gt_type");
//  backend::createGlobalTagObject("my_gt", "unlocked", "my_gt_type");
//  backend::createPayloadType("my_pl_type");
  std::string pll_name = backend::createPayloadList("my_pl_type");
  backend::attachPayloadList("my_gt", pll_name);
  backend::lockGlobalTag("my_gt");
  int pll_id = backend::createPayloadIOV("data/prompt/data.dat", 1, 1);
  //int pll_id_2 = backend::createPayloadIOV("data/prompt/file.dat", 1, 1, 2, 2);
  backend::attachPayloadIOV(pll_name, pll_id);
  std::cout<<backend::getGlobalTags()<<std::endl;
  return 0;
}
