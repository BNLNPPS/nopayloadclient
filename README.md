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
In the project folder, configure cmake and compile
```
cmake -S . -B build && cmake --build build/
```
Configure the client by specifying the config file in
an env variable:
```
export NOPAYLOADCLIENT_CONF="/absolute/path/to/config.json"
```
If this is not done, the parameters in config/default.json will
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
cmake -S . -B build && cmake --build build/ && build/examples/example && cd ..
```