#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include <time.h>
#include <stdlib.h>
#include <sstream>
#include <filesystem>
#include <unistd.h>

#include "sphenixclient.hpp"

int createRandomPayload(char filename[]) {
  std::filesystem::remove(filename);
  int fd = mkstemp(filename);
  if (fd == -1) return 1;
  for (int i=0; i<10; i++) {
    std::string ding = std::to_string(random());
    write(fd, ding.c_str(), ding.size() + 1);
  }
  return 0;
}


int main()
{
  // initialize helper variables for this test
  char my_local_url[] = "/tmp/file.dat";
  nlohmann::json resp;
  srandom(time(NULL));
  int rand_iov = random();

  if (createRandomPayload(my_local_url) == 1) return 1;

  // create the global tag if it does not exist
  resp = sphenixclient::createGlobalTag("my_gt");
  std::cout << resp << std::endl;

  /*
  // create the payload type if it does not exist
  resp = sphenixclient::createPayloadType("my_pt");
  std::cout << resp << std::endl;

  // insert should work
  resp = sphenixclient::insertPayload("my_gt", "my_pt", my_local_url, rand_iov);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;
  */

  return EXIT_SUCCESS;
}
