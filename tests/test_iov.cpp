#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient/exception.hpp>
#include <nopayloadclient/iov.hpp>


int main()
{
  std::cout<<"testing iov..."<<std::endl;
  iov::Moment mom1 = iov::Moment(1, 2);
  iov::Moment mom2 = iov::Moment(0, 3);
  std::cout << "mom1 = " << mom1 << std::endl;
  std::cout << "mom2 = " << mom2 << std::endl;
  std::cout << "mom1 < mom2 = " << (mom1 < mom2) << std::endl;

  iov::IOV iov1 {1, 2, 3, 4};
  std::cout << iov1 << std::endl;
  return 0;
}
