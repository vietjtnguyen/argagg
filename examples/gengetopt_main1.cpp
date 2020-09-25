/**
 * @file
 * @brief
 * This example is an adaptation of the example that can be found in the
 * gengetopt documentation:
 * https://www.gnu.org/software/gengetopt/gengetopt.html. It does not reach
 * feature parity because argagg does not implement dependent options, option
 * sections, word wrapping, and a few other features.
 */
#include <argagg/argagg.hpp>

#include <iostream>
#include <cstdlib>

int main(int argc, char **argv)
{
  using argagg::parser_results;
  using argagg::parser;
  using std::cerr;
  using std::cout;
  using std::ofstream;
  using std::ostream;
  using std::ostringstream;
  using std::string;

  parser argparser {{
      {
        "help", {"-h", "--help"},
        "Print help and exit", 0},
      {
        "version", {"-V", "--version"},
        "Print version and exit", 0},
      {
        "str-opt", {"-s", "--str-opt"},
        "A string option, for a filename", 1},
      {
        "my-opt", {"-m", "--my-opt"},
        "Another integer option", 1},
      {
        "int-opt", {"-i", "--int-opt"},
        "A int option", 1},
      {
        "flag-opt", {"--flag-opt"},
        "A flag option (default: off)", 0},
      {
        "funct-opt", {"-F", "--funct-opt"},
        "A function option", 0},
      {
        "long-opt", {"--long-opt"},
        "A long option", 1},
      {
        "def-opt", {"--def-opt"},
        "A string option with default (default: 'Hello')", 1},
      {
        "enum-opt", {"--enum-opt"},
        "A string option with list of values (possible values=\"foo\", "
        "\"bar\", \"hello\", \"bye\"; default=\"hello\")", 1},
      {
        "dependant", {"--dependant"},
        "option that depends on str-opt", 1},
    }};

  cout
    << "This one is from a C++ program\n"
    << "Try to launch me with some options\n"
    << "(type " << argv[0] << " --help for the complete list)\n"
    << "For example: " << argv[0] << " *.* --funct-opt\n";

  // Define our usage text.
  ostringstream usage;
  usage
    << argv[0] << " 2.0\n"
    << '\n'
    << "Usage: " << argv[0] << " [OPTIONS]... [FILES]...\n"
    << '\n';

  // Use our argument parser to... parse the command line arguments. If there
  // are any problems then just spit out the usage and help text and exit.
  argagg::parser_results args;
  try {
    args = argparser.parse(argc, argv);
  } catch (const std::exception& e) {
    argagg::fmt_ostream fmt(cerr);
    fmt << usage.str() << argparser << '\n'
        << "Encountered exception while parsing arguments: " << e.what()
        << '\n';
    return EXIT_FAILURE;
  }

  // If the help flag was specified then spit out the usage and help text and
  // exit.
  if (args["help"]) {
    argagg::fmt_ostream fmt(cerr);
    fmt << usage.str() << argparser;
    return EXIT_SUCCESS;
  }

  if (args["version"]) {
    cerr << "2.0\n";
    return EXIT_SUCCESS;
  }

  if (!args["int-opt"]) {
    cerr << args.program << ": '--int-opt' ('-i') option required\n";
    return EXIT_FAILURE;
  }

  cout << "Here are the options you passed...\n";

  for (const auto& file : args.pos) {
    cout << "file: " << file << '\n';
  }

  if (args["funct-opt"]) {
    cout << "You chose --funct-opt or -F.\n";
  }

  if (args["str-opt"]) {
    cout << "You inserted " << args["str-opt"].as<std::string>() << " for " <<
      "--str-opt option.\n";
  }

  if (args["int-opt"]) {
    cout << "This is the integer you input: " << args["int-opt"].as<int>()
      << ".\n";
  }

  string flag_opt_arg = "off";
  if (args["flag-opt"]) {
    cout << "The flag option was given!\n";
    flag_opt_arg = "on";
  }
  cout << "The flag is " << flag_opt_arg << ".\n";

  if (args["enum-opt"]) {
    cout << "enum-opt value: " << args["enum-opt"].as<string>() << '\n';
    cout << "enum-opt (original specified) value: hello\n";
  }

  cout << "def_opt: " << args["def-opt"].as<string>("Hello") << "! ";

  cout << "Have a nice day! :-)\n";

  return EXIT_SUCCESS;
}
