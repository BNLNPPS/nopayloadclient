#include <iostream>

#include "nopayloadclient.hpp"

int main()
{
  auto globalTags = nopayloadclient::getGlobalTags();
  std::cout<<"globalTags = "<<globalTags<<std::endl;
  //auto globalTagNames = nopayloadclient::getGlobalTagNames();
  //std::cout<<"globalTagNames = "<<globalTagNames<<std::endl;
  return EXIT_SUCCESS;
  //auto reponse = nopayloadclient::getHash("SCE", 5844);
}
