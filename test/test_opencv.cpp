#include "../include/argagg/argagg.hpp"
#include "../include/argagg/convert/opencv.hpp"

#include "doctest.h"

#include <cstring>
#include <iostream>
#include <vector>


TEST_CASE("cv::Point example")
{
  argagg::parser argparser {{
      { "point", {"-p", "--point"},
        "point as a comma separated list of components (e.g. '1,2,3')", 1},
    }};
  SUBCASE("two components") {
    std::vector<const char*> argv {
      "test", "-p", "1,3"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("point") == true);
    auto point = args["point"].as<cv::Point>();
    CHECK(point.x == 1);
    CHECK(point.y == 3);
  }
  SUBCASE("one component") {
    std::vector<const char*> argv {
      "test", "-p", "1"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("point") == true);
    auto point = args["point"].as<cv::Point>();
    CHECK(point.x == 1);
    CHECK(point.y == 0);
  }
  SUBCASE("float") {
    std::vector<const char*> argv {
      "test", "-p", "1.123,4.56789"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("point") == true);
    auto point = args["point"].as<cv::Point_<float>>();
    CHECK(point.x == 1.123f);
    CHECK(point.y == 4.56789f);
  }
  SUBCASE("double") {
    std::vector<const char*> argv {
      "test", "-p", "1.123,4.56789"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("point") == true);
    auto point = args["point"].as<cv::Point_<double>>();
    CHECK(point.x == 1.123);
    CHECK(point.y == 4.56789);
  }
}
