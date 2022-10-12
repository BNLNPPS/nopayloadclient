#include <iostream>
#include <vector>
#include "plmover.hpp"
#include "nopayloadclient.hpp"

int main()
{
  // prepare tests by removing payloads from plstorage
  //std::filesystem::remove_all("/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/plstorage/my_gt/");

  // Writing
  //nopayloadclient::insertPayload("my_gt", "my_pl_type", "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/prompt/data.dat", 0);
  //nopayloadclient::insertPayload("my_gt", "my_pl_type", "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/prompt/data.dat", 1);

  // Reading
  std::string remoteUrl = nopayloadclient::get("my_gt", "my_pl_type", 42);
  std::cout<<"remoteUrl = "<<remoteUrl<<std::endl;

  return EXIT_SUCCESS;
}
