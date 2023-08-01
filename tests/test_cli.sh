#!/bin/bash

run() {
  resp="$($CLI_PATH $1)"
}

assert_resp_contains() {
  if [[ "$resp" != *"$1"* ]]; then
    exit 1
  fi
}

# set config file
export NOPAYLOADCLIENT_CONF=$2

# path to the CLI binary
CLI_PATH=$1

run "checkConnection"
assert_resp_contains '"code": 0'

run "createGlobalTag cli_gt"

run "getGlobalTags"
assert_resp_contains '"name": "cli_gt"'

run "createPayloadType cli_pt"

run "getPayloadTypes"
assert_resp_contains '"name": "cli_pt"'

run "insertPayload cli_gt cli_pt /tmp/file.dat 7 0"
assert_resp_contains '"code": 0'

run "getUrlDict cli_gt 11 0"
assert_resp_contains '"cli_pt":'

exit 0
