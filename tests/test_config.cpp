#include <iostream>
#include <vector>
#include "config.hpp"
#include "backend.hpp"
#include <nlohmann/json.hpp>

int main()
{
  std::cout<<"testing config"<<std::endl;
  nlohmann::json rawDict = config::rawDict();
  std::cout<<"rawDict = "<<rawDict<<std::endl;
  nlohmann::json rawDict2 = config::rawDict();
  std::cout<<"rawDict2 = "<<rawDict2<<std::endl;
  std::cout<<"apiUrl() = "<<config::apiUrl()<<std::endl;
//  nlohmann::json conf = config::fromFile("/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/config/test.json");
//  nlohmann::json conf2 = config::fromFile();
//  std::cout<<"config = "<<conf<<std::endl;
//  std::cout<<"config2 = "<<conf2<<std::endl;
  //nlohmann::json j = backend::getGlobalTags();
  //std::cout<<"j = "<<j<<std::endl;
  return EXIT_SUCCESS;
}
