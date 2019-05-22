#!/bin/echo dont-run-this-directly

set -e

echo $0

SCRIPTNAME=$(basename "$0")
SCRIPTDIR=$(dirname "$0")

# call this from the parent directory, not from inside scripts
# use it like script.sh [OUTPUT]
# where OUTPUT is the output filename for the plot, or not filename for an interactive graph
if [ ! -f "$PWD/scripts/$SCRIPTNAME" ]; then
    set +e
    PARENT="$( cd "$(dirname "$0")/.." ; pwd -P )"
    echo "Please run this script from the root project directory: $PARENT"
    exit 1
fi

# set PLOTDIR to specify that plots should be written to svg file in plotdir, otherwise 
# they'll be shown interactively
if [ -z "$FIGDIR" ]; then
    OUT=()
else
    OUTFILE=$(basename -s '.sh' "$0").svg
    OUT=("--out" "$FIGDIR/$OUTFILE")
fi

mkdir -p tmp
TDIR=$(mktemp -d "./tmp/XXXXXXXX")

PLOTPY=./scripts/plot-csv.py

COMMON_ARGS=(data/all.csv --ylim 0 --xcol 0 --ylabel 'nanoseconds per element' --xlabel 'input size' "${OUT[@]}")
