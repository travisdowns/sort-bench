## What is it?

This a sort benchmark in support of [this related blog post](https://travisdowns.github.io/blog/2019/05/22/sorting.html).

## Building

Just run `make`.

Currently it builds on Linux with clang or gcc, or on Windows Subsystem for Linux (WSL) with the same compilers.

In principle it could work fine on Windows with other compilers, but I haven't had time to sort out my Visual Studio Community license.

It should work on OSX with clang or gcc, although I haven't tried it.

Patches which add platform support are welcome.

## Running

Run it like `./bench` to run all the benchmarks, or like `./bench BENCH_NAME` to run a specific benchmark (e.g., `./bench qsort` to run the qsort benchmark). The benchmark names are the same as those shown in the first row when you run `./bench`.

## Generating the Data and Figures

You too can generate the figures from the blog post on your hardware, using the scripts in the `scripts` directory.

First generate the data by running `scripts/data.sh`. This updates the CSV file `data/all.csv`.

Then you can generate all the figures by running `scripts/allfigs.sh`. This generates the .svg figures in `figures`. You can also generate any figure directly by calling say `scripts/fig2.sh` to generate Figure 2 (which by default opens an interactive window to display the figure).

## Acknowledgements

Some code in the `bench.cpp` and `wyhash.h` originates from [this code](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2019/05/07) written by Daniel Lemire for a [blog post](https://lemire.me/blog/2019/05/07/almost-picking-n-distinct-numbers-at-random/).

[wyhash](https://github.com/wangyi-fudan/wyhash) courtesy of Wang Yi.
