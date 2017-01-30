Argument Aggregator {#mainpage}
===================

This is the Doxygen documentation for Argument Aggregator, a simple C++11 argument parser.

To use create a argagg::parser object. However, the struct doesn't provide any explicit methods for defining flags. Instead we define the flags using initialization lists.

    argagg::parser argparser {{
        {"help", {"-h", "--help"}, "shows this help message", 0},
      }}

A flag is specified by four things: the name of the flag, the strings that activate the flag, the flag's help message, and the number of arguments the flag expects.

With the parser defined you actually parse the arguments by calling the argagg::parser::parse() method. If there are any problems then either a std::out_of_range or argagg::unexpected_flag_error exception.

    argagg::arg args;
    try {
      args = argparser.parse(argc, argv);
    } except (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return EXIT_FAILURE;
    }

You can check if a flag shows up in the arguments using the argagg::args::has_flag() method and giving it the flag name (not the flag text itself). You can write out some simplistic flag help message but streaming the argagg::parser instance itself.

    if (args.has_flag("help")) {
      std::cerr << argparser;
      //    -h, --help
      //        shows this help message
      return EXIT_SUCCESS;
    }

That help message is only for the flags. If you want a usage message it's up to you to provide it.

    if (args.has_flag("help")) {
      std::cerr << "Usage: program [options] ARG1 ARG2" << std::endl
                << argparser;
      // Usage: program [options] ARG1 ARG2
      //     -h, --help
      //         shows this help message
      return EXIT_SUCCESS;
    }

Generally argagg tries to do a minimal amount of work to leave most of the control with the user.

Finally, you can get all of the positional arguments as an std::vector using the argagg::args::args member.

For a more detailed treatment take a look at the examples or test cases.
