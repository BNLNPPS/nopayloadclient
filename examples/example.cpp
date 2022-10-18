#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "nopayloadclient.hpp"
#include "backend.hpp"

int main()
{
  std::vector PL_TYPEs = {"Beam", "FieldMap", "CEMC_Thresh", "ZDC", "CEMC_Geo"};

  std::string basePath = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/";
  nlohmann::json payloadDict;
  payloadDict["Beam"] =        basePath + "D0DXMagnets.dat";
  payloadDict["FieldMap"] =    basePath + "sphenix3dbigmapxyz.root";
  payloadDict["CEMC_Thresh"] = basePath + "CEMCprof_Thresh30MeV.root";
  payloadDict["ZDC"] =         basePath + "towerMap_ZDC.txt";
  payloadDict["CEMC_Geo"] =    basePath + "cemc_geoparams-0-0-4294967295-1536789215.xml";

  std::string gtName = "sPHENIX_ExampleGT_1";
  backend::createGlobalTagStatus("unlocked");
  backend::createGlobalTagObject(gtName, "unlocked");

  for (auto& plType : PL_TYPEs){
    backend::createPayloadType(plType);
    std::string pllName = backend::createPayloadList(plType);
    backend::attachPayloadList(gtName, pllName);
  }

  for (auto& el : payloadDict.items()){
    nopayloadclient::insertPayload(gtName, el.key(), el.value(), 0);
  }


  return 0;
}
