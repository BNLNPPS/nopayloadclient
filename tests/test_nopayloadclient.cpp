#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <time.h>
#include <experimental/filesystem>

#include <nopayloadclient/nopayloadclient.hpp>

using json = nlohmann::json;

namespace fs = std::experimental::filesystem::v1;

int getPayloadNumber(fs::path pl_path) {
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

  std::cout << "initializing helper variables ..." << std::endl;
  char my_local_url[] = "/tmp/file.dat";

  json resp;
  srandom(time(NULL));

  long long major_iov_start = randLong(0, 1e6);
  long long minor_iov_start = randLong(0, 1e6);
  long long major_iov_end = randLong(major_iov_start, 10e6);
  long long minor_iov_end = randLong(minor_iov_start, 10e6);
  long long major_iov = randLong(major_iov_start, major_iov_end);
  long long minor_iov = randLong(minor_iov_start, minor_iov_end);

  nopayloadclient::Client client {"my_gt"};
  json conf_dict = client.getConfDict()["msg"];

  int n_pl_0 = getPayloadNumber(conf_dict["write_dir"]);

  resp = client.checkConnection();
  std::cout << resp << std::endl;
  std::cout << client.getConfDict() << std::endl;

  // delete & re-create the global tag
  resp = client.deleteGlobalTag();
  std::cout << resp << std::endl;

  resp = client.createGlobalTag();
  std::cout << resp << std::endl;

  // create the payload type if it does not exist
  resp = client.createPayloadType("my_pt");
  std::cout << resp << std::endl;

  // insert should work
  if (createRandomPayload(my_local_url) == 1) return 1;
  std::cout << "attempting insertion" << std::endl;
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov_start, minor_iov_start,
                              major_iov_end, minor_iov_end);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // number of payloads should have increased by one
  int n_pl_1 = getPayloadNumber(conf_dict["write_dir"]);
  if (n_pl_1 != (n_pl_0 + 1)) return 1;

  // getting the url from the DB again should work
  resp = client.getUrlDict(major_iov, minor_iov);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // inserting another iov with the same payload should work...
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov, minor_iov,
                              major_iov_end, minor_iov_end);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // ... but not change the number of payloads
  if (getPayloadNumber(conf_dict["write_dir"]) != n_pl_1) return 1;

  // trying to lock the global tag
  resp = client.lockGlobalTag();
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  // should not be able to write to a locked gt ...
  if (createRandomPayload(my_local_url) == 1) return 1;
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov, minor_iov);
  std::cout << resp << std::endl;
  if (resp["code"] == 0) return 1;

  // ... and not change the number of payloads ...
  if (getPayloadNumber(conf_dict["write_dir"]) != n_pl_1) return 1;

  // ... except if the IOV does not overlap with existing
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov_end, minor_iov_end);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;
  if (getPayloadNumber(conf_dict["write_dir"]) != n_pl_1 + 1) return 1;

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

  // retrieval after deletion should fail...
  resp = client.getUrlDict(major_iov, minor_iov);
  std::cout << resp << std::endl;
  if (resp["code"] == 0) return 1;

  // ... and inserting to a global tag that does not exist...
  client.setGlobalTag("non_existing_gt");
  resp = client.insertPayload("my_pt", my_local_url, 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;
  client.setGlobalTag("my_gt");

  // ... same for payload type that does not exist...
  resp = client.insertPayload("non_existing_type", my_local_url, 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;

  // ... and if the payload file does not exist locally...
  resp = client.insertPayload("my_pt", "non_existing_file", 0, 0);
  std::cout<<resp<<std::endl;
  if (resp["code"]==0) return 1;

  return EXIT_SUCCESS;
}
