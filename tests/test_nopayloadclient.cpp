#include <iostream>
#include <vector>
#include "nopayloadclient.hpp"

int main()
{
  nopayloadclient::insertPayload("my_gt", "testtype", "data/prompt/data.dat", 0);
  //std::vector<std::string> globalTagNames = nopayloadclient::getGlobalTagNames();
  //std::cout<<"globalTagNames =";
  //for (auto i: globalTagNames){std::cout<<" "<<i;}
  return EXIT_SUCCESS;
  //auto reponse = nopayloadclient::getHash("SCE", 5844);
}
