#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <time.h>
#include <experimental/filesystem>
#include <unistd.h>
#include <thread>

#include <nopayloadclient/nopayloadclient.hpp>
#include <nopayloadclient/cache.hpp>

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
  json conf {{"cache_life_time", 0.1},
             {"cache_max_mb", 0.0001}};
  nopayloadclient::Cache cache {conf};
  std::cout << "cache = " << cache << std::endl;
  std::cout << "cache.contains('a') = " << cache.contains("a") << std::endl;
  json resp1 {"jigga jigga"};
  json resp2 {{"pl_type_1", "url_1"}, {"pl_type_2", "url_2"}};
  cache.set("a", resp1);
  cache.set("b", resp2);
  cache.set("c", resp2); // this should replace entry for 'a' (max size)
  std::cout << "cache = \n" << cache << std::endl;
  std::cout << "cache.contains('c') = " << cache.contains("c") << std::endl; // should be valid
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "cache.contains('c') = " << cache.contains("c") << std::endl; // but not this one

  return 0;
}
