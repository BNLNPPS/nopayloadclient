#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "nopayloadclient.hpp"


std::vector<std::string> extractPlTypes(nlohmann::json plDict){
  std::vector<std::string> plTypes;
  for (auto& el : plDict.items()){
    std::cout<<"el.key() = "<<el.key()<<std::endl;
    plTypes.push_back(el.key());
  }
  return plTypes;
}


int main()
{
  std::string gtName = "sPHENIX_ExampleGT_1";
  int nIovs = 1;
  std::string basePath = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/";
  nlohmann::json payloadDict;
  payloadDict["Beam"] =        basePath + "D0DXMagnets.dat";
  payloadDict["FieldMap"] =    basePath + "sphenix3dbigmapxyz.root";
  payloadDict["CEMC_Thresh"] = basePath + "CEMCprof_Thresh30MeV.root";
  payloadDict["ZDC"] =         basePath + "towerMap_ZDC.txt";
  payloadDict["CEMC_Geo"] =    basePath + "cemc_geoparams-0-0-4294967295-1536789215.xml";

  nlohmann::json resp;
  resp = nopayloadclient::createGlobalTag(gtName);
  for (auto plType : extractPlTypes(payloadDict)) {
    resp = nopayloadclient::createPayloadType(plType);
  }

  for (auto& el : payloadDict.items()){
    for (int i=0; i<nIovs; i++){
      resp = nopayloadclient::insertPayload(gtName, el.key(), el.value(), i, 0);
    }
  }

  return 0;
}

