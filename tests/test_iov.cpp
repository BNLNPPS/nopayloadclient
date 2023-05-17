#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient/exception.hpp>
#include <nopayloadclient/iov.hpp>
#include <nopayloadclient/moment.hpp>


int main()
{
  std::cout<<"testing iov..."<<std::endl;
  nopayloadclient::Moment mom1 {1, 2};
  nopayloadclient::Moment mom2 {4, 3};
  std::cout << "mom1 = " << mom1 << std::endl;
  std::cout << "mom2 = " << mom2 << std::endl;
  std::cout << "mom1 < mom2 = " << (mom1 < mom2) << std::endl;
  std::cout << "mom2 < mom1 = " << (mom2 < mom1) << std::endl;

  nopayloadclient::IOV iov0 {0, 42};
  nopayloadclient::IOV iov1 {1, 2, 3, 4};
  nopayloadclient::IOV iov2 {mom1, mom2};
  nopayloadclient::IOV iov3 {99, 99};
  std::cout << iov0 << std::endl;
  std::cout << iov1 << std::endl;
  std::cout << iov2 << std::endl;

  std::cout << "iov0.contains(mom1) = " << iov0.contains(mom1) << std::endl;
  std::cout << "iov1.contains(mom1) = " << iov1.contains(mom1) << std::endl;
  std::cout << "iov2.contains(mom1) = " << iov2.contains(mom1) << std::endl;
  std::cout << "iov2.contains(mom2) = " << iov2.contains(mom2) << std::endl;
  std::cout << "iov3.contains(mom1) = " << iov3.contains(mom1) << std::endl;

  return 0;
}
