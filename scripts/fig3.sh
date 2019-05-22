#!/usr/bin/env bash

source "$(dirname "$0")/common.sh"

mkdir -p figures

$PLOTPY "${COMMON_ARGS[@]}" --cols 1 2 3 4 5 --title 'Figure 3: C++ Sorts'
