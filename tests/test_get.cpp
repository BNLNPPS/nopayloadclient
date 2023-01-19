#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient/nopayloadclient.hpp>
#include <nopayloadclient/config.hpp>


int main()
{
  nlohmann::json resp = nopayloadclient::get("my_gt", "my_pt", 1410065408, 0);
  std::cout<<resp<<std::endl;

  return 0;
}
