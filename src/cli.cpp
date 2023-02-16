#include <nopayloadclient/cli.hpp>

namespace nopayloadclient {

CLI::CLI() {
    command_map_["getSize"] = &CLI::getSize;
    command_map_["getGlobalTags"] = &CLI::getGlobalTags;
    command_map_["getPayloadTypes"] = &CLI::getPayloadTypes;
    command_map_["checkConnection"] = &CLI::checkConnection;
    command_map_["getUrlDict"] = &CLI::getUrlDict;
    command_map_["createGlobalTag"] = &CLI::createGlobalTag;
    command_map_["createPayloadType"] = &CLI::createPayloadType;
    command_map_["lockGlobalTag"] = &CLI::lockGlobalTag;
    command_map_["unlockGlobalTag"] = &CLI::unlockGlobalTag;
    command_map_["deleteGlobalTag"] = &CLI::deleteGlobalTag;
    command_map_["insertPayload"] = &CLI::insertPayload;
}


void CLI::conduct(Client& client, int argc, char *argv[]) {
    std::string command = argv[1];
    if (!command_map_.count(command)) {
        std::cout << "command " << command << " not found." << std::endl;
        return;
    }
    command_map_[command](client, argc, argv);
}


void CLI::getSize(Client& client, int argc, char *argv[]) {
    std::cout << "nopayloadclient::CLI::getSize()" << std::endl;
    std::cout << "typeid(client).name = " << typeid(client).name() << std::endl;
    std::cout << client.getSize() << std::endl;
}


void CLI::getGlobalTags(Client& client, int argc, char *argv[]) {
    std::cout << client.getGlobalTags() << std::endl;
}


void CLI::getPayloadTypes(Client& client, int argc, char *argv[]) {
    std::cout << client.getPayloadTypes() << std::endl;
}


void CLI::checkConnection(Client& client, int argc, char *argv[]) {
    std::cout << client.checkConnection() << std::endl;
}


void CLI::getUrlDict(Client& client, int argc, char *argv[]){
    std::string global_tag = argv[2];
    long long major_iov = std::atoi(argv[3]);
    long long minor_iov = std::atoi(argv[4]);
    client.setGlobalTag(global_tag);
    std::cout << client.getUrlDict(major_iov, minor_iov) << std::endl;
}


void CLI::createGlobalTag(Client& client, int argc, char *argv[]) {
    std::string global_tag = argv[2];
    client.setGlobalTag(global_tag);
    std::cout << client.createGlobalTag() << std::endl;
}


void CLI::createPayloadType(Client& client, int argc, char *argv[]) {
    std::string pt = argv[2];
    std::cout << client.createPayloadType(pt) << std::endl;
}


void CLI::lockGlobalTag(Client& client, int argc, char *argv[]) {
    std::string global_tag = argv[2];
    client.setGlobalTag(global_tag);
    std::cout << client.lockGlobalTag() << std::endl;
}


void CLI::unlockGlobalTag(Client& client, int argc, char *argv[]) {
    std::string global_tag = argv[2];
    client.setGlobalTag(global_tag);
    std::cout << client.unlockGlobalTag() << std::endl;
}


void CLI::deleteGlobalTag(Client& client, int argc, char *argv[]) {
    std::string global_tag = argv[2];
    client.setGlobalTag(global_tag);
    std::cout << client.deleteGlobalTag() << std::endl;
}


void CLI::insertPayload(Client& client, int argc, char *argv[]) {
    std::string global_tag = argv[2];
    std::string pt = argv[3];
    std::string file_url = argv[4];
    long long major_iov_start = std::atoi(argv[5]);
    long long minor_iov_start = std::atoi(argv[6]);
    client.setGlobalTag(global_tag);
    if (argc == 7) {
      std::cout << client.insertPayload(pt, file_url, major_iov_start, minor_iov_start) << std::endl;
    }
    else if (argc == 9) {
      long long major_iov_end = std::atoi(argv[7]);
      long long minor_iov_end = std::atoi(argv[8]);
      std::cout << client.insertPayload(pt, file_url, major_iov_start, minor_iov_start,
                                        major_iov_end, minor_iov_end) << std::endl;
    }
    else {
      std::cout << "insertPayload takes 5 or 7 arguments (" << argc-2 << " were given)." << std::endl;
    }
}

}

int main(int argc, char *argv[])
{
    nopayloadclient::Client client;
    nopayloadclient::CLI cli;
    cli.conduct(client, argc, argv);
    return 0;
}
