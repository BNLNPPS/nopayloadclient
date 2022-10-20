#include <iostream>
#include "nopayloadclient.hpp"

int main()
{
  std::string remoteUrl = nopayloadclient::get("my_gt", "my_pl_type", 1, 1);
  return 0;
}
