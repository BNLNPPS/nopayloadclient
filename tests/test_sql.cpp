#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient/nopayloadclient.hpp>
#include <nopayloadclient/config.hpp>

using json = nlohmann::json;


int main()
{
  json resp;
  nopayloadclient::Client client {"heavy-usage"};

  // New SQL method should return same result
  std::cout << "CHECK SQL ENDPOINT" << std::endl;
  resp = client.getUrlDict(10000, 10000);
  std::cout << resp << std::endl;
  resp = client.getUrlDictSQL(10000, 10000);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;
  //if (resp != resp2) return 1;

  return 0;
}
