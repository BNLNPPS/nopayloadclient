#include <iostream>
#include "nopayloadclient.hpp"
#include "config.hpp"

int main()
{
  std::cout<<"Hello, world!"<<std::endl;
  std::cout<<config::rawDict()<<std::endl;
  
  nopayloadclient::insertPayload("my_gt", "my_pl_type",
   "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/prompt/data.dat", 0);

  //std::cout<<"NOPAYLOADCLIENT_DATA_INSTALL_DIR_FULL = "<<std::endl;
  //std::cout<<std::getenv("@NOPAYLOADCLIENT_DATA_INSTALL_DIR_FULL@")<<std::endl;
  //nopayloadclient::get("my_gt", "my_pl_type", 1);
  return 0;
}
