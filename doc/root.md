Argument Aggregator {#mainpage}
===================

This is the Doxygen documentation for Argument Aggregator, a simple C++11 argument parser.

To use just create an argagg::parser object. However, the struct doesn't provide any explicit methods for defining flags. Instead we define the flags using initialization lists.

    argagg::parser argparser {{
        { "help", {"-h", "--help"},
          "shows this help message", 0},
        { "delim", {"-d", "--delim"},
          "delimiter (default: ,)", 1},
        { "num", {"-n", "--num"},
          "number", 1},
      }};

An option is specified by four things: the name of the option, the strings that activate the option (flags), the option's help message, and the number of arguments the option expects.

With the parser defined you actually parse the arguments by calling the argagg::parser::parse() method. If there are any problems then either an argagg::option_lacks_argument_error or argagg::unexpected_option_error exception.

    argagg::parser_results args;
    try {
      args = argparser.parse(argc, argv);
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return EXIT_FAILURE;
    }

You can check if an option shows up in the command line arguments by accessing the option by name from the parser results and using the implicit boolean conversion. You can write out a simplistic option help message by streaming the argagg::parser instance itself.

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

That help message is only for the flags. If you want a usage message it's up to you to provide it.

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

Generally argagg tries to do a minimal amount of work to leave most of the control with the user.

If you want to get an option argument but fallback on a default value if it doesn't exist then you can use the argagg::option_results::as() API and providing a default value.

    auto delim = args["delim"].as<std::string>(",");

If you don't mind being implicit an implicit conversion operator is provided allowing you to write simple assignments.

    int x = 0;
    if (args["num"]) {
      x = args["num"];
    }

Finally, you can get all of the positional arguments as an std::vector using the argagg::parser_results::pos member.

One can also specify `--` on the command line in order to treat all following arguments as not options.

For a more detailed treatment take a look at the examples or test cases.
