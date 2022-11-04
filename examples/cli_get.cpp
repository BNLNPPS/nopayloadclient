#include <iostream>
#include <vector>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include "nopayloadclient.hpp"


int main(int argc, char *argv[])
{
  std::string global_tag = argv[1];
  std::string pl_type = argv[2];
  int major_iov = std::atoi(argv[3]);
  int minor_iov = std::atoi(argv[4]);

  nlohmann::json resp;
  resp = nopayloadclient::get(global_tag, pl_type, major_iov, minor_iov);
  std::cout<<resp<<std::endl;
  return 0;
}

