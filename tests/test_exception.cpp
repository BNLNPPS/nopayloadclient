#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient/exception.hpp>


int main()
{
  std::cout<<"testing exceptions..."<<std::endl;
  BaseException e1 = BaseException("generic error message");
  PayloadException e2 = PayloadException("payload exception");
  DataBaseException e3 = DataBaseException("database exception");
  //PayloadException e3 = ServerException('"{\"detail\":\"Not found.\"}');
  //{"code":1,"msg":"{\"detail\":\"Not found.\"}"}
  std::cout << "e1.jsonify() = " << e1.jsonify() << std::endl;
  std::cout << "e2.jsonify() = " << e2.jsonify() << std::endl;
  std::cout << "e3.jsonify() = " << e3.jsonify() << std::endl;
  return 0;
}
