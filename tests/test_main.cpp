#define CATCH_CONFIG_MAIN  // This tells Catch2 to provide a main()
#include "catch2/catch.hpp"
#include <nlohmann/json.hpp>
#include <experimental/filesystem>

#include <nopayloadclient/nopayloadclient.hpp>

namespace fs = std::experimental::filesystem::v1;
using Client = nopayloadclient::NoPayloadClient;

template <typename ClientType>
class ModifyEntryFixture {
public:
    ModifyEntryFixture() {
        conf_dict = client.getConfDict()["msg"];
        if (fs::exists(conf_dict["write_dir"])) {
            throw std::runtime_error("Test directory already exists! Aborting tests to prevent data loss.");
        }
        client.createGlobalTag(gt_name);
    }
    ~ModifyEntryFixture() {
        client.deleteGlobalTag(gt_name);
        fs::remove_all(conf_dict["write_dir"]);
    }
    ClientType client;
    nlohmann::json conf_dict;
    std::string gt_name = "test_gt";
};


TEST_CASE_METHOD(ModifyEntryFixture<Client>, "gt creation") {
    nlohmann::json resp = client.getGlobalTags();
    REQUIRE(resp["code"] == 0);
    REQUIRE(nopayloadclient::objWithNameExists(resp["msg"], gt_name));
    std::cout << client.getConfDict() << std::endl;
}


/*

class GlobalTagFixture {
public:
    GlobalTagFixture() {
        // Setup: Create temporary directory.
        tempDirPath = "/path/to/tempDir"; // You can generate a unique name here.
        std::filesystem::create_directories(tempDirPath);
    }

    ~TempDirFixture() {
        // Teardown: Remove temporary directory.
        std::filesystem::remove_all(tempDirPath);
    }

    std::filesystem::path tempDirPath;
};



void commonBaseClassTests(nopayloadclient::NoPayloadClient* instance) {
    REQUIRE(instance->getConfDict()["code"] == 0);
    REQUIRE(instance->checkConnection()["code"] == 0);
}

TEST_CASE("BaseClass Tests") {
    nopayloadclient::NoPayloadClient baseInstance;
    commonBaseClassTests(&baseInstance);
}

//TEST_CASE("DerivedClass Tests") {
//    DerivedClass derivedInstance;
//
//    // Run common tests
//    commonBaseClassTests(&derivedInstance);
//
//    // Additional tests for DerivedClass (if any)
//    // ...
//}
*/