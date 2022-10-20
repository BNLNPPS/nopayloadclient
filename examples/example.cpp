#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "nopayloadclient.hpp"
#include "backend.hpp"


void prepareGlobalTag(std::string gtName){
  backend::createGlobalTagStatus("unlocked");
  backend::createGlobalTagObject(gtName, "unlocked");
}

std::vector<std::string> extractPlTypes(nlohmann::json plDict){
  std::vector<std::string> plTypes;
  for (auto& el : plDict.items()){
    std::cout<<"el.key() = "<<el.key()<<std::endl;
    plTypes.push_back(el.key());
  }
  return plTypes;
}

void preparePlTypes(nlohmann::json plDict, std:: string gtName){
  std::vector<std::string> plTypes = extractPlTypes(plDict);
  for (auto& plType : plTypes){
    backend::createPayloadType(plType);
    std::string pllName = backend::createPayloadList(plType);
    backend::attachPayloadList(gtName, pllName);
  }
}



int main()
{
  std::string gtName = "sPHENIX_ExampleGT_1";
  std::string basePath = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/";
  nlohmann::json payloadDict;
  payloadDict["Beam"] =        basePath + "D0DXMagnets.dat";
  payloadDict["FieldMap"] =    basePath + "sphenix3dbigmapxyz.root";
  payloadDict["CEMC_Thresh"] = basePath + "CEMCprof_Thresh30MeV.root";
  payloadDict["ZDC"] =         basePath + "towerMap_ZDC.txt";
  payloadDict["CEMC_Geo"] =    basePath + "cemc_geoparams-0-0-4294967295-1536789215.xml";

  prepareGlobalTag(gtName);
  preparePlTypes(payloadDict, gtName);

  for (auto& el : payloadDict.items()){
    nopayloadclient::insertPayload(gtName, el.key(), el.value(), 0, 0);
  }

  return 0;
}

