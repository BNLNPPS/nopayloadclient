#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient/exception.hpp>
#include <nopayloadclient/iov.hpp>
#include <nopayloadclient/moment.hpp>


int main()
{
  std::cout<<"testing iov..."<<std::endl;
  npc::Moment mom1 {1, 2};
  npc::Moment mom2 {4, 3};
  std::cout << "mom1 = " << mom1 << std::endl;
  std::cout << "mom2 = " << mom2 << std::endl;
  std::cout << "mom1 < mom2 = " << (mom1 < mom2) << std::endl;
  std::cout << "mom2 < mom1 = " << (mom2 < mom1) << std::endl;

  npc::IOV iov0 {0, 42};
  npc::IOV iov1 {1, 2, 3, 4};
  npc::IOV iov2 {mom1, mom2};
  std::cout << iov0 << std::endl;
  std::cout << iov1 << std::endl;
  std::cout << iov2 << std::endl;
  return 0;
}
