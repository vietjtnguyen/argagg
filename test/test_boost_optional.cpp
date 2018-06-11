#include "../include/argagg/argagg.hpp"
#include "../include/argagg/convert/boost_optional.hpp"

#include "doctest.h"

#include <cstring>
#include <iostream>
#include <vector>


TEST_CASE("boost::optional example")
{
  argagg::parser argparser {{
      { "number", {"-n", "--number"},
        "a number", 1},
    }};
  SUBCASE("empty") {
    std::vector<const char*> argv {
      "test", "-n", ""};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    auto number = args["number"].as<boost::optional<int>>();
    CHECK(static_cast<bool>(number) == false);
  }
  SUBCASE("not a number") {
    std::vector<const char*> argv {
      "test", "-n", "foo"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    auto number = args["number"].as<boost::optional<int>>();
    CHECK(static_cast<bool>(number) == false);
  }
  SUBCASE("an integer") {
    std::vector<const char*> argv {
      "test", "-n", "1"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    auto number = args["number"].as<boost::optional<int>>();
    CHECK(static_cast<bool>(number) == true);
    CHECK(number.get() == 1);
  }
  SUBCASE("a double") {
    std::vector<const char*> argv {
      "test", "-n", "3.14159265358"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    auto number = args["number"].as<boost::optional<double>>();
    CHECK(static_cast<bool>(number) == true);
    CHECK(number.get() == 3.14159265358);
  }
  SUBCASE("a string") {
    std::vector<const char*> argv {
      "test", "-n", "hello"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    auto number = args["number"].as<boost::optional<std::string>>();
    CHECK(static_cast<bool>(number) == true);
    CHECK(number.get() == "hello");
  }
  SUBCASE("an empty string") {
    std::vector<const char*> argv {
      "test", "-n", ""};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    auto number = args["number"].as<boost::optional<std::string>>();
    CHECK(static_cast<bool>(number) == true);
    CHECK(number.get() == "");
  }
}
