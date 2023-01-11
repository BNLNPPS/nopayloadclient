#include <iostream>
#include <nopayloadclient.hpp>


int main () {
    std::cout << "hello, world!" << std::endl;
    //nopayloadclient::createGlobalTag("sPHENIX_ExampleGT_1");
    nopayloadclient::sayHello();
    //std::cout << nopayloadclient::createGlobalTag("dingeling") << std::endl;
    return 0;
}
