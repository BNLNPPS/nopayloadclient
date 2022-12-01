#include <iostream>
#include <nlohmann/json.hpp> // every method returns a json object
#include "sphenixclient.hpp"


int main()
{

  // ++++++++++++++++++++++++++++++
  //   PREPARATION / HELPER VARS
  // ++++++++++++++++++++++++++++++

  // local payload folder. this variable is just for convenience
  std::string basePath = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/";

  // make sure global tag & payload type exist
  std::cout << sphenixclient::createGlobalTag("sPHENIX_ExampleGT_1") << std::endl;
  std::cout << sphenixclient::createPayloadType("Beam") << std::endl;;

  // create json object to which the response is written
  nlohmann::json resp;


  // ++++++++++++++++++++++++++++++
  //         INSERTION
  // ++++++++++++++++++++++++++++++

  // try inserting payload
  resp = sphenixclient::insertPayload("sPHENIX_ExampleGT_1", "Beam", basePath+"D0DXMagnets.dat", 0);

  // analyse response: code==0 -> OK, code!=0 -> Not OK!
  if (resp["code"]!=0) {
    std::cout<<"something went wrong. error message: "<<resp["msg"]<<std::endl;
  }
  else {
    std::cout<<"all good. details: "<<resp["msg"]<<std::endl;
  }

  // w/o error handling, inserting a payload is a one-liner:
  resp = sphenixclient::insertPayload("sPHENIX_ExampleGT_1", "Beam", basePath+"D0DXMagnets.dat", 0);
  std::cout << resp << std::endl;


  // ++++++++++++++++++++++++++++++
  //          RETRIEVAL
  // ++++++++++++++++++++++++++++++

  resp = sphenixclient::get("sPHENIX_ExampleGT_1", "Beam", 42);
  std::cout << resp << std::endl;

  return 0;
}

