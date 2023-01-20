## Welcome to nopayloadclient!
### Table of contents
* [Introduction](#introduction)
* [Setup](#setup)
* [Testing](#testing)
* [Usage](#usage)

### Introduction
This client-side library is meant to communicate with
NoPayloadDB (https://github.com/bnlnpps/nopayloaddb).

### Setup
Define where ```nopayloadclient``` will be installed 
```shell
source setup_custom_install_path.sh /absolute/install/path
```
If this step is omitted, the default install path (```/usr/local/```) will be assumed.

In the project folder, configure cmake and compile
```shell
cmake -S . -B build && cmake --build build/
```
Install the client
```shell
cmake --install build/
```
Configure the client by specifying the config file in
an env variable
```
export NOPAYLOADCLIENT_CONF=/path/to/config.json
```
If this is not done, the parameters in ```config/default.json``` will
be assumed.

### Testing
The following command activates testing, reconfigures cmake, recompiles the code
and runs the tests defined in the ```tests/``` directory
```
cmake -DBUILD_TESTING=ON -S . -B build && cmake --build build/ && cd build/ && ctest --verbose && cd ..
```

### Usage
To use this library in an independent project, include its
header and the widely-spread nlohmann json header-only lib
(https://github.com/nlohmann/json)
```c
#include <nlohmann/json.hpp>
#include <nopayloadclient.hpp>
```
Payloads can be inserted via
```c
nopayloadclient::insertPayload(<gt_name>, <gt_type>, <local_url>, <major_iov>, <minor_iov>);
```
and their url can be retrieved via
```c
nopayloadclient::get(<gt_name>, <gt_type>, <major_iov>, <minor_iov>);
```
Every method returns a nlohmann::json object with a response code and a
message body in the  following format:
```
{'code': <code>, 'msg': <message>}
```
Currently, the following response codes are implemented:
```
0: ok
1: base error (no further details)
2: error when handling a payload
3: error in the database (server-side)
```
An example on how to use this tool in a c++ program can be found in
```examples/standalone.cpp```. It can be compiled via
```
g++ -std=c++17 standalone.cpp -lnopayloadclient -lcurl
```


#### Usage through command line interface
```examples/cli_npc.cpp``` is an implementation of a command line interface.
It is compiled along the rest of the project and can be run via
```shell
cli_npc <command> <parameters>
```
The available commands are: get, createGlobalTag, createPayloadType, lockGlobalTag,
unlockGlobalTag, deleteGlobalTag, insertPayload (overloaded), getSize,
getPayloadTypes, getGlobalTags. Example workflow:
```shell
export NOPAYLOADCLIENT_CONF=/path/to/config.json
cli_npc checkConnection
cli_npc createGlobalTag example_gt
cli_npc getGlobalTags
cli_npc createPayloadType example_pt
cli_npc getPayloadTypes
cli_npc insertPayload example_gt example_pt /tmp/file.dat 7 0
cli_npc get example_gt example_pt 11 0
cli_npc getSize
```
