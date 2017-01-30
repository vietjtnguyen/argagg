Argument Aggregator
===================

This is yet another C++ command line argument/option parser. It was written as a simple and idiomatic alternative to other frameworks like [getopt][], [Boost program options][], [TCLAP][], and others. The goal is to achieve the majority of argument parsing needs in a very simple manner (simple mental model) and easy to use API. It operates as a single pass over all arguments, recognizing flags prepended by `-` (short) or `--` (long) and aggregating them into easy to access structures.

[getopt]: https://www.gnu.org/software/libc/manual/html_node/Getopt.html#Getopt
[Boost program options]: http://www.boost.org/doc/libs/release/libs/program_options/
[TCLAP]: http://tclap.sourceforge.net/
