#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient/nopayloadclient.hpp>


int main () {

    nopayloadclient::NoPayloadClient client("ExampleGT");
    std::cout << client.deleteGlobalTag() << std::endl;

    std::cout << client.createGlobalTag() << std::endl;
    std::cout << client.createPayloadType("ExamplePT") << std::endl;;

    nlohmann::json resp;

    resp = client.provokeTimeOut();
    std::cout << resp << std::endl;

    for(int i=0; i<1; i++){
        std::cout << "inserting payload for minor iov " << i << std::endl;
        resp = client.insertPayload("ExamplePT", "data/file_" + std::to_string(i) + ".txt", 0, i);
        if (resp["code"] != 0) {
            std::cout << resp << std::endl;
            exit(1);
        }
        resp = client.getPayloadIOVs(0, i);
        if (resp["msg"]["ExamplePT"]["minor_iov_start"] != i) {
            std::cout << "Read-back minor iov is not equal to last inserted one" << std::endl;
            exit(1);
        }
    }

    return 0;

}
