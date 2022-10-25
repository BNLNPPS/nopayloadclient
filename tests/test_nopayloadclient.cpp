#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include <time.h>
#include <stdlib.h>

#include "nopayloadclient.hpp"
#include "config.hpp"


int main()
{
  // define some parameters for the rest of this test
  std::string myUrl = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/test.dat";
  nlohmann::json resp;
  srandom(time(NULL));
  int randIov = random();

  // create random payload file
  /*
  std::filesystem::remove(myUrl);
  std::ofstream payloadFile(myUrl);
  for (int i=0; i<10; i++) {
    payloadFile << random();
  }
  payloadFile.close();
  */

  // create the global tag if it does not exist
  //resp = nopayloadclient::createGlobalTag("my_gt");
  //std::cout<<resp<<std::endl;

  // create the payload type if it does not exist
  //resp = nopayloadclient::createPayloadType("my_pt");
  //std::cout<<resp<<std::endl;

  // insert should work
  resp = nopayloadclient::insertPayload("my_gt", "my_pt", myUrl, randIov, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==1) return 1;

  // getting the inserted file:
  resp = nopayloadclient::get("my_gt", "my_pt", randIov, 0);
  std::cout<<resp<<std::endl;
  return 0;

  // ++++++++++++++++++++++++++++++
  //    THINGS THAT SHOULD FAIL
  // ++++++++++++++++++++++++++++++

  // try inserting to a global tag that does not exist should cause an error
  resp = nopayloadclient::insertPayload("non_existing_gt", "my_pt", myUrl, 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;

  // ... same for payload type that does not exist...
  resp = nopayloadclient::insertPayload("my_gt", "non_existing_type2", myUrl, 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;

  // ... and if the payload file does not exist locally ...
  resp = nopayloadclient::insertPayload("my_gt", "my_pt", "non_existing_file", 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;

  // ... and if a payload with same IOV already exists ...
  resp = nopayloadclient::insertPayload("my_gt", "my_pt", myUrl, randIov, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;


  return EXIT_SUCCESS;
}
