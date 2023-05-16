#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient/nopayloadclient.hpp>


int main () {

    nopayloadclient::Client client("linos_debbuging_gt");
    /*
    std::cout << client.createGlobalTag() << std::endl;
    std::cout << client.createPayloadType("pt_1") << std::endl;;

    std::cout << client.insertPayload("pt_1", "file_1.txt", 1, 1) << std::endl;
    std::cout << client.insertPayload("pt_1", "file_2.txt", 2, 2) << std::endl;
    */
    std::cout << client.getPayloadIOVs(1, 0) << std::endl;
    std::cout << client.getPayloadIOVs(2, 0) << std::endl;
    std::cout << client.getPayloadIOVs(3, 0) << std::endl;

    return 0;

}
