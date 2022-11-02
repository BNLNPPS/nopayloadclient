## Welcome to nopayloadclient!
### Table of contents
* [Introduction](#introduction)
* [Setup](#setup)
* [Usage](#usage)

### Introduction
This client-side library is meant to communicate with
NoPayloadDB (https://github.com/bnlnpps/nopayloaddb).

### Setup
Compile
```
cmake -S . -B build && cmake --build build/
```
Configure
```
export NOPAYLOADCLIENT_CONF="/absolute/path/to/config.json"
```
If this is not done, the parameters in config/default.json will
be assumed.

### Usage
Run the tests
```
ctest --verbose
```


Every method returns a nlohmann::json object with a response code and a message body in the
following format:
```
{'code': <code>, 'msg': <message>}
```
Currently, the following response codes are implemented:
```
0: ok
1: error
```
Examples on how to use this tool in a c++ program can be found in the ```examples/``` folder.



```
cmake -S . -B build && cmake --build build/ && cd build/ && ctest --verbose && cd ..
curl -X PUT http://localhost:8000/api/cdb_rest/gt_change_status/linos_object/locked --libcurl code.c
```