#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include <time.h>
#include <stdlib.h>
#include <sstream>
#include <filesystem>
#include <unistd.h>

#include <config.hpp>
#include "nopayloadclient.hpp"


int getPayloadNumber() {
  const std::filesystem::path pl_path{config::remote_pl_dir};
  std::filesystem::recursive_directory_iterator pl_iterator{pl_path};
  int n = 0;
  for (auto const& dir_entry : pl_iterator) {
      if (!std::filesystem::is_directory(dir_entry.path())) n++;
  }
  return n;
}

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
  int n_pl_0 = getPayloadNumber();

  if (createRandomPayload(my_local_url) == 1) return 1;

  // create the global tag if it does not exist
  resp = nopayloadclient::createGlobalTag("my_gt");
  std::cout << resp << std::endl;

  // create the payload type if it does not exist
  resp = nopayloadclient::createPayloadType("my_pt");
  std::cout << resp << std::endl;

  // insert should work
  resp = nopayloadclient::insertPayload("my_gt", "my_pt", my_local_url, rand_iov, 0);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // number of payloads should have increased by one
  int n_pl_1 = getPayloadNumber();
  if (n_pl_1 != (n_pl_0 + 1)) return 1;

  // getting the url from the DB again
  resp = nopayloadclient::get("my_gt", "my_pt", rand_iov, 0);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // inserting another iov with the same payload should not change number of payloads
  resp = nopayloadclient::insertPayload("my_gt", "my_pt", my_local_url, rand_iov, 0);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;
  int n_pl_2 = getPayloadNumber();
  if (n_pl_2 != n_pl_1) return 1;


  // ++++++++++++++++++++++++++++++
  //    THINGS THAT SHOULD FAIL
  // ++++++++++++++++++++++++++++++

  // try inserting to a global tag that does not exist should cause an error
  resp = nopayloadclient::insertPayload("non_existing_gt", "my_pt", my_local_url, 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;

  // ... same for payload type that does not exist...
  resp = nopayloadclient::insertPayload("my_gt", "non_existing_type", my_local_url, 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;

  // ... and if the payload file does not exist locally ...
  resp = nopayloadclient::insertPayload("my_gt", "my_pt", "non_existing_file", 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;

//  // ... and if a payload with same IOV already exists ...
//  resp = nopayloadclient::insertPayload("my_gt", "my_pt", my_local_url, rand_iov, 0);
//  std::cout<<resp<<std::endl;
//  if (resp["code"]==0) return 1;

  return EXIT_SUCCESS;
}
