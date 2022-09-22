#include <iostream>
#include <vector>
#include "backend.hpp"

int main()
{
  //backend::createGlobalTagStatus("linos_status");
  //backend::createGlobalTagType("linos_gt_type");
  //backend::createGlobalTagObject("linos_object", "linos_status", "linos_gt_type");
  //backend::createPayloadType("linos_pl_type");
  //std::string pll_name = backend::createPayloadList("linos_pl_type");
  //std::cout<<"pll_name = "<<pll_name<<std::endl;
  //backend::attachPayloadList("linos_gt", pll_name);
  backend::lockGlobalTag("linos_gt");
  //std::string pll_id = backend::createPayloadIOV("data/file.dat", 1, 1);
  ////backend::createPayloadIOV("data/file.dat", 1, 1, 2, 2);
  //backend::attachPayloadIOV("Beam", pll_id);
  return 0;
}

//
//
//
