#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient.hpp>
#include <config.hpp>


int main()
{
  std::cout<<"running test_get()..."<<std::endl;
//  std::cout<<"config::dict = "<<config::dict<<std::endl;
  //std::cout<<"config::api_url = "<<config::api_url<<std::endl;
  nlohmann::json resp = nopayloadclient::get("my_gt", "my_pt", 1410065408, 0);
  //std::cout<<resp["msg"]<<std::endl;
  return 0;
}
