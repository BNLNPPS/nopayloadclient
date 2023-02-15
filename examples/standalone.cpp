#include <iostream>
#include <nlohmann/json.hpp>
#include <nopayloadclient/nopayloadclient.hpp>


int main () {
    // ++++++++++++++++++++++++++++++
    //         PREPARATION
    // ++++++++++++++++++++++++++++++

    std::cout << "beginning of standalone..." << std::endl;

    // create an instance of the client
    nopayloadclient::Client client {"ExampleGT"};

    std::cout << client.createGlobalTag() << std::endl;
    std::cout << client.createPayloadType("ExamplePT") << std::endl;;

    // create json object to which the response is written
    nlohmann::json resp;

    // ++++++++++++++++++++++++++++++
    //         INSERTION
    // ++++++++++++++++++++++++++++++

    // try inserting payload
    resp = client.insertPayload("ExamplePT", "/tmp/file.dat", 7, 0);

    // analyse response: code==0 -> OK, code!=0 -> Not OK!
    if (resp["code"]!=0) {
      std::cout<<"something went wrong. error message: "<<resp["msg"]<<std::endl;
    }
    else {
      std::cout<<"all good. details: "<<resp["msg"]<<std::endl;
    }

    // w/o error handling, inserting a payload is a one-liner:
    std::cout << client.insertPayload("ExamplePT", "/tmp/file.dat", 8, 0) << std::endl;

    // ++++++++++++++++++++++++++++++
    //          RETRIEVAL
    // ++++++++++++++++++++++++++++++

    resp = client.getUrlDict(42, 0);
    std::cout << resp << std::endl;
    return 0;
}
