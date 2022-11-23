#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "nopayloadclient.hpp"


int main()
{
  std::string gtName = "sPHENIX_ExampleGT_1";
  int nIovs = 10;
  std::string basePath = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/";
  //std::string basePath = "/lbne/u/lgerlach1/Projects/nopayloadclient/data/local/";

  nlohmann::json payloadDict;
  payloadDict["Beam"] =        basePath + "D0DXMagnets.dat";
  payloadDict["FieldMap"] =    basePath + "sphenix3dbigmapxyz.root";
  payloadDict["CEMC_Thresh"] = basePath + "CEMCprof_Thresh30MeV.root";
  payloadDict["ZDC"] =         basePath + "towerMap_ZDC.txt";
  payloadDict["CEMC_Geo"] =    basePath + "cemc_geoparams-0-0-4294967295-1536789215.xml";

  nlohmann::json resp;

  resp = nopayloadclient::createGlobalTag(gtName);
  std::cout<<"resp = "<<resp<<std::endl;

  for (const auto& el : payloadDict.items()){
    resp = nopayloadclient::createPayloadType(el.key());
    std::cout<<"resp = "<<resp<<std::endl;
    for (int i=0; i<nIovs; i++){
      resp = nopayloadclient::insertPayload(gtName, el.key(), el.value(), i, 0);
      std::cout<<"resp = "<<resp<<std::endl;
    }
  }

  return 0;
}

