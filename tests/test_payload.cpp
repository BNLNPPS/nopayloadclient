#include <iostream>
#include <vector>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/exception.hpp>


int main()
{

  std::string local_url = "/Users/linogerlach/Projects/DUNE/ConditionsHandling/nopayloadclient/data/local/test.dat";
  payload::Payload pl = payload::Payload(local_url, "my_type");

  std::cout << "pl = " << pl << std::endl;

  return 0;

}
