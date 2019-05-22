#!/usr/bin/env bash

source "$(dirname "$0")/common.sh"

mkdir -p figures

$PLOTPY "${COMMON_ARGS[@]}" --cols 1 3 4 6 7 --title 'Figure 5: Radix2 with reserve()'
