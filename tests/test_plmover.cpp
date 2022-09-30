#include <iostream>
#include <vector>
#include "plmover.hpp"

int main()
{
  plmover::fileExists("data/prompt/data.dat");
  //std::vector<std::string> globalTagNames = nopayloadclient::getGlobalTagNames();
  //std::cout<<"globalTagNames =";
  //for (auto i: globalTagNames){std::cout<<" "<<i;}
  return EXIT_SUCCESS;
  //auto reponse = nopayloadclient::getHash("SCE", 5844);
}
