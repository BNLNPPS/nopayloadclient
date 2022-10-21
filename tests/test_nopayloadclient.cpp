#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "nopayloadclient.hpp"

int main()
{
  // prepare tests by removing payloads from plstorage
  std::filesystem::remove_all("/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/remote/*");

  // define some parameters for the rest of this test
  std::string myGt = "my_gt";
  std::string myPt = "my_pl_type";
  std::string myUrl = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/data.dat";
  nlohmann::json resp;

  // try inserting to a global tag that does not exist should cause an error
  resp = nopayloadclient::insertPayload("non_existing_gt", myPt, myUrl, 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;

  // same for payload type that does not exist...
  resp = nopayloadclient::insertPayload(myGt, "non_existing_type", myUrl, 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;

  // ... and if the payload file does not exist locally
  resp = nopayloadclient::insertPayload(myGt, myPt, "non_existing_file", 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;


  resp = nopayloadclient::insertPayload("my_gt", "my_pl_type", "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/data.dat", 0, 1);
  std::cout<<resp<<std::endl;
  resp = nopayloadclient::insertPayload("my_gt", "my_pl_type", "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/prompt/data.dat", 1, 1);
  std::cout<<resp<<std::endl;

  // Reading
  nlohmann::json ans1 = nopayloadclient::get("my_gt", "my_pl_type", 1, 1);
  std::cout<<"ans1 = "<<ans1<<std::endl;

  return EXIT_SUCCESS;
}
