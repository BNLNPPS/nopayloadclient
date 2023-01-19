#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient/nopayloadclient.hpp>
#include <nopayloadclient/backend.hpp>


int main()
{
  std::cout<<"testing cache..."<<std::endl;
  std::cout<<"gtExists = "<<backend::gtExists("dinger")<<std::endl;
  std::cout<<"gtExists = "<<backend::gtExists("dinger")<<std::endl;
  nopayloadclient::createGlobalTag("yoho");
  std::cout<<"gtExists = "<<backend::gtExists("dinger")<<std::endl;
  std::cout<<"gtExists = "<<backend::gtExists("yoho")<<std::endl;
  return 0;
}
