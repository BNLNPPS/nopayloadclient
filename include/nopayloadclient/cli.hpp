#include <iostream>
#include <map>
#include <nopayloadclient/nopayloadclient.hpp>

namespace nopayloadclient {

template <class T>
class CLI {
public:
    CLI();
    void conduct(T& client, int argc, char *argv[]);

private:
    std::map<std::string, void (*)(T& client, int argc, char *argv[])> command_map_;
    static void sayHello(T& client, int argc, char *argv[]);
    static void getSize(T& client, int argc, char *argv[]);
    static void getGlobalTags(T& client, int argc, char *argv[]);
    static void getPayloadTypes(T& client, int argc, char *argv[]);
    static void checkConnection(T& client, int argc, char *argv[]);
    static void getUrlDict(T& client, int argc, char *argv[]);
    static void createGlobalTag(T& client, int argc, char *argv[]);
    static void createPayloadType(T& client, int argc, char *argv[]);
    static void lockGlobalTag(T& client, int argc, char *argv[]);
    static void unlockGlobalTag(T& client, int argc, char *argv[]);
    static void deleteGlobalTag(T& client, int argc, char *argv[]);
    static void insertPayload(T& client, int argc, char *argv[]);
};

}
