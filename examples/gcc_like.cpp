/*
 * $ mkdir build
 * $ cd build
 * $ cmake -DARGAGG_BUILD_EXAMPLES=ON ..
 * $ make
 * $ ./bin/gcc_like -I/usr/include -I/usr/local/include -DNAME=VALUE \
 *   -DN_DEBUG -ofoo foo.h foo.cpp -I./include -DHACKS_ENABLED hacks.cpp
 * include paths:
 *   /usr/include
 *   /usr/local/include
 *   ./include
 * definitions:
 *   NAME=VALUE
 *   N_DEBUG
 *   HACKS_ENABLED
 * input files:
 *   foo.h
 *   foo.cpp
 *   hacks.cpp
 * outputting to:
 *   foo
 */
#include <argagg/argagg.hpp>

#include <cstdlib>
#include <iostream>
#include <sstream>

int main(int argc, const char** argv)
{
  argagg::parser argparser {{
    {"help", {"-h", "--help"}, "displays help information", 0},
    {"verbose", {"-v", "--verbose"}, "increases verbosity", 0},
    {"include_path", {"-I"}, "include path (can be repeated)", 1},
    {"definition", {"-D"}, "preprocessor definitions (can be repeated)", 1},
    {"output", {"-o", "--output"}, "output filename (default: stdout)", 1},
  }};

  // Define our usage text.
  std::ostringstream usage;
  usage
    << "Mimics a small set of GCC command line arguments\n"
    << "\n"
    << "Usage: " << argv[0] << " [options] INPUT [INPUT...]\n"
    << "\n";

  // Use our argument parser to... parse the command line arguments. If there
  // are any problems then just spit out the usage and help text and exit.
  argagg::parser_results args;
  try {
    args = argparser.parse(argc, argv);
  } catch (const std::exception& e) {
    argagg::fmt_ostream help(std::cerr);
    help
      << usage.str() << argparser << "\n"
      << "Encountered exception while parsing arguments: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  // If the help flag was specified then spit out the usage and help text and
  // exit.
  if (args["help"]) {
    argagg::fmt_ostream help(std::cerr);
    help << usage.str() << argparser;
    return EXIT_SUCCESS;
  }

  // Process include paths
  std::vector<std::string> include_paths;
  for (const auto& include_path_option : args["include_path"].all) {
    include_paths.emplace_back(include_path_option.as<std::string>());
  }

  // Process definitions
  std::vector<std::string> definitions;
  for (const auto& definition_option : args["definition"].all) {
    definitions.emplace_back(definition_option.as<std::string>());
  }

  // Regurgitate arguments
  std::cout << "include paths:\n";
  for (const auto& include_path : include_paths) {
    std::cout << "  " << include_path << "\n";
  }
  std::cout << "definitions:\n";
  for (const auto& definition : definitions) {
    std::cout << "  " << definition << "\n";
  }
  std::cout << "input files:\n";
  for (const auto& input_arg : args.pos) {
    std::cout << "  " << input_arg << "\n";
  }
  std::cout
    << "outputting to: \n"
    << "  " << args["output"].as<std::string>() << "\n";

  return EXIT_SUCCESS;
}
