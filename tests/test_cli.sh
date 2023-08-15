#!/bin/bash

run() {
  echo "running <$1>"
  resp="$($CLI_PATH $1)"
  echo "$resp"
}

assert_resp_contains() {
  if [[ "$resp" != *"$1"* ]]; then
    exit 1
  fi
}

assert_resp_contains_not() {
  if [[ "$resp" == *"$1"* ]]; then
    exit 1
  fi
}

# set config file
export NOPAYLOADCLIENT_CONF=$2

# path to the CLI binary
CLI_PATH=$1

run "checkConnection"
assert_resp_contains '"code": 0'

run "deleteGlobalTag cli_gt"
run "createGlobalTag cli_gt"

run "getGlobalTags"
assert_resp_contains '"name": "cli_gt"'

run "createPayloadType cli_pt"

run "getPayloadTypes"
assert_resp_contains '"name": "cli_pt"'

run "insertPayload cli_gt cli_pt /tmp/file.dat 7 0 42 69"
assert_resp_contains '"code": 0'

run "getUrlDict cli_gt 11 0"
assert_resp_contains '"cli_pt":'

run "deletePayloadType cli_pt"
assert_resp_contains_not '"code": 0'

run "deletePayloadIOV cli_gt cli_pt 7 0 42 69"
assert_resp_contains '"code": 0'

run "deletePayloadType cli_pt"
assert_resp_contains '"code": 0'

run "lockGlobalTag cli_gt"
assert_resp_contains '"code": 0'

run "deleteGlobalTag cli_gt"
assert_resp_contains_not '"code": 0'

run "unlockGlobalTag cli_gt"
assert_resp_contains '"code": 0'

run "deleteGlobalTag cli_gt"
assert_resp_contains '"code": 0'

exit 0
