#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "nopayloadclient.hpp"


int main()
{
  nlohmann::json resp;
  resp = nopayloadclient::get("sPHENIX_ExampleGT_1", "Beam", 0, 0);
  std::cout<<resp<<std::endl;

  return 0;
}

