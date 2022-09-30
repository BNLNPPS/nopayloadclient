#include <iostream>
#include <vector>
#include "plmover.hpp"

int main()
{
  std::string localUrl = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/prompt/data.dat";
  std::string remoteUrl = plmover::getRemoteUrl("my_gt", "my_pl_type", 1);
  if (!plmover::fileExists(localUrl)) return 1;
  if (plmover::fileExists("nonexistingfile")) return 1;
  if (plmover::fileExists(remoteUrl)){
    std::remove(remoteUrl.c_str());
  }
  plmover::uploadFile(localUrl, "my_gt", "my_pl_type", 1);
  if (!plmover::fileExists(remoteUrl)) return 1;
  return EXIT_SUCCESS;
}
