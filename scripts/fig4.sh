#!/usr/bin/env bash

source "$(dirname "$0")/common.sh"

mkdir -p figures

$PLOTPY "${COMMON_ARGS[@]}" --cols 1 2 3 4 5 6 --title 'Figure 4: First Radix Sort'
