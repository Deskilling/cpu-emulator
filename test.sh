#!/bin/sh

make

YELLOW='\033[1;33m'
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

find test -type f -name '*.hex' | while read -r hex; do
    dir=$(dirname "$hex")
    base=$(basename "$hex" .hex)

    mem="$dir/$base.mem"
    reg="$dir/$base.reg"

    [ -f "$mem" ] || { echo "$YELLOW missing .mem:$NC $base "; continue; }
    [ -f "$reg" ] || { echo "$YELLOW missing .reg:$NC $base "; continue; }

    ./build/cpu-emulator "$hex"

    status="$GREEN passed:$NC"

    diff -i ".mem_dump.mem" "$mem" >/dev/null 2>&1 || status="$RED failed:$NC"
    diff -i ".reg_dump.reg" "$reg" >/dev/null 2>&1 || status="$RED failed:$NC"

    echo "$status $base"
done
