#include <nopayloadclient/cli.hpp>

namespace nopayloadclient {

template <class T>
CLI<T>::CLI() {
    command_map_["sayHello"] = &CLI<T>::sayHello;
    command_map_["getSize"] = &CLI<T>::getSize;
    command_map_["getGlobalTags"] = &CLI<T>::getGlobalTags;
    command_map_["getPayloadTypes"] = &CLI<T>::getPayloadTypes;
    command_map_["checkConnection"] = &CLI<T>::checkConnection;
    command_map_["getUrlDict"] = &CLI<T>::getUrlDict;
    command_map_["createGlobalTag"] = &CLI<T>::createGlobalTag;
    command_map_["createPayloadType"] = &CLI<T>::createPayloadType;
    command_map_["lockGlobalTag"] = &CLI<T>::lockGlobalTag;
    command_map_["unlockGlobalTag"] = &CLI<T>::unlockGlobalTag;
    command_map_["deleteGlobalTag"] = &CLI<T>::deleteGlobalTag;
    command_map_["insertPayload"] = &CLI<T>::insertPayload;
}

template <class T>
void CLI<T>::conduct(T& client, int argc, char *argv[]) {
    std::string command = argv[1];
    if (!command_map_.count(command)) {
        std::cout << "command " << command << " not found." << std::endl;
        return;
    }
    command_map_[command](client, argc, argv);
}

template <class T>
void CLI<T>::sayHello(T& client, int argc, char *argv[]) {
    std::cout << "sayHello(argv=" << argv << ")" << std::endl;
    std::cout << "hello" << std::endl;
}

template <class T>
void CLI<T>::getSize(T& client, int argc, char *argv[]) {
    std::cout << "nopayloadclient::CLI<T>::getSize()" << std::endl;
    std::cout << "typeid(client).name = " << typeid(client).name() << std::endl;
    std::cout << client.getSize() << std::endl;
}

template <class T>
void CLI<T>::getGlobalTags(T& client, int argc, char *argv[]) {
    std::cout << client.getGlobalTags() << std::endl;
}

template <class T>
void CLI<T>::getPayloadTypes(T& client, int argc, char *argv[]) {
    std::cout << client.getPayloadTypes() << std::endl;
}

template <class T>
void CLI<T>::checkConnection(T& client, int argc, char *argv[]) {
    std::cout << client.checkConnection() << std::endl;
}

template <class T>
void CLI<T>::getUrlDict(T& client, int argc, char *argv[]){
    std::string global_tag = argv[2];
    long long major_iov = std::atoi(argv[3]);
    long long minor_iov = std::atoi(argv[4]);
    client.setGlobalTag(global_tag);
    std::cout << client.getUrlDict(major_iov, minor_iov) << std::endl;
}

template <class T>
void CLI<T>::createGlobalTag(T& client, int argc, char *argv[]) {
    std::string global_tag = argv[2];
    client.setGlobalTag(global_tag);
    std::cout << client.createGlobalTag() << std::endl;
}

template <class T>
void CLI<T>::createPayloadType(T& client, int argc, char *argv[]) {
    std::string pt = argv[2];
    std::cout << client.createPayloadType(pt) << std::endl;
}

template <class T>
void CLI<T>::lockGlobalTag(T& client, int argc, char *argv[]) {
    std::string global_tag = argv[2];
    client.setGlobalTag(global_tag);
    std::cout << client.lockGlobalTag() << std::endl;
}

template <class T>
void CLI<T>::unlockGlobalTag(T& client, int argc, char *argv[]) {
    std::string global_tag = argv[2];
    client.setGlobalTag(global_tag);
    std::cout << client.unlockGlobalTag() << std::endl;
}

template <class T>
void CLI<T>::deleteGlobalTag(T& client, int argc, char *argv[]) {
    std::string global_tag = argv[2];
    client.setGlobalTag(global_tag);
    std::cout << client.deleteGlobalTag() << std::endl;
}

template <class T>
void CLI<T>::insertPayload(T& client, int argc, char *argv[]) {
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
    nopayloadclient::CLI<nopayloadclient::Client> cli;
    cli.conduct(client, argc, argv);
    return 0;
}
