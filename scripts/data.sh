#!/usr/bin/env bash

source "$(dirname "$0")/common.sh"

mkdir -p data

./bench > data/all.csv
