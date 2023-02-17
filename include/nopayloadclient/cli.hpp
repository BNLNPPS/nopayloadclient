#pragma once

#include <iostream>
#include <map>
#include <nopayloadclient/nopayloadclient.hpp>
#include <nopayloadclient/exception.hpp>

#include <functional>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <cassert>
#include <tuple>


namespace nopayloadclient {


class CLI {
typedef json (CLI::*voidFunctionType) ();
public:
    CLI();
    virtual json getSize(Client& c, int& argc, char* argv[]);
    virtual json getConfDict(Client& c, int& argc, char* argv[]);
    virtual json getPayloadTypes(Client& c, int& argc, char* argv[]);
    virtual json getGlobalTags(Client& c, int& argc, char* argv[]);
    virtual json checkConnection(Client& c, int& argc, char* argv[]);
    virtual json createPayloadType(Client& c, int& argc, char* argv[]);
    virtual json createGlobalTag(Client& c, int& argc, char* argv[]);
    virtual json deleteGlobalTag(Client& c, int& argc, char* argv[]);
    virtual json lockGlobalTag(Client& c, int& argc, char* argv[]);
    virtual json unlockGlobalTag(Client& c, int& argc, char* argv[]);
    virtual json getUrlDict(Client& c, int& argc, char* argv[]);
    virtual json insertPayload(Client& c, int& argc, char* argv[]);

    template<typename T>
    void insertCommand(std::string name, T func){
        command_map_[name] = (voidFunctionType)func;
    }

    template<typename... Args>
    json callCommand(std::string name, Args&&... args){
        if (!command_map_.count(name)) {
            return BaseException("command <" + name + "> not found").jsonify();
        }
        auto func = command_map_[name];
        auto casted_func = (json (CLI::*)(Args...))func;
        return (this->*casted_func)(std::forward<Args>(args)...);
    }

private:
    std::map<std::string, voidFunctionType> command_map_;

};

}
