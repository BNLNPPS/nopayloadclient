## Welcome to nopayloadclient!
### Table of contents
* [Introduction](#introduction)
* [Setup](#setup)
* [Testing](#testing)
* [Usage](#usage)
* [Configuration](#configuration)

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
Firstly, create an instance of the client and configure it for a global tag
```c
nopayloadclient::Client client;
client.setGlobalTag(<gt_name>);
```
Payloads can be inserted via
```c
client.insertPayload(<pl_type>, <local_url>, <major_iov>, <minor_iov>);
```
and a dictionary mapping each payload type to one url can be retrieved via
```c
client.getUrlDict(<major_iov>, <minor_iov>);
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
g++ -std=c++14 standalone.cpp -lnopayloadclient -lcurl
```

#### Usage through command line interface
```src/cli.cpp``` is an implementation of a command line interface.
It is compiled along the rest of the project and can be run via
```shell
cli_npc <command> <parameters>
```
Some available commands are: get, createGlobalTag, createPayloadType, lockGlobalTag,
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
cli_npc getUrlDict example_gt 11 0
cli_npc getSize
```

### Configuration
Configuration of the client happens via a ```.json``` file.
Examples can be found in the ```config/``` dir. The default
parameters are defined in ```config/default.json```:
```json
{
  "base_url": "localhost:8000",
  "api_res": "/api/cdb_rest/",
  "write_dir": "/tmp/remote_pl_store/",
  "read_dir_list": ["/tmp/remote_pl_store/", "/cvmfs/place/holder/"],
  "n_retries": 5,
  "cache_life_time": 10,
  "cache_max_mb": 1,
  "print_time_stamps": false,
  "use_fake_backend": false
}
```
`base_url` and `api_res` together define the url to the rest api
of the back end. The example above assumes that the service can be
reached under `localhost:8000/api/cdb_rest/`.

`write_dir` is where the payload files are copied to. To optimise the
performance of the file system, subdirectories are created based on the 
checksum of files. The naming is not meant to be human-readable.

`read_dir_list` defines a list of directories that the client will use to
find a payload file. The directories are checked in the defined order and the
complete path of the first payload file found will be used as the payload url.
In the simplest case, `write_dir` would be the only entry in `read_dir_list`.

`n_retries` specifies the number of retries to contact the rest api before
throwing an error. The waiting time before each retry grows exponentially.

`cache_life_time` is the lifetime of each entry in the cache. Set to `-1`
for infinite lifetime.

`cache_max_mb` is the maximum (approximated) size in mb that the cache will occupy.
The oldest entry will be removed until the cache is below the limit again. Set to `-1`
for unlimited size.

`print_time_stamps`: if set to `true`, print a time stamp before and after
calling curl. Mainly used for performance evaluation.

`use_fake_backend`: if set to `true`, the client will not contact the actual rest api
but instead simulate its response assuming a single, minimal global tag. This is useful
when testing the client's interface (e.g. when integrating into an existing software framework)
in an environment where the backend is not reachable. Allows only read operations. Some details on the simulated DB content:
One global tag `ExampleGT`, two payload lists `ExamplePT1` and `ExamplePT2` with one IOV each that is valid after `major_iov=42`
and `minor_iov=42`. The inserted payload files where called `example_file.dat` and contained
the string `data`.