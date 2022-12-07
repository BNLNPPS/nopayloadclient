#include <iostream>
#include <vector>
#include <config.hpp>
#include <backend.hpp>
#include <plmover.hpp>
#include <nlohmann/json.hpp>

int main()
{
  std::cout << "testing config" << std::endl;
  std::cout << "api_url = " << config::api_url << std::endl;
  std::cout << "n_retries = " << config::n_retries << std::endl;

  std::cout << "listing remote payload dirs next" << std::endl;
  for (const auto s : config::read_dir_list) {
    std::cout << s << std::endl;
  }

  return EXIT_SUCCESS;
}
