Argument Aggregator {#mainpage}
===================

This is the Doxygen documentation for Argument Aggregator, a simple C++11 argument parser.

To use just create an argagg::parser object. However, the struct doesn't provide any explicit methods for defining flags. Instead we define the flags using initialization lists.

    argagg::parser argparser {{
        { "help", {"-h", "--help"},
          "shows this help message", 0},
        { "delim", {"-d", "--delim"},
          "delimiter (default: ,)", argagg::flag_spec::optional},
      }}

A flag is specified by four things: the name of the flag, the strings that activate the flag, the flag's help message, and the number of arguments the flag expects.

With the parser defined you actually parse the arguments by calling the argagg::parser::parse() method. If there are any problems then either a std::out_of_range or argagg::unexpected_flag_error exception.

    argagg::result args;
    try {
      args = argparser.parse(argc, argv);
    } except (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return EXIT_FAILURE;
    }

You can check if a flag shows up in the arguments using the argagg::result::has_flag() method and giving it the flag name (not the flag text itself). You can write out some simplistic flag help message but streaming the argagg::parser instance itself.

    if (args.has_flag("help")) {
      std::cerr << argparser;
      //    -h, --help
      //        shows this help message
      return EXIT_SUCCESS;
    }

There's also an implicit boolean conversion you can use.

    if (args["help"]) {
      std::cerr << argparser;
      //    -h, --help
      //        shows this help message
      return EXIT_SUCCESS;
    }

That help message is only for the flags. If you want a usage message it's up to you to provide it.

    if (args["help"]) {
      std::cerr << "Usage: program [options] ARG1 ARG2" << std::endl
                << argparser;
      // Usage: program [options] ARG1 ARG2
      //     -h, --help
      //         shows this help message
      return EXIT_SUCCESS;
    }

Generally argagg tries to do a minimal amount of work to leave most of the control with the user.

If you want to get a flag argument but fallback on a default value if it doesn't exist then you can use the argagg::flags::as() API but providing a default value.

    auto delim = args["delim"].as<std::string>(",");

Finally, you can get all of the positional arguments as an std::vector using the argagg::results::pos member.

For a more detailed treatment take a look at the examples or test cases.
