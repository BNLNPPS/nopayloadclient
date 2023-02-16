#include <dunenpc/cli.hpp>
#include <functional>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <cassert>
#include <tuple>

namespace dunenpc {

void CLI::conduct(Client& client, int argc, char *argv[]) {
    std::string command = argv[1];
    if (!command_map_.count(command)) {
        std::cout << "command " << command << " not found." << std::endl;
        return;
    }
    command_map_[command](client, argc, argv);
}

void CLI::getUrlDict(Client& client, int argc, char *argv[]){
    std::cout << "dunenpc::CLI::getUrlDict()" << std::endl;
    std::string global_tag = argv[2];
    long long run_number = std::atoi(argv[3]);
    client.setGlobalTag(global_tag);
    std::cout << client.getUrlDict(run_number) << std::endl;
}

}


void fun1(void){
    std::cout<<"inside fun1\n";
}

int fun2(){
    std::cout<<"inside fun2\n";
    return 2;
}

int fun3(int a){
    std::cout<<"inside fun3(a="<<a<<")\n";
    return a;
}

std::vector<int> fun4(){
    std::cout<<"inside fun4\n";
    std::vector<int> v(4,100);
    return v;
}

class A {
public:
    virtual int print(int a) {
        std::cout << "A::print(a=" << a << ")" << std::endl;
        return a;
    }
    virtual int add(int a, int b) {
        std::cout << "A::add(a=" << a << ", b=" << b << ")" << std::endl;
        return a+b;
    }
};

class B : public A {
public:
    int print(int a) {
        std::cout << "B: a = " << a << std::endl;
        return a;
    }
    virtual int add(int a, int b) {
        std::cout << "B::add(a=" << a << ", b=" << b << ")" << std::endl;
        return a+b;
    }
};

// every function pointer will be stored as this type
//typedef int (*voidFunctionType)(void);
//typedef json (nopayloadclient::Client::*voidFunctionType) ();
typedef int (A::*voidFunctionType) ();

struct Interface{

    std::map<std::string, voidFunctionType> m1;

    template<typename T>
    void insert(std::string s1, T f1){
        std::cout << "Interface.insert()" << std::endl;
        m1.insert(std::make_pair(s1, (voidFunctionType)f1));
    }

    //template<typename... Args>
    auto searchAndCall(std::string s1){
        auto mapIter = m1.find(s1);
        /*chk if not end*/
        auto mapVal = mapIter->second;

        // auto typeCastedFun = reinterpret_cast<T(*)(Args ...)>(mapVal.first);
        return mapVal;
        //auto typeCastedFun = (int(*)(Args ...))(mapVal);
    }

    template<typename... Args>
    auto searchAndCall2(A *obj, std::string s1, Args&&... args){
        auto mapIter = m1.find(s1);
        /*chk if not end*/
        auto mapVal = mapIter->second;

        // auto typeCastedFun = reinterpret_cast<T(*)(Args ...)>(mapVal.first);
        auto typeCastedFun = (int (A::*)(Args...))mapVal;
        return (obj->*typeCastedFun)(std::forward<Args>(args)...);
    }

};


int main(int argc, char *argv[])
{
    Interface a1;
    //a1.insert("fun1",fun1);
    //a1.insert("fun2",fun2);
    //a1.insert("fun3",fun3);
    a1.insert("fun69", &A::print);
    a1.insert("add", &A::add);
    //a1.insert("fun70", &nopayloadclient::Client::getSize);
    //a1.insert("fun71", &nopayloadclient::Client::setGlobalTag);
    //a1.insert("fun4",fun4);

    //auto typeCastedFun = (int(*)(int))(a1.searchAndCall("fun3"));
    //int retVal = typeCastedFun(2);

    //auto typeCastedFun2 = (int(*)(int))(a1.searchAndCall("fun69"));
                        // (A::*voidFunctionType)
    auto typeCastedFun2 = (int (A::*)(int))a1.searchAndCall("fun69");
    A *a = new A();
    int retVal = (a->*typeCastedFun2)(2);

    B *b = new B();
    int retVal2 = (b->*typeCastedFun2)(2);

    std::cout << a1.searchAndCall2(a, "fun69", 2) << std::endl;
    std::cout << a1.searchAndCall2(b, "fun69", 2) << std::endl;

    std::cout << a1.searchAndCall2(a, "add", 2, 3) << std::endl;
    std::cout << a1.searchAndCall2(b, "add", 2, 3) << std::endl;

    //a1.searchAndCall<void>("fun1");
    //int retVal = a1.searchAndCall("fun3",2);
    //std::cout << "retVal = " << retVal << std::endl;
    //a1.searchAndCall("fun2");
    //auto temp = a1.searchAndCall<std::vector<int>>("fun4");

    /*

    nopayloadclient::Client *obj = new nopayloadclient::Client();
    dunenpc::Client *obj2 = new dunenpc::Client();

    Interface a1;
    a1.insert("getSize", &nopayloadclient::Client::getSize);
    a1.insert("getGlobalTag", &nopayloadclient::Client::getGlobalTag);
    //std::cout << (obj->*a1.get)("getSize") << std::endl;

*/

/*
    typedef json (nopayloadclient::Client::*MethodPtr) ();
    typedef std::unordered_map<std::string, MethodPtr> script_map;
    script_map m;

    typedef json (nopayloadclient::Client::*MethodPtr2) (std::string);
    typedef std::unordered_map<std::string, MethodPtr2> script_map2;
    script_map2 m2;

    m.emplace("getSize", &nopayloadclient::Client::getSize);
    m.emplace("checkConnection", &nopayloadclient::Client::checkConnection);
    m2.emplace("setGlobalTag", &nopayloadclient::Client::setGlobalTag);
    //m.emplace("getUrlDict", &nopayloadclient::Client::getUrlDict);
    //m.emplace("getUrlDict", &nopayloadclient::Client::getUrlDict);
    //MethodPtr method = &nopayloadclient::Client::getSize;
//    typedef json (nopayloadclient::Client::*MethodPtr) ();
//    MethodPtr method = &nopayloadclient::Client::getSize;
//    json (nopayloadclient::Client::*method) () = &nopayloadclient::Client::getSize;
    nopayloadclient::Client *obj = new nopayloadclient::Client();
    dunenpc::Client *obj2 = new dunenpc::Client();

    //auto iter = m.find("getSize");
    //std::cout << (obj->*iter->second)() << std::endl;


    std::cout << (obj->*m["getSize"])() << std::endl;
    //std::cout << (obj->*method)() << std::endl;
    std::cout << obj->getSize() << std::endl;

    std::cout << (obj2->*m["getSize"])() << std::endl;
    //std::cout << (obj2->*method)() << std::endl;
    std::cout << obj2->getSize() << std::endl;

//    std::cout << "main(argc=" << argc <<", argv=" << argv << ")" << std::endl;
//    dunenpc::Client dune_client;
//    dunenpc::CLI cli;
//    cli.conduct(dune_client, argc, argv);
*/

    return 0;

}
