## Welcome to nopayloadclient!
### Table of contents
* [Introduction](#introduction)
* [Setup](#setup)
* [Testing](#testing)
* [Usage](#usage)
* [SPHENIX use case](#sphenix)

### Introduction
This client-side library is meant to communicate with
NoPayloadDB (https://github.com/bnlnpps/nopayloaddb).

### Setup
In the project folder, configure cmake and compile
```
cmake -S . -B build && cmake --build build/
```
Configure the client by specifying the config file in
an env variable
```
export NOPAYLOADCLIENT_CONF=example_config.json
```
where only files in the ```config/``` directory are considered.
If this is not done, the parameters in ```default.json``` will
be assumed.

### Testing
Go to build/ directory and run the tests (with verbose output)
```
cd build/
ctest --verbose
```
You can specify which tests to run at the very bottom of
```tests/CMakeLists.txt```. Don't forget to recompile before
running the tests again. The following command does everything
in one step:
```
cmake -S . -B build && cmake --build build/ && cd build/ && ctest --verbose && cd ..
```

### Usage
To use this library in an independent project, include its
header and the widely-spread nlohmann json header-only lib
(https://github.com/nlohmann/json)
```c
#include <nlohmann/json.hpp>
#include "nopayloadclient.hpp"
```
Payloads can be inserted via
```c
nopayloadclient::insertPayload(<gtName>, <gtType>, <localUrl>, <majorIov>, <minorIov>);
```
and their url can be retrieved via
```c
nopayloadclient::get(<gtName>, <gtType>, <majorIov>, <minorIov>);
```
Every method returns a nlohmann::json object with a response code and a
message body in the  following format:
```
{'code': <code>, 'msg': <message>}
```
Currently, the following response codes are implemented:
```
0: ok
1: error
```
Examples on how to use this tool in a c++ program can be found in the ```examples/```
folder. The following command compiles and runs them:
```
cmake -S . -B build && cmake --build build/ && build/examples/example_insert_short && cd ..
```

### Sphenix use case
An experiment-specific version of this client has been developed for the sphenix use case.
It's simplified API can be found in ```include/sphenixclient.hpp```. It drops the major-
and minorIOV in favor of a single iov parameter.

#### Usage within a c++ program
An example of how to use this client in a c++ program can be found in
```examples/example_sphenix.cpp```.

#### Usage through command line interface
```examples/cli_sphenix.cpp``` is an implementation of a command line interface for the
sphenix client. It is compiled along the rest of the project and can be run via 
```shell
./build/examples/cli_sphenix <command> <parameters>
```
The available commands are: get, createGlobalTag, createPayloadType, lockGlobalTag,
unlockGlobalTag, deleteGlobalTag, insertPayload (overloaded). Example workflow:
```shell
export NOPAYLOADCLIENT_CONF=my_sphenix_conf.json
cd build/examples/
./cli_sphenix createGlobalTag example_gt
./cli_sphenix createPayloadType example_pt
./cli_sphenix insertPayload example_gt example_pt /tmp/file.dat 7
./cli_sphenix get example_gt example_pt 11
```
