#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include "nopayloadclient.hpp"


int main()
{
  nlohmann::json resp;
  resp = nopayloadclient::getSize();
  std::cout<<resp<<std::endl;

  return 0;
}

