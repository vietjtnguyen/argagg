#include <argagg/argagg.hpp>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

static std::ofstream g_dev_null;

int main(
  int argc,
  const char** argv)
{
  using argagg::parser_results;
  using argagg::parser;
  using std::cerr;
  using std::cout;
  using std::ofstream;
  using std::ostream;
  using std::ostringstream;
  using std::string;

  // Use an initializer list to define the argument parser. The first brace
  // starts the initializer list, the second brace starts the initializer list
  // for the definitions vector in the argagg::parser struct.
  parser argparser {{

      // Each entry here is an initializer list for an `argagg::definition`
      // struct.
      {
        // Name of the option. This is the key used to retrieve the flag parser
        // results.
        "help",

        // The strings ("flags") that must be matched to activate this option.
        {"-h", "--help"},

        // The help string that is streamed out when the argagg::parser object
        // itself is streamed out.
        "displays help information",

        // Number of arguments needed by this option. Should be 0 or 1.
        0},
      {
        "verbose", {"-v", "--verbose"},
        "increases verbosity", 0},
      {
        "lorem-ipsum", {"--lorem-ipsum"},
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
        "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim "
        "ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut "
        "aliquip ex ea commodo consequat. Duis aute irure dolor in "
        "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "
        "pariatur. Excepteur sint occaecat cupidatat non proident, sunt in "
        "culpa qui officia deserunt mollit anim id est laborum.", 0},
      {
        "sep", {"-s", "--sep"},
        "separator (default ',')", 1},
      {
        "output", {"-o", "--output"},
        "output filename (stdout if not specified)", 1},
    }};

  // Define our usage text.
  ostringstream usage;
  usage
    << "Joins all positional arguments together with a separator\n"
    << '\n'
    << "Usage: " << argv[0] << " [options] ARG [ARG...]\n"
    << '\n';

  // Use our argument parser to... parse the command line arguments. If there
  // are any problems then just spit out the usage and help text and exit.
  argagg::parser_results args;
  try {
    args = argparser.parse(argc, argv);
  } catch (const std::exception& e) {
    argagg::fmt_ostream help(cerr);
    help << usage.str() << argparser << '\n'
         << "Encountered exception while parsing arguments: " << e.what()
         << '\n';
    return EXIT_FAILURE;
  }

  // If the help flag was specified then spit out the usage and help text and
  // exit.
  if (args["help"]) {
    argagg::fmt_ostream help(cerr);
    help << usage.str() << argparser;
    return EXIT_SUCCESS;
  }

  // Respect verbosity. Okay, the logging here is a little ludicrous. The point
  // I want to show here is that you can quickly get the number of times an
  // option shows up.
  int verbose_level = args["verbose"].count();

  // Set up our verbose log output stream selector that selects stderr if the
  // requested log level is lower than or equal to the currently set verbose
  // level.
  g_dev_null.open("/dev/null"); // portable? eh... simple? yes!
  auto vlog = [&](int level) -> ostream& {
      return verbose_level >= level ? cerr : g_dev_null;
    };

  vlog(1) << "verbose log level: " << verbose_level << '\n';

  // Use comma as the separator unless one was specified.
  auto sep = args["sep"].as<string>(",");
  vlog(1) << "set separator to '" << sep << "'\n";

  // Determine output stream.
  ofstream output_file;
  ostream* output = &std::cout;
  if (args["output"]) {
    string filename = args["output"];
    output_file.open(filename);
    output = &output_file;
    vlog(1) << "outputting to file at '" << filename << "'\n";
  } else {
    vlog(1) << "outputting to stdout\n";
  }

  // Join the arguments.
  if (args.count() < 1) {
    vlog(0) << usage.str() << argparser << '\n'
            << "Not enough arguments\n";
    return EXIT_FAILURE;
  }
  for (auto& arg : args.pos) {
    vlog(2) << "writing argument\n";
    vlog(4) << "argument is '" << arg << "'\n";
    *output << arg;
    if (arg != args.pos.back()) {
      vlog(3) << "writing separator\n";
      *output << sep;
    }
  }
  vlog(4) << "writing endl\b";
  *output << '\n';

  vlog(4) << "everything a-okay\n";
  return EXIT_SUCCESS;
}
