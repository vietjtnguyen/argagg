Argument Aggregator
===================

| Branch | Status |
| --- | --- |
| `master` | ![Build Status](https://api.travis-ci.org/vietjtnguyen/argagg.svg?branch=master) |
| `dev` | ![Build Status](https://api.travis-ci.org/vietjtnguyen/argagg.svg?branch=dev) |

This is yet another C++ command line argument/option parser. It was written as a simple and idiomatic alternative to other frameworks like [getopt][], [Boost program options][], [TCLAP][], and others. The goal is to achieve the majority of argument parsing needs in a very simple manner (simple mental model) and easy to use API. It operates as a single pass over all arguments, recognizing flags prepended by `-` (short) or `--` (long) and aggregating them into easy to access structures with lots of convenience functions.

[getopt]: https://www.gnu.org/software/libc/manual/html_node/Getopt.html#Getopt
[Boost program options]: http://www.boost.org/doc/libs/release/libs/program_options/
[TCLAP]: http://tclap.sourceforge.net/

Introduction
------------

To use just create an `argagg::parser` object. However, the struct doesn't provide any explicit methods for defining flags. Instead we define the flags using initialization lists.

```cpp
argagg::parser argparser {{
    { "help", {"-h", "--help"},
      "shows this help message", 0},
    { "delim", {"-d", "--delim"},
      "delimiter (default: ,)", argagg::flag_spec::optional},
  }}
```

A flag is specified by four things: the name of the flag, the strings that activate the flag, the flag's help message, and the number of arguments the flag expects.

With the parser defined you actually parse the arguments by calling the `argagg::parser::parse()` method. If there are any problems then either a `std::out_of_range` or `argagg::unexpected_flag_error` exception.

```cpp
argagg::result args;
try {
  args = argparser.parse(argc, argv);
} except (const std::exception& e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
}
```

You can check if a flag shows up in the arguments using the `argagg::results::has_flag()` method and giving it the flag name (not the flag text itself). You can write out some simplistic flag help message but streaming the `argagg::parser` instance itself.

```cpp
if (args.has_flag("help")) {
  std::cerr << argparser;
  //    -h, --help
  //        shows this help message
  return EXIT_SUCCESS;
}
```

There's also an implicit boolean conversion you can use.

```cpp
if (args["help"]) {
  std::cerr << argparser;
  //    -h, --help
  //        shows this help message
  return EXIT_SUCCESS;
}
```

That help message is only for the flags. If you want a usage message it's up to you to provide it.

```cpp
if (args["help"]) {
  std::cerr << "Usage: program [options] ARG1 ARG2" << std::endl
            << argparser;
  // Usage: program [options] ARG1 ARG2
  //     -h, --help
  //         shows this help message
  return EXIT_SUCCESS;
}
```

Generally `argagg` tries to do a minimal amount of work to leave most of the control with the user.

If you want to get a flag argument but fallback on a default value if it doesn't exist then you can use the argagg::flags::as() API but providing a default value.

```cpp
auto delim = args["delim"].as<std::string>(",");
```

Finally, you can get all of the positional arguments as an `std::vector` using the `argagg::results::pos` member.

For a more detailed treatment take a look at the [examples](./examples) or [test cases](./test).

Doxygen documentation can be found [here](https://vietjtnguyen.github.io/argagg/latest/).

Installation
------------

There is just a single header file ([`argagg.hpp`](./include/argagg/argagg.hpp)) so you can copy that whereever you want. If you want to properly install it you can use the CMake script. The CMake script exists primarily to build the tests and documentation, but an install target for the header is provided.

The standard installation dance using CMake and `make` is as follows:

```sh
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
make install
ctest -V # optionally run tests
```

Override [`CMAKE_INSTALL_PREFIX`](https://cmake.org/cmake/help/v2.8.12/cmake.html#variable:CMAKE_INSTALL_PREFIX) to change the installation location. By default (on UNIX variants) it will install to `/usr/local` resulting in the header being copied to `/usr/local/include/argagg/argagg.hpp`.

If you have [Doxygen](http://www.stack.nl/~dimitri/doxygen/) it should build and install documentation as well.

There are no dependencies other than the standard library.
