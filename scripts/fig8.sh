#!/usr/bin/env bash

source "$(dirname "$0")/common.sh"

mkdir -p figures

$PLOTPY "${COMMON_ARGS[@]}" --cols 1 3 4 6 7 8 9 10 --title 'Figure 8: Radix5 with buffer swapping'
