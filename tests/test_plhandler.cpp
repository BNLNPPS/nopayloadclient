#include <iostream>
#include <vector>
#include <nopayloadclient/plhandler.hpp>
#include <nopayloadclient/exception.hpp>

int main()
{

  return 0;


  /*
  std::string cs = plhandler::getCheckSum("/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/test.dat");
  std::cout << "cs = " << cs << std::endl;
  std::string remote_dir = plhandler::getRemoteDir(cs);
  std::cout << "remote_dir = " << remote_dir << std::endl;

  return 0;
  std::string localUrl = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/data.dat";
  std::string remoteUrl = plhandler::getRemoteUrl("my_gt", "my_pl_type", 1, 1);
//  if (!plhandler::fileExists(localUrl)) return 1;
//  if (plhandler::fileExists("nonexistingfile")) return 1;
//  if (plhandler::fileExists(remoteUrl)){
//    std::remove(remoteUrl.c_str());
//  }
  plhandler::uploadFile(localUrl, "my_gt", "my_pl_type", 1, 1);
  try {
    plhandler::compareCheckSums(localUrl, "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/towerMap_ZDC.txt");
  }
  catch (NoPayloadException &e){
    std::cout<<e.what()<<std::endl;
  }
  //if (!plhandler::fileExists(remoteUrl)) return 1;
  return EXIT_SUCCESS;
  */
}
