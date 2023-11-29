#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient/nopayloadclient.hpp>


int main () {

    nopayloadclient::NoPayloadClient client("ExampleGT");

    std::cout << client.provokeTimeOut() << std::endl;

    return 0;

}
