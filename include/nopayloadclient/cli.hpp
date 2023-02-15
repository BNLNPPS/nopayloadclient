#include <iostream>
#include <map>
#include <nopayloadclient/nopayloadclient.hpp>

namespace nopayloadclient {

class CLI {
public:
    CLI();
    void conduct(Client& client, int argc, char *argv[]);

private:
    std::map<std::string, void (*)(Client& client, int argc, char *argv[])> command_map_;
    static void sayHello(Client& client, int argc, char *argv[]);
    static void getSize(Client& client, int argc, char *argv[]);
    static void getGlobalTags(Client& client, int argc, char *argv[]);
    static void getPayloadTypes(Client& client, int argc, char *argv[]);
    static void checkConnection(Client& client, int argc, char *argv[]);
    static void getUrlDict(Client& client, int argc, char *argv[]);
    static void createGlobalTag(Client& client, int argc, char *argv[]);
    static void createPayloadType(Client& client, int argc, char *argv[]);
    static void lockGlobalTag(Client& client, int argc, char *argv[]);
    static void unlockGlobalTag(Client& client, int argc, char *argv[]);
    static void deleteGlobalTag(Client& client, int argc, char *argv[]);
    static void insertPayload(Client& client, int argc, char *argv[]);
};

}
