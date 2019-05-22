#!/usr/bin/env python3

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import csv
import argparse
import sys
import collections
import os
import pdutil


p = argparse.ArgumentParser(usage='plot output from PLOT=1 ./bench')

# input and output configuration
p.add_argument('input', help='CSV file to plot (or stdin)', nargs='*',
    type=argparse.FileType('r'), default=[ sys.stdin ])
p.add_argument('--out', help='output filename')

# column selection and configuration
p.add_argument('--xcol', help='Column index to use as x axis (default: 0)', type=int, default=0)
p.add_argument('--cols',  help='Use only these zero-based columns on primary axis (default: all columns)',
    type=int, nargs='+')
p.add_argument('--cols2', help='Use only these zero-based columns on secondary axis (default: no secondary axis)',
    type=int, nargs='+')

# chart text
p.add_argument('--clabels', help="Comma separated list of column names used as label for data series (default: column header)",
    type=lambda x: x.split(','))
p.add_argument('--title', help='Set chart title', default='Some chart (use --title to specify title)')
p.add_argument('--xlabel', help='Set x axis label')
p.add_argument('--ylabel', help='Set y axis label')
p.add_argument('--suffix-names', help='Suffix each column name with the file it came from', action='store_true')

# axis configuration
p.add_argument('--ylim', help='Set the y axis limits explicitly (e.g., to cross at zero)', type=int, nargs='+')

# debugging
p.add_argument('--verbose', '-v', help='enable verbose logging', action='store_true')
args = p.parse_args()

vprint = print if args.verbose else lambda *a: None
vprint("args = ", args)

xi = args.xcol
dfs = []
for f in args.input:
    df = pd.read_csv(f)
    if args.suffix_names:
        df = df.add_suffix(' ' + os.path.basename(f.name))
    vprint("----- df from: ", f.name, "-----\n", df.head(), "\n---------------------")
    dfs.append(df)

df = pd.concat(dfs, axis=1)
vprint("----- merged df -----\n", df.head(), "\n---------------------")

# rename any duplicate columns because otherwise Pandas gets mad
df = df.rename(columns=pdutil.renamer())

vprint("---- renamed df ----\n", df.head(), "\n---------------------")

def extract_cols(cols, df, name):
    vprint(name, "axis columns: ", cols)
    if (max(cols) >= len(df.columns)):
        print("Column", max(cols), "too large: input only has", len(df.columns), "columns", file=sys.stderr)
        exit(1)
    pruned = df.iloc[:, cols]
    vprint("----- pruned ", name, " df -----\n", pruned.head(), "\n---------------------")
    return pruned

df2 = extract_cols(args.cols2, df, "secondary") if args.cols2 else None

if args.cols:
    if xi not in args.cols:
        args.cols.insert(0, xi)
    df = extract_cols(args.cols, df, "primary")

if args.clabels:
    if len(df.columns) != len(args.clabels):
        sys.exit("ERROR: number of column labels not equal to the number of selected columns")
    df.columns = args.clabels

# dupes will break pandas beyond this point, should be impossible due to above renaming
dupes = df.columns.duplicated()
if True in dupes:
    print("Duplicate columns after merge and pruning, consider --suffix-names",
        df.columns[dupes].values.tolist(), file=sys.stderr)
    exit(1)

# set x labels to strings so we don't get a scatter plot, and
# so the x labels are not themselves plotted
df.iloc[:,xi] = df.iloc[:,xi].apply(str)


ax = df.plot.line(x=xi, title=args.title, figsize=(12,8), grid=True, linewidth=2)

# this sets the ticks explicitly to one per x value, which means that
# all x values will be shown, but the x-axis could be crowded if there
# are too many, remmove to use the auto tick density
ticks = df.iloc[:,xi].values
plt.xticks(ticks=range(len(ticks)), labels=ticks)

if args.ylabel:
    ax.set_ylabel(args.ylabel)

if args.xlabel:
    ax.set_xlabel(args.xlabel)

if args.ylim:
    if (len(args.ylim) == 1):
        ax.set_ylim(args.ylim[0])
    elif (len(args.ylim) == 2):
        ax.set_ylim(args.ylim[0], args.ylim[1])
    else:
        sys.exit('provide one or two args to --ylim')

# secondary axis handling
if df2 is not None:
    df2.plot(secondary_y=True, ax=ax, grid=True)

if (args.out):
    vprint("Saving figure to ", args.out, "...")
    plt.savefig(args.out)
else:
    vprint("Showing interactive plot...")
    plt.show()
