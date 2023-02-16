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
    void insert(std::string s1, T f1){
        m1.insert(std::make_pair(s1, (voidFunctionType)f1));
    }

    template<typename... Args>
    json searchAndCall(std::string s1, Args&&... args){
        if (!m1.count(s1)) {
            return BaseException("command <" + s1 + "> not found").jsonify();
        }
        auto mapVal = m1[s1];
        auto typeCastedFun = (json (CLI::*)(Args...))mapVal;
        return (this->*typeCastedFun)(std::forward<Args>(args)...);
    }

private:
    std::map<std::string, voidFunctionType> m1;

};

}
