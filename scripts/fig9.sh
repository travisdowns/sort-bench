#!/usr/bin/env bash

source "$(dirname "$0")/common.sh"

mkdir -p figures

$PLOTPY "${COMMON_ARGS[@]}" --ylim 0 140 --cols 1 3 4 6 7 8 9 10 11 --title 'Figure 9: Radix6 skipping trivial passes'
