#include <iostream>
#include <nlohmann/json.hpp> // nlohman json object is returned by every nopayloadclient function
#include "nopayloadclient.hpp"


int main()
{
  // local payload folder. this variable is just for convenience
  std::string basePath = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/";
  //std::string basePath = "/lbne/u/lgerlach1/Projects/nopayloadclient/data/local/";

  // make sure global tag & payload type exist
  nopayloadclient::createGlobalTag("sPHENIX_ExampleGT_1");
  nopayloadclient::createPayloadType("Beam");

  // create json object to which the response is written
  nlohmann::json resp;

  // try inserting payload
  resp = nopayloadclient::insertPayload("sPHENIX_ExampleGT_1", "Beam", basePath+"D0DXMagnets.dat", 0, 0);

  // analyse response: code==0 -> OK, code!=0 -> Not OK!
  if (resp["code"]!=0) {
    std::cout<<"something went wrong. error message: "<<resp["msg"]<<std::endl;
  }
  else {
    std::cout<<"all good. details: "<<resp["msg"]<<std::endl;
  }

  // w/o error handling, inserting a payload is a one-liner:
  nopayloadclient::insertPayload("sPHENIX_ExampleGT_1", "Beam", basePath+"D0DXMagnets.dat", 0, 0);
  nopayloadclient::insertPayload("sPHENIX_ExampleGT_1", "FieldMap", basePath+"sphenix3dbigmapxyz.root", 0, 0);
  nopayloadclient::insertPayload("sPHENIX_ExampleGT_1", "CEMC_Thresh", basePath+"CEMCprof_Thresh30MeV.root", 0, 0);
  nopayloadclient::insertPayload("sPHENIX_ExampleGT_1", "CEMC_Geo", basePath+"cemc_geoparams-0-0-4294967295-1536789215.xml", 0, 0);

  return 0;
}

