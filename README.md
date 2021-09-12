# FSA Tools

Generate random finite state automata and determine their complexity.

## Requirements

* `genfsa` requires only a C99 compiler.
* `dotify-att` is an AWK script
* the `verify-*` tests require GHC and [the LTK](https://github.com/vvulpes0/Language-Toolkit-2)
* the `Makefile` is written for BSD-compatible `make`

## Installation
Run `make`, then run `make install`.
The default installation prefix is `/usr/local`.
You can use a different prefix using `make PREFIX=/path/to/dir install`

## Basic Use

Generate a random FSA with seven states and an alphabet of four symbols
and view it (requires GraphViz and a `display` program):

```sh
genfsa 7 4 | dotify-att | dot -Tpng | display
```

GraphViz (which provides the `dot` program) has several other layout options
and can generate other types of files, including PDF, if desired.

Generate a somewhat sparse graph and determine if it is strictly local:

```sh
genfsa -e15 7 4 | verify-sl
```

Generate 100 random automata and determine
what proportion is strictly piecewise:

```sh
yes | head -n100 | xargs -L1 sh -c 'genfsa 7 5 | verify-sp && printf "1\n" || true' | wc -l | tr -d '[:blank:]'
```

If you have a BSD-compatible `xargs` then you can add, say, `-P16`
after `-L1` in order to run up to 16 checks in parallel at a time.
