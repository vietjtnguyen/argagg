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
  using std::endl;
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

  cout << "This one is from a C++ program" << endl ;
  cout << "Try to launch me with some options" << endl ;
  cout << "(type " << argv[0] << " --help for the complete list)" << endl ;
  cout << "For example: " << argv[0] << " *.* --funct-opt" << endl ;

  // Define our usage text.
  ostringstream usage;
  usage
    << "Joins all positional arguments together with a separator" << endl
    << endl
    << "Usage: " << argv[0] << " [OPTIONS]... [FILES]..." << endl
    << endl;

  // Use our argument parser to... parse the command line arguments. If there
  // are any problems then just spit out the usage and help text and exit.
  argagg::parser_results args;
  try {
    args = argparser.parse(argc, argv);
  } catch (const std::exception& e) {
    cerr << usage.str() << argparser << endl
         << "Encountered exception while parsing arguments: " << e.what()
         << endl;
    return EXIT_FAILURE;
  }

  // If the help flag was specified then spit out the usage and help text and
  // exit.
  if (args["help"]) {
    cerr << usage.str() << argparser;
    return EXIT_SUCCESS;
  }

  if (args["version"]) {
    cerr << "2.0" << endl;
    return EXIT_SUCCESS;
  }

  if (!args["int-opt"]) {
    cerr << args.program << ": '--int-opt' ('-i') option required" << endl;
    return EXIT_FAILURE;
  }

  cout << "Here are the options you passed..." << endl;

  for (const auto& file : args.pos) {
    cout << "file: " << file << endl;
  }

  if (args["funct-opt"]) {
    cout << "You chose --funct-opt or -F." << endl;
  }

  if (args["str-opt"]) {
    cout << "You inserted " << args["str-opt"].as<std::string>() << " for " <<
      "--str-opt option." << endl;
  }

  if (args["int-opt"]) {
    cout << "This is the integer you input: " << args["int-opt"].as<int>()
      << "." << endl;
  }

  string flag_opt_arg = "off";
  if (args["flag-opt"]) {
    cout << "The flag option was given!" << endl;
    flag_opt_arg = "on";
  }
  cout << "The flag is " << flag_opt_arg << "." << endl;

  if (args["enum-opt"]) {
    cout << "enum-opt value: " << args["enum-opt"].as<string>() << endl;
    cout << "enum-opt (original specified) value: " << "hello" << endl;
  }

  cout << "def_opt: " << args["def-opt"].as<string>("Hello") << "! ";

  cout << "Have a nice day! :-)" << endl;

  return EXIT_SUCCESS;
}
