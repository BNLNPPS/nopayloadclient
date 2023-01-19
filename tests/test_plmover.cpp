#include <iostream>
#include <vector>
#include <nopayloadclient/plmover.hpp>
#include <nopayloadclient/exception.hpp>

int main()
{

  return 0;


  /*
  std::string cs = plmover::getCheckSum("/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/test.dat");
  std::cout << "cs = " << cs << std::endl;
  std::string remote_dir = plmover::getRemoteDir(cs);
  std::cout << "remote_dir = " << remote_dir << std::endl;

  return 0;
  std::string localUrl = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/data.dat";
  std::string remoteUrl = plmover::getRemoteUrl("my_gt", "my_pl_type", 1, 1);
//  if (!plmover::fileExists(localUrl)) return 1;
//  if (plmover::fileExists("nonexistingfile")) return 1;
//  if (plmover::fileExists(remoteUrl)){
//    std::remove(remoteUrl.c_str());
//  }
  plmover::uploadFile(localUrl, "my_gt", "my_pl_type", 1, 1);
  try {
    plmover::compareCheckSums(localUrl, "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/towerMap_ZDC.txt");
  }
  catch (NoPayloadException &e){
    std::cout<<e.what()<<std::endl;
  }
  //if (!plmover::fileExists(remoteUrl)) return 1;
  return EXIT_SUCCESS;
  */
}
