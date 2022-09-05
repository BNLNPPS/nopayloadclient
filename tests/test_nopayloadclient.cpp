#include <iostream>
#include <vector>
#include "nopayloadclient.hpp"

int main()
{
  auto globalTags = nopayloadclient::getGlobalTags();
  std::cout<<"globalTags = "<<globalTags<<std::endl;
  std::vector<std::string> globalTagNames = nopayloadclient::getGlobalTagNames();
  std::cout<<"globalTagNames =";
  for (auto i: globalTagNames){std::cout<<" "<<i;}
  return EXIT_SUCCESS;
  //auto reponse = nopayloadclient::getHash("SCE", 5844);
}
