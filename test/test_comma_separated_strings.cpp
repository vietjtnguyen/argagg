#include "../include/argagg/argagg.hpp"
#include "../include/argagg/convert/comma_separated_strings.hpp"

#include "doctest.h"

#include <vector>


TEST_CASE("")
{
  argagg::parser argparser {{
      { "names", {"--names"},
        "names as a comma separated list of components (e.g. 'alice,bob')", 1},
    }};
  SUBCASE("empty") {
    std::vector<const char*> argv {
      "test", "--names", ""};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("names") == true);
    auto names = args["names"].as<argagg::comma_separated_strings>();
    CHECK(names.values.size() == 1);
    CHECK(names.values[0] == "");
  }
  SUBCASE("one") {
    std::vector<const char*> argv {
      "test", "--names", "alice"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("names") == true);
    auto names = args["names"].as<argagg::comma_separated_strings>();
    CHECK(names.values.size() == 1);
    CHECK(names.values[0] == "alice");
  }
  SUBCASE("two") {
    std::vector<const char*> argv {
      "test", "--names", "alice,bob"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("names") == true);
    auto names = args["names"].as<argagg::comma_separated_strings>();
    CHECK(names.values.size() == 2);
    CHECK(names.values[0] == "alice");
    CHECK(names.values[1] == "bob");
  }
  SUBCASE("multiple empty") {
    std::vector<const char*> argv {
      "test", "--names", "alice,,,bob"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("names") == true);
    auto names = args["names"].as<argagg::comma_separated_strings>();
    CHECK(names.values.size() == 4);
    CHECK(names.values[0] == "alice");
    CHECK(names.values[1] == "");
    CHECK(names.values[2] == "");
    CHECK(names.values[3] == "bob");
  }
}
