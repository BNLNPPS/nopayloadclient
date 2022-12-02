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
  std::cout << "createRandomPayload(filename=" << filename << ")" << std::endl;
  std::cout << "attempting to remove existing file..." << std::endl;
  std::filesystem::remove(filename);
  std::cout << "attempting to open new file..." << std::endl;
  int fd = mkstemp(filename);
  std::cout << "fd = " << fd << std::endl;
  if (fd == -1) return 1;
  std::cout << "writing random data to new file..." << std::endl;
  for (int i=0; i<10; i++) {
    std::string ding = std::to_string(random());
    write(fd, ding.c_str(), ding.size() + 1);
  }
  std::cout << "success! random payload file has been created." << std::endl;
  return 0;
}


int main()
{
  std::cout << "initializing helper variables ..." << std::endl;
  char my_local_url[] = "/tmp/file.dat";
  nlohmann::json resp;
  srandom(time(NULL));
  int rand_iov = random();
  int n_pl_0 = getPayloadNumber();

  std::cout << "creating random payload file ..." << std::endl;
  if (createRandomPayload(my_local_url) == 1) return 1;

  std::cout << "creating global tag and payload type (if non-existant) ..." << std::endl;
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
  std::cout << "attempting to retrieve" << std::endl;
  resp = nopayloadclient::get("my_gt", "my_pt", rand_iov, 0);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // inserting another iov with the same payload should not change number of payloads
  resp = nopayloadclient::insertPayload("my_gt", "my_pt", my_local_url, rand_iov, 0);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;
  int n_pl_2 = getPayloadNumber();
  if (n_pl_2 != n_pl_1) return 1;

  // should not be able to write to a locked gt
  resp = nopayloadclient::lockGlobalTag("my_gt");
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  resp = nopayloadclient::insertPayload("my_gt", "my_pt", my_local_url, rand_iov, 0);
  std::cout << resp << std::endl;
  if (resp["code"] == 0) return 1;

  // insertion should work again after unlocking
  resp = nopayloadclient::unlockGlobalTag("my_gt");
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  resp = nopayloadclient::insertPayload("my_gt", "my_pt", my_local_url, rand_iov, 0);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;


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
