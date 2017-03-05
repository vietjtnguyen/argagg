Argument Aggregator
===================

| Branch | Build | Coverage |
| --- | --- | --- |
| `master` | ![Build Status](https://api.travis-ci.org/vietjtnguyen/argagg.svg?branch=master) | | [![Coverage Status](https://coveralls.io/repos/github/vietjtnguyen/argagg/badge.svg?branch=master)](https://coveralls.io/github/vietjtnguyen/argagg?branch=master) |
| `dev` | ![Build Status](https://api.travis-ci.org/vietjtnguyen/argagg.svg?branch=dev) | [![Coverage Status](https://coveralls.io/repos/github/vietjtnguyen/argagg/badge.svg?branch=dev)](https://coveralls.io/github/vietjtnguyen/argagg?branch=dev) |

This is yet another C++ command line argument/option parser. It was written as a simple and idiomatic alternative to other frameworks like [getopt][], [Boost program options][], [TCLAP][], and others. The goal is to achieve the majority of argument parsing needs in a simple manner with an easy to use API. It operates as a single pass over all arguments, recognizing flags prefixed by `-` (short) or `--` (long) and aggregating them into easy to access structures with lots of convenience functions. It defers processing types until you access them, so the result structures end up just being pointers into the original command line argument C-strings.

[getopt]: https://www.gnu.org/software/libc/manual/html_node/Getopt.html#Getopt
[Boost program options]: http://www.boost.org/doc/libs/release/libs/program_options/
[TCLAP]: http://tclap.sourceforge.net/

Introduction
------------

To use just create an `argagg::parser` object. The struct doesn't provide any explicit methods for defining options. Instead we define the options using [initialization lists][].

[initialization lists]: http://en.cppreference.com/w/cpp/language/list_initialization

```cpp
argagg::parser argparser {{
    { "help", {"-h", "--help"},
      "shows this help message", 0},
    { "delim", {"-d", "--delim"},
      "delimiter (default: ,)", 1},
    { "num", {"-n", "--num"},
      "number", 1},
  }};
```

An option is specified by four things: the name of the option, the strings that activate the option (flags), the option's help message, and the number of arguments the option expects.

With the parser defined you actually parse the arguments by calling the `argagg::parser::parse()` method. If there are any problems then either an `argagg::option_lacks_argument_error` or `argagg::unexpected_option_error` exception are thrown.

```cpp
argagg::parser_results args;
try {
  args = argparser.parse(argc, argv);
} catch (const std::exception& e) {
  std::cerr << e.what() << std::endl;
  return EXIT_FAILURE;
}
```

You can check if an option shows up in the command line arguments by accessing the option by name from the parser results and using the implicit boolean conversion. You can write out a simplistic option help message by streaming the `argagg::parser` instance itself.

```cpp
if (args["help"]) {
  std::cerr << argparser;
  //     -h, --help
  //         shows this help message
  //     -d, --delim
  //         delimiter (default: ,)
  //     -n, --num
  //         number
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
  //     -d, --delim
  //         delimiter (default: ,)
  //     -n, --num
  //         number
  return EXIT_SUCCESS;
}
```

Generally `argagg` tries to do a minimal amount of work to leave most of the control with the user.

If you want to get an option argument but fallback on a default value if it doesn't exist then you can use the `argagg::option_results::as()` API and provide a default value.

```cpp
auto delim = args["delim"].as<std::string>(",");
```

If you don't mind being implicit an implicit conversion operator is provided allowing you to write simple assignments.

```cpp
int x = 0;
if (args["num"]) {
  x = args["num"];
}
```

Finally, you can get all of the positional arguments as an `std::vector` using the `argagg::parser_results::pos` member. You can alternatively convert individual positional arguments using the same conversion functions as the option argument conversion methods.

    auto y = 0.0;
    if (args.pos.size() > 0) {
      y = args.as<double>(0);
    }

One can also specify `--` on the command line in order to treat all following arguments as not options.

For a more detailed treatment take a look at the [examples](./examples) or [test cases](./test).

Mental Model
------------

The parser just returns a structure of pointers to the C-strings in the original `argv` array. The `parse()` method returns a `parser_results` object which has two things: position arguments and option results. The position arguments are just a `std::vector` of `const char*`. The option results are a mapping from option name (`std::string`) to `option_results` objects. The `option_results` objects are just an `std::vector` of `option_result` objects. Each instance of an `object_result` represents the option showing up on the command line. If there was an argument associated with it then the `object_result`'s `arg` member will *not* be `nullptr`.

Consider the following command:

```sh
gcc -g -I/usr/local/include -I. -o test main.o foo.o -L/usr/local/lib -lz bar.o -lpng
```

This would produce a structure like follows, written in psuedo-YAML, where each string is actually a `const char*` pointing to some part of a string in the original `argv` array:

```yaml
parser_results:
  program: "gcc"
  pos: ["main.o", "foo.o", "bar.o"]
  options:
    version:
    debug:
      all:
      - arg: null
    include_path:
      all:
      - arg: "/usr/local/include"
      - arg: "."
    library_path:
      all:
      - arg: "/usr/local/lib"
    library:
      all:
      - arg: "z"
      - arg: "png"
    output:
      all:
      - arg: "test"
```

Conversion to types occurs at the very end when the `as<T>()` API is used. Up to that point `argagg` is just dealing with C-strings.

API Reference
-------------

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
