#include <iostream>
#include <vector>
#include "backend.hpp"

int main()
{
  backend::createGlobalTagStatus("linos_status");
  backend::createGlobalTagType("linos_gt_type");
  backend::createGlobalTagObject("linos_object", "linos_status", "linos_gt_type");
  backend::createPayloadType("linos_pl_type");
  std::string pll_name = backend::createPayloadList("linos_pl_type");
  std::cout<<"pll_name = "<<pll_name<<std::endl;
  backend::attachPayloadList("linos_gt", pll_name);
  //return EXIT_SUCCESS;
  return 0;
}
