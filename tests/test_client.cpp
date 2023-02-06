#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <time.h>
#include <experimental/filesystem>

#include <nopayloadclient/nopayloadclient.hpp>

namespace fs = std::experimental::filesystem::v1;

int getPayloadNumber() {
  nlohmann::json conf_dict = config::getDict();
  std::cout << "conf_dict = " << conf_dict << std::endl;
  const fs::path pl_path = conf_dict["write_dir"];
  fs::recursive_directory_iterator pl_iterator{pl_path};
  int n = 0;
  for (auto const& dir_entry : pl_iterator) {
      if (!fs::is_directory(dir_entry.path())) n++;
  }
  return n;
}

int createRandomPayload(char filename[]) {
  fs::remove(filename);
  std::ofstream my_file(filename);
  for (int i=0; i<10; i++) {
    my_file << std::to_string(random());
  }
  my_file.close();
  return 0;
}

long long randLong(long long lower, long long upper) {
    return lower + (random() % (upper-lower+1));
}


int main()
{

  /*
  nopayloadclient::Client client2 = nopayloadclient::Client("my_gt");
  nopayloadclient::Client client3 = nopayloadclient::Client();

  client3.setGlobalTag("my_gtski");

  std::cout << client2 << std::endl;
  std::cout << client3 << std::endl;

  return 0;
  */

  std::cout << "initializing helper variables ..." << std::endl;
  char my_local_url[] = "/tmp/file.dat";
  char my_local_url_2[] = "/tmp/file_2.dat";

  nlohmann::json resp;
  srandom(time(NULL));

  long long major_iov_start = randLong(0, 1e6);
  long long minor_iov_start = randLong(0, 1e6);
  long long major_iov_end = randLong(major_iov_start, 10e6);
  long long minor_iov_end = randLong(minor_iov_start, 10e6);
  long long major_iov = randLong(major_iov_start, major_iov_end);
  long long minor_iov = randLong(minor_iov_start, minor_iov_end);

  int n_pl_0 = getPayloadNumber();

  // create random payload files
  if (createRandomPayload(my_local_url) == 1) return 1;
  if (createRandomPayload(my_local_url_2) == 1) return 1;

  nopayloadclient::Client client = nopayloadclient::Client("my_gt");

  resp = client.checkConnection();
  std::cout << resp << std::endl;

  // create the global tag if it does not exist
  resp = client.deleteGlobalTag();
  std::cout << resp << std::endl;

  resp = client.createGlobalTag();
  std::cout << resp << std::endl;

  // create the payload type if it does not exist
  resp = client.createPayloadType("my_pt");
  std::cout << resp << std::endl;

  // insert should work
  std::cout << "attempting insertion" << std::endl;
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov_start, minor_iov_start,
                              major_iov_end, minor_iov_end);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;


  // ---------------- TEMP
  //std::cout << client.getTypeUrlDict("my_gt", major_iov, minor_iov) << std::endl;
  //return 0;
  // ---------------- TEMP



  // number of payloads should have increased by one
  int n_pl_1 = getPayloadNumber();
  if (n_pl_1 != (n_pl_0 + 1)) return 1;

  // getting the url from the DB again should work
  resp = client.get("my_pt", major_iov, minor_iov);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // inserting another iov with the same payload should work...
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov, minor_iov,
                              major_iov_end, minor_iov_end);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // ... but not change the number of payloads
  int n_pl_2 = getPayloadNumber();
  if (n_pl_2 != n_pl_1) return 1;

  // trying to lock the global tag
  resp = client.lockGlobalTag();
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // should not be able to write to a locked gt ...
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov, minor_iov);
  std::cout << resp << std::endl;
  if (resp["code"] == 0) return 1;

  // ... except if the IOV does not overlap with existing
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov_end, minor_iov_end);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // deletion of locked global tag should fail
  resp = client.deleteGlobalTag();
  std::cout << resp << std::endl;
  if (resp["code"] == 0) return 1;

  // trying to unlock global tag
  resp = client.unlockGlobalTag();
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // insertion should work again after unlocking
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov, minor_iov);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // deletion of unlocked global tag should work
  resp = client.deleteGlobalTag();
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // retrieval after deletion should fail
  resp = client.get("my_pt", major_iov, minor_iov);
  std::cout << resp << std::endl;
  if (resp["code"] == 0) return 1;


  // ++++++++++++++++++++++++++++++
  //    THINGS THAT SHOULD FAIL
  // ++++++++++++++++++++++++++++++

  // try inserting to a global tag that does not exist should cause an error
  client.setGlobalTag("non_existing_gt");
  resp = client.insertPayload("my_pt", my_local_url, 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;
  client.setGlobalTag("my_gt");

  // ... same for payload type that does not exist...
  resp = client.insertPayload("non_existing_type", my_local_url, 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;

  // ... and if the payload file does not exist locally ...
  resp = client.insertPayload("my_pt", "non_existing_file", 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;

  return EXIT_SUCCESS;
}
