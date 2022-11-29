#include <iostream>
#include <vector>
#include "payload.hpp"
#include "exception.hpp"


int main()
{

  std::string local_url = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/test.dat";
  payload::Payload pl = payload::Payload(local_url, "my_type");

  std::cout << "pl = " << pl << std::endl;

  return 0;

}
