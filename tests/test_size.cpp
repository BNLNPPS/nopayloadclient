#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient.hpp>
#include <config.hpp>


int main()
{
  nlohmann::json resp = nopayloadclient::getSize();
  std::cout<<resp<<std::endl;
  return 0;
}
