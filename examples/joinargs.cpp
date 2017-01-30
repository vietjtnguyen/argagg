#include <argagg/argagg.hpp>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

int main(
  int argc,
  const char** argv)
{
  using argagg::args;
  using argagg::flag_spec;
  using argagg::parser;
  using std::cerr;
  using std::cout;
  using std::endl;
  using std::ofstream;
  using std::ostream;
  using std::ostringstream;
  using std::string;

  // Use an initializer list to define the argument parser. The first brace
  // starts the initializer list, the second brace starts the initializer list
  // for the `specs` vector in the `argagg::parser` struct.
  parser argparser {{
      // Each entry here is an initializer list for an `argagg::flag_spec`
      // struct. The struct entities are the name of the flag, a vector/list of
      // strings that when matched will match this flag, the help string, and
      // the number of arguments this flag needs.
      {
        "help", {"-h", "--help"},
        "displays help information", 0},
      {
        "verbose", {"-v", "--verbose"},
        "increases verbosity", 0},
      {
        "sep", {"-s", "--sep"},
        "separator (default ',')", flag_spec::optional},
      {
        "output", {"-o", "--output"},
        "output filename (stdout if not specified)", 1},
    }};

  // Define our usage text.
  ostringstream usage;
  usage
    << "Joins all positional arguments together with a separator" << endl
    << endl
    << "Usage: " << argv[0] << " [options] ARG [ARG...]" << endl
    << endl;

  // Use our argument parser to... parse the command line arguments. If there
  // are any problems then just spit out the usage and help text and exit.
  args args;
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
  if (args.has_flag("help")) {
    cerr << usage.str() << argparser;
    return EXIT_SUCCESS;
  }

  // Use comma as the separator unless one was specified.
  string sep = ",";
  if (args.has_flag("sep")) {
    sep = args["sep"].as<string>();
  }

  // Determine output stream.
  ofstream output_file;
  ostream* output = &std::cout;
  if (args.has_flag("output")) {
    output_file.open(args["output"].as<string>());
    output = &output_file;
  }

  // Join the arguments.
  if (args.count() < 1) {
    cerr << usage.str() << argparser << endl
         << "Not enough arguments" << endl;
    return EXIT_FAILURE;
  }
  for (auto& arg : args.args) {
    *output << arg;
    if (arg != args.args.back()) {
      *output << sep;
    }
  }
  *output << endl;

  return EXIT_SUCCESS;  
}
