#include <iostream>
#include <vector>
#include <config.hpp>
#include <backend.hpp>
#include <plmover.hpp>
#include <nlohmann/json.hpp>

int main()
{
  std::cout<<"testing config"<<std::endl;
  std::cout<<"api_url = "<<config::api_url<<std::endl;
  std::cout<<"remote_pl_dir = "<<config::remote_pl_dir<<std::endl;
  std::cout<<"n_retries = "<<config::n_retries<<std::endl;





//  std::cout<<"apiUrl() = "<<config::apiUrl()<<std::endl;
//  nlohmann::json conf = config::fromFile("/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/config/test.json");
//  nlohmann::json conf2 = config::fromFile();
//  std::cout<<"config = "<<conf<<std::endl;
//  std::cout<<"config2 = "<<conf2<<std::endl;
  //nlohmann::json j = backend::getGlobalTags();
  //std::cout<<"j = "<<j<<std::endl;
  return EXIT_SUCCESS;
}
