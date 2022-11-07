#include <iostream>
#include <vector>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include "backend.hpp"
#include "nopayloadclient.hpp"
#include "exception.hpp"


int main(int argc, char *argv[])
{
  int n_gt = std::atoi(argv[1]);
  int n_pt = std::atoi(argv[2]);
  int n_iov = std::atoi(argv[3]);
  std::vector<std::string> gts;
  std::vector<std::string> pts;
  std::string temp;

  std::cout<<"inserting data for "<<n_gt<<" global tags, "<<n_pt<<" payload types, and "<<n_iov<<" IOvs."<<std::endl;

  for(int i=0; i<n_gt; i++){
    temp = "global_tag_" + std::to_string(i);
    gts.push_back(temp);
    std::cout<<nopayloadclient::createGlobalTag(temp)<<std::endl;
  }

  for(int j=0; j<n_pt; j++){
    temp = "pl_type_" + std::to_string(j);
    pts.push_back(temp);
    std::cout<<nopayloadclient::createPayloadType(temp)<<std::endl;
  }

  for(auto gt : gts){
    for(auto pt : pts){
      backend::prepareInsertIov(gt, pt);
      for(int i=0; i<n_iov; i++){
        std::cout<<"inserting into gt="<<gt<<", pt="<<pt<<", iov="<<i<<std::endl;
        backend::insertIov(gt, pt, "file.dat", i, 0);
      }
    }
  }

  std::cout<<nopayloadclient::getSize()<<std::endl;

  return 0;
}

