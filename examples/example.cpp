#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "nopayloadclient.hpp"
#include "config.hpp"

int main()
{
  std::cout<<"Hello, world!"<<std::endl;
  std::vector PL_TYPEs = {"Beam", "FieldMap", "CEMC_Thresh", "ZDC", "CEMC_Geo"};
    for(auto word: PL_TYPEs){
      std::cout<<word<<std::endl;
  }

  std::string basePath = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/";
  nlohmann::json payloadDict;
  payloadDict["Beam"] =        "D0DXMagnets.dat";
  payloadDict["FieldMap"] =    "sphenix3dbigmapxyz.root";
  payloadDict["CEMC_Thresh"] = "CEMCprof_Thresh30MeV.root";
  payloadDict["ZDC"] =         "towerMap_ZDC.txt";
  payloadDict["CEMC_Geo"] =    "cemc_geoparams-0-0-4294967295-1536789215.xml";

  std::string gtName = "sPHENIX_ExampleGT_1";

  std::cout<<"payloadDict = "<<payloadDict<<std::endl;
  for (auto& el : payloadDict.items()){
    std::cout<<"key: "<<el.key()<<", value: "<< el.value() << std::endl;
    nopayloadclient::insertPayload(gtName, el.key(), el.value(), 0);
  }




  /*
  std::cout<<config::rawDict()<<std::endl;

  nopayloadclient::insertPayload("my_gt", "my_pl_type",
   "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/prompt/data.dat", 0);

  //std::cout<<"NOPAYLOADCLIENT_DATA_INSTALL_DIR_FULL = "<<std::endl;
  //std::cout<<std::getenv("@NOPAYLOADCLIENT_DATA_INSTALL_DIR_FULL@")<<std::endl;
  //nopayloadclient::get("my_gt", "my_pl_type", 1);
  */
  return 0;
}
