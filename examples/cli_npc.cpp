#include <iostream>
#include <nopayloadclient/nopayloadclient.hpp>

void get(char *argv[]){
  std::string global_tag = argv[2];
  std::string pl_type = argv[3];
  long long major_iov = std::atoi(argv[4]);
  long long minor_iov = std::atoi(argv[5]);
  std::cout << nopayloadclient::get(global_tag, pl_type, major_iov, minor_iov) << std::endl;
}

void createGlobalTag(char *argv[]) {
  std::string gt = argv[2];
  std::cout << nopayloadclient::createGlobalTag(gt) << std::endl;
}

void createPayloadType(char *argv[]) {
  std::string pt = argv[2];
  std::cout << nopayloadclient::createPayloadType(pt) << std::endl;
}

void lockGlobalTag(char *argv[]) {
  std::string gt = argv[2];
  std::cout << nopayloadclient::lockGlobalTag(gt) << std::endl;
}

void unlockGlobalTag(char *argv[]) {
  std::string gt = argv[2];
  std::cout << nopayloadclient::unlockGlobalTag(gt) << std::endl;
}

void deleteGlobalTag(char *argv[]) {
  std::string gt = argv[2];
  std::cout << nopayloadclient::deleteGlobalTag(gt) << std::endl;
}

void insertPayload(int argc, char *argv[]) {
  std::string gt = argv[2];
  std::string pt = argv[3];
  std::string file_url = argv[4];
  long long major_iov_start = std::atoi(argv[5]);
  long long minor_iov_start = std::atoi(argv[6]);
  if (argc == 7) {
      std::cout << nopayloadclient::insertPayload(gt, pt, file_url, major_iov_start,
       minor_iov_start) << std::endl;
  }
  else if (argc == 9) {
      long long major_iov_end = std::atoi(argv[7]);
      long long minor_iov_end = std::atoi(argv[8]);
      std::cout << nopayloadclient::insertPayload(gt, pt, file_url, major_iov_start,
       minor_iov_start, major_iov_end, minor_iov_end) << std::endl;
  }
  else {
      std::cout << "insertPayload takes 5 or 7 arguments (" << argc-2 << " were given)." << std::endl;
  }
}


int main(int argc, char *argv[])
{
  std::string command = argv[1];
  //std::cout << "command = " << command << std::endl;
  if (command == "get") {
      get(argv);
  }
  else if (command == "createGlobalTag") {
      createGlobalTag(argv);
  }
  else if (command == "createPayloadType") {
      createPayloadType(argv);
  }
  else if (command == "lockGlobalTag") {
      lockGlobalTag(argv);
  }
  else if (command == "unlockGlobalTag") {
      unlockGlobalTag(argv);
  }
  else if (command == "insertPayload") {
      insertPayload(argc, argv);
  }
  else if (command == "deleteGlobalTag") {
      deleteGlobalTag(argv);
  }
  else if (command == "getSize") {
      std::cout << nopayloadclient::getSize() << std::endl;
  }
  else if (command == "getGlobalTags") {
      std::cout << nopayloadclient::getGlobalTags() << std::endl;
  }
  else if (command == "getPayloadTypes") {
      std::cout << nopayloadclient::getPayloadTypes() << std::endl;
  }
  else if (command == "checkConnection") {
      std::cout << nopayloadclient::checkConnection() << std::endl;
  }
  else {
      std::cout << "unknown command <" << command << ">. (pick from [get, createGlobalTag, createPayloadType, lockGlobalTag, unlockGlobalTag, insertPayload, deleteGlobalTag, checkConnection]" << std::endl;
  }
  return 0;
}
