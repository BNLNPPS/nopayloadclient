#include <iostream>
#include <nlohmann/json.hpp>
#include "nopayloadclient.hpp"


int main()
{
  std::string basePath = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/";
  //std::string basePath = "/lbne/u/lgerlach1/Projects/nopayloadclient/data/local/";

  nlohmann::json resp;
  resp = nopayloadclient::insertPayload("sPHENIX_ExampleGT_1", "Beam", basePath+"D0DXMagnets.dat", 0, 0);
  std::cout<<resp<<std::endl;
  resp = nopayloadclient::insertPayload("sPHENIX_ExampleGT_1", "Beam", basePath+"D0DXMagnets.dat", 0, 0);
  std::cout<<resp<<std::endl;
  /*
  resp = nopayloadclient::insertPayload("sPHENIX_ExampleGT_1", "FieldMap", basePath+"sphenix3dbigmapxyz.root", 0, 0);
  std::cout<<resp<<std::endl;
  resp = nopayloadclient::insertPayload("sPHENIX_ExampleGT_1", "CEMC_Thresh", basePath+"CEMCprof_Thresh30MeV.root", 0, 0);
  std::cout<<resp<<std::endl;
  resp = nopayloadclient::insertPayload("sPHENIX_ExampleGT_1", "CEMC_Geo", basePath+"cemc_geoparams-0-0-4294967295-1536789215.xml", 0, 0);
  std::cout<<resp<<std::endl;
  */
  return 0;
}

