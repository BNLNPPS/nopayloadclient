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
  char my_other_local_url[] = "/tmp/other_file.dat";
  char my_overriden_url[] = "/tmp/overriden_file.dat";

  json resp;
  srandom(time(NULL));

  long long major_iov_start = randLong(0, 1e6);
  long long minor_iov_start = randLong(0, 1e6);
  long long major_iov_end = randLong(major_iov_start, 10e6);
  long long minor_iov_end = randLong(minor_iov_start, 10e6);
  long long major_iov = randLong(major_iov_start, major_iov_end);
  long long minor_iov = randLong(minor_iov_start, minor_iov_end);

  nopayloadclient::NoPayloadClient client {"my_gt"};
  json conf_dict = client.getConfDict()["msg"];

  std::cout << "create pl write dir if it does not exist.." << std::endl;
  if (!fs::is_directory(conf_dict["write_dir"]) || !fs::exists(conf_dict["write_dir"])) {
      fs::create_directory(conf_dict["write_dir"]);
  }

  int n_pl_0 = getPayloadNumber(conf_dict["write_dir"]);

  resp = client.checkConnection();
  std::cout << resp << std::endl;

  std::cout << "delete & re-create the global tag..." << std::endl;
  resp = client.deleteGlobalTag();
  std::cout << resp << std::endl;

  resp = client.createGlobalTag();
  std::cout << resp << std::endl;

  std::cout << "create the payload type if it does not exist..." << std::endl;
  resp = client.createPayloadType("my_pt");
  std::cout << resp << std::endl;

  std::cout << "insert should work" << std::endl;
  if (createRandomPayload(my_local_url) == 1) return 1;
  std::cout << "attempting insertion" << std::endl;
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov_start, minor_iov_start,
                              major_iov_end, minor_iov_end);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  std::cout << "number of payloads should have increased by one" << std::endl;
  int n_pl_1 = getPayloadNumber(conf_dict["write_dir"]);
  if (n_pl_1 != (n_pl_0 + 1)) return 1;

  std::cout << "getting the url from the DB again should work" << std::endl;
  resp = client.getUrlDict(major_iov, minor_iov);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  std::cout << "check cloning..." << std::endl;
  json my_url_dict = resp["msg"];
  resp = client.cloneGlobalTag("cloned_gt");
  std::cout << resp << std::endl;
  client.setGlobalTag("cloned_gt");
  json cloned_url_dict = client.getUrlDict(major_iov, minor_iov)["msg"];
  if (my_url_dict != cloned_url_dict) return 1;
  client.deleteGlobalTag();
  client.setGlobalTag("my_gt");

  std::cout << "check overriding..." << std::endl;
  resp = client.createPayloadType("overriden_pt");
  std::cout << resp << std::endl;
  if (createRandomPayload(my_overriden_url) == 1) return 1;
  resp = client.insertPayload("overriden_pt", my_overriden_url,
                              major_iov_start, minor_iov_start,
                              major_iov_end, minor_iov_end);
  std::cout << resp << std::endl;
  resp = client.getUrlDict(major_iov, minor_iov);
  std::cout << resp << std::endl;
  std::string old_file_name = resp["msg"]["overriden_pt"];
  client.override("overriden_pt", my_overriden_url);
  resp = client.getUrlDict(major_iov, minor_iov);
  std::string new_file_name = resp["msg"]["overriden_pt"];
  if (old_file_name == new_file_name) return 1;
  if (new_file_name != my_overriden_url) return 1;

  std::cout << "inserting another iov with the same payload should work..." << std::endl;
  int n_pl_2 = getPayloadNumber(conf_dict["write_dir"]);
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov, minor_iov,
                              major_iov_end, minor_iov_end);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  std::cout << "... but not change the number of payloads" << std::endl;
  if (getPayloadNumber(conf_dict["write_dir"]) != n_pl_2) return 1;

  std::cout << "try locking the global tag..." << std::endl;
  resp = client.lockGlobalTag();
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  std::cout << "should not be able to write to a locked gt ..." << std::endl;
  if (createRandomPayload(my_local_url) == 1) return 1;
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov, minor_iov);
  std::cout << resp << std::endl;
  if (resp["code"] == 0) return 1;

  std::cout << "... and not change the number of payloads ..." << std::endl;
  if (getPayloadNumber(conf_dict["write_dir"]) != n_pl_2) return 1;

  std::cout << "... except if the IOV does not overlap with existing" << std::endl;
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov_end, minor_iov_end);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;
  if (getPayloadNumber(conf_dict["write_dir"]) != n_pl_2 + 1) return 1;

  std::cout << "deletion of locked global tag should fail" << std::endl;
  resp = client.deleteGlobalTag();
  std::cout << resp << std::endl;
  if (resp["code"] == 0) return 1;

  std::cout << "trying to unlock global tag..." << std::endl;
  resp = client.unlockGlobalTag();
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  std::cout << "insertion should work again after unlocking" << std::endl;
  resp = client.insertPayload("my_pt", my_local_url,
                              major_iov, minor_iov);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  std::cout << "creating a payload type to be deleted..." << std::endl;
  resp = client.createPayloadType("pt_to_delete");
  json before_insertion = client.getUrlDict(major_iov, minor_iov);
  resp = client.insertPayload("pt_to_delete", my_local_url,
                              major_iov_start, minor_iov_start,
                              major_iov_end, minor_iov_end);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;
  json after_insertion = client.getUrlDict(major_iov, minor_iov);
  if (before_insertion == after_insertion) return 1;

  std::cout << "... which should not work when containing IOVs" << std::endl;
  resp = client.deletePayloadType("pt_to_delete");
  std::cout << resp << std::endl;
  if (resp["code"] == 0) return 1;

  std::cout << "should be able to delete iov..." << std::endl;
  resp = client.deletePayloadIOV("pt_to_delete", major_iov_start, minor_iov_start, major_iov_end, minor_iov_end);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  std::cout << ".. and receive same resp as before insertion" << std::endl;
  json after_deletion = client.getUrlDict(major_iov, minor_iov);
  if (before_insertion != after_deletion) return 1;

  std::cout << "deletion of payload type w/o iovs should work..." << std::endl;
  resp = client.deletePayloadType("pt_to_delete");
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  std::cout << "... and pt should not appear in resp afterwards" << std::endl;
  resp = client.getUrlDict(major_iov, minor_iov);
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;
  if (resp["msg"].contains("pt_to_delete")) return 1;

  std::cout << "deletion of unlocked global tag should work" << std::endl;
  resp = client.deleteGlobalTag();
  std::cout << resp << std::endl;
  if (resp["code"] != 0) return 1;

  std::cout << "retrieval after deletion should fail..." << std::endl;
  resp = client.getUrlDict(major_iov, minor_iov);
  std::cout << resp << std::endl;
  if (resp["code"] == 0) return 1;

  std::cout << "... and inserting to a global tag that does not exist..." << std::endl;
  client.setGlobalTag("non_existing_gt");
  resp = client.insertPayload("my_pt", my_local_url, 0, 0);
  std::cout << resp << std::endl;
  if (resp["code"]==0) return 1;
  client.setGlobalTag("my_gt");

  std::cout << "... same for payload type that does not exist..." << std::endl;
  resp = client.insertPayload("non_existing_type", my_local_url, 0, 0);
  std::cout << resp << std::endl;
  if (resp["code"]==0) return 1;

  std::cout << "... and if the payload file does not exist locally..." << std::endl;
  resp = client.insertPayload("my_pt", "non_existing_file", 0, 0);
  std::cout << resp << std::endl;
  if (resp["code"]==0) return 1;


  return EXIT_SUCCESS;
}
