#include "../include/argagg/argagg.hpp"
#include "../include/argagg/convert/csv.hpp"

#include "doctest.h"

#include <iostream>
#include <vector>


// Define a custom conversion function for the test that follows
struct plus_triple {
  double x;
  double y;
  double z;
};
namespace argagg {
namespace convert {
  template <>
  plus_triple arg(const char* s)
  {
    plus_triple result {0.0, 0.0, 0.0};
    if (!parse_next_component(s, result.x, '+')) {
      return result;
    }
    if (!parse_next_component(s, result.y, '+')) {
      return result;
    }
    if (!parse_next_component(s, result.z, '+')) {
      return result;
    }
    return result;
  }
} // namespace convert
} // namespace argagg


TEST_CASE("comma separated values")
{
  argagg::parser argparser {{
      { "items", {"--items"},
        "items as a comma separated list of components (e.g. 'alice,bob,1,2')", 1},
    }};
  SUBCASE("empty") {
    std::vector<const char*> argv {
      "test", "--items", ""};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("items") == true);
    auto items = args["items"].as<argagg::csv<std::string>>();
    CHECK(items.values.size() == 1);
    CHECK(items.values[0] == "");
  }
  SUBCASE("one") {
    std::vector<const char*> argv {
      "test", "--items", "alice"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("items") == true);
    auto items = args["items"].as<argagg::csv<std::string>>();
    CHECK(items.values.size() == 1);
    CHECK(items.values[0] == "alice");
  }
  SUBCASE("two") {
    std::vector<const char*> argv {
      "test", "--items", "alice,bob"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("items") == true);
    auto items = args["items"].as<argagg::csv<std::string>>();
    CHECK(items.values.size() == 2);
    CHECK(items.values[0] == "alice");
    CHECK(items.values[1] == "bob");
  }
  SUBCASE("multiple empty") {
    std::vector<const char*> argv {
      "test", "--items", "alice,,,bob"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("items") == true);
    auto items = args["items"].as<argagg::csv<std::string>>();
    CHECK(items.values.size() == 4);
    CHECK(items.values[0] == "alice");
    CHECK(items.values[1] == "");
    CHECK(items.values[2] == "");
    CHECK(items.values[3] == "bob");
  }
  SUBCASE("integers") {
    std::vector<const char*> argv {
      "test", "--items", "1,2,3"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("items") == true);
    auto items = args["items"].as<argagg::csv<int>>();
    CHECK(items.values.size() == 3);
    CHECK(items.values[0] == 1);
    CHECK(items.values[1] == 2);
    CHECK(items.values[2] == 3);
  }
  SUBCASE("integers, bad conversion") {
    std::vector<const char*> argv {
      "test", "--items", "1,bob,3"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("items") == true);
    CHECK_THROWS({
      args["items"].as<argagg::csv<int>>();
    });
  }
  SUBCASE("doubles") {
    std::vector<const char*> argv {
      "test", "--items", "1.2,3.45,6.789"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("items") == true);
    auto items = args["items"].as<argagg::csv<double>>();
    CHECK(items.values.size() == 3);
    CHECK(items.values[0] == 1.2);
    CHECK(items.values[1] == 3.45);
    CHECK(items.values[2] == 6.789);
  }
  SUBCASE("doubles, bad conversion") {
    std::vector<const char*> argv {
      "test", "--items", "alice,3.45,6.789"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("items") == true);
    CHECK_THROWS({
      args["items"].as<argagg::csv<double>>();
    });
  SUBCASE("plus triples") {
    std::vector<const char*> argv {
      "test", "--items", "1.2+2.4+3.141,4.0+5.32,6.5341"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("items") == true);
    auto items = args["items"].as<argagg::csv<plus_triple>>();
    CHECK(items.values.size() == 3);
    CHECK(items.values[0].x == 1.2);
    CHECK(items.values[0].y == 2.4);
    CHECK(items.values[0].z == 3.141);
    CHECK(items.values[1].x == 4.0);
    CHECK(items.values[1].y == 5.32);
    CHECK(items.values[1].z == 0.0);
    CHECK(items.values[2].x == 6.5341);
    CHECK(items.values[2].y == 0.0);
    CHECK(items.values[2].z == 0.0);
  }
  }
}
