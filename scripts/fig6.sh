#!/usr/bin/env bash

source "$(dirname "$0")/common.sh"

mkdir -p figures

$PLOTPY "${COMMON_ARGS[@]}" --cols 1 3 4 6 7 8 --title 'Figure 6: Radix3 with vector re-use'
