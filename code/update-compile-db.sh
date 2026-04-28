#!/usr/bin/env bash
# update-compile-db.sh
jq -s 'add' \
  pc/build/compile_commands.json \
  pico/drone/build/compile_commands.json \
  pico/transceiver/build/compile_commands.json \
  > compile_commands.json
