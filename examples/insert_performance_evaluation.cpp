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

  std::vector<int> iovs;
  std::string gt_temp;
  std::string pt_temp;
  nlohmann::json existing_gt_dict; // {gt_0: {pt_0: [0,1,...], pt_1: [0,1,...], ...}, gt_1: ...}
  nlohmann::json pll_dict = backend::getPayloadLists();
  for ( const auto & pll : pll_dict){
    gt_temp = pll["global_tag"];
    pt_temp = pll["payload_type"];
    iovs.clear();
    for (const auto & piov : pll["payload_iov"]) {
      iovs.push_back(piov["major_iov"]);
    }
    existing_gt_dict[gt_temp][pt_temp] = iovs;
  }
  std::cout << existing_gt_dict << std::endl;

  //  exit(0);

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

  std::vector<int> existing_iovs;
  for(auto gt : gts){
    for(auto pt : pts){
      std::cout << "gt = " << gt << ", pt = " << pt << std::endl;
      backend::prepareInsertIov(gt, pt);
      for(int i=0; i<n_iov; i++){
        std::cout<<"probing gt="<<gt<<", pt="<<pt<<", iov="<<i<<std::endl;
	if (std::find(existing_gt_dict[gt][pt].begin(), existing_gt_dict[gt][pt].end(), i) != existing_gt_dict[gt][pt].end()) {
	  std::cout << "entry exists already. skipping..." << std::endl;
	  continue;
	}
	std::cout << "performing insertion ... " << std::endl;
	backend::insertIov(gt, pt, "file.dat", i, 0);
      }
    }
  }

  std::cout<<nopayloadclient::getSize()<<std::endl;

  return 0;
}

