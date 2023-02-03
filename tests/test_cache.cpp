#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <time.h>
#include <experimental/filesystem>

#include <nopayloadclient/nopayloadclient.hpp>

namespace fs = std::experimental::filesystem::v1;


int createRandomPayload(char filename[]) {
  srandom(time(NULL));
  fs::remove(filename);
  std::ofstream my_file(filename);
  for (int i=0; i<10; i++) {
    my_file << std::to_string(random());
  }
  my_file.close();
  return 0;
}


int main()
{
  char my_local_url[] = "/tmp/file.dat";
  nlohmann::json resp;

  if (createRandomPayload(my_local_url) == 1) return 1;

  nopayloadclient::Client client = nopayloadclient::Client();
  resp = client.checkConnection();
  std::cout << resp << std::endl;

  client.deleteGlobalTag("my_gt");
  client.createGlobalTag("my_gt");
  client.createPayloadType("my_pt_1");
  client.createPayloadType("my_pt_2");

  // insert should work
  std::cout << client.insertPayload("my_gt", "my_pt_1", my_local_url, 42, 42) << std::endl;
  std::cout << client.insertPayload("my_gt", "my_pt_2", my_local_url, 42, 42) << std::endl;

  std::cout << "Curlwrapper::get should only be called once here:" << std::endl;
  resp = client.get("my_gt", "my_pt_1", 43, 43);
  resp = client.get("my_gt", "my_pt_2", 43, 43);
  std::cout << resp << std::endl;

  return EXIT_SUCCESS;
}
