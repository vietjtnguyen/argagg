#include "../include/argagg/argagg.hpp"
#include "../include/argagg/convert/opencv.hpp"

#include "doctest.h"

#include <cstring>
#include <iostream>
#include <vector>


TEST_CASE("cv::Point_ example")
{
  argagg::parser argparser {{
      { "point", {"-p", "--point"},
        "point as a comma separated list of components (e.g. '1,2')", 1},
    }};
  SUBCASE("zero components") {
    std::vector<const char*> argv {
      "test", "-p", ""};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("point") == true);
    CHECK_THROWS({
      args["point"].as<cv::Point>();
    });
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
  SUBCASE("three components (too many)") {
    std::vector<const char*> argv {
      "test", "-p", "1,3,5"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("point") == true);
    auto point = args["point"].as<cv::Point>();
    CHECK(point.x == 1);
    CHECK(point.y == 3);
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


TEST_CASE("cv::Point3_ example")
{
  argagg::parser argparser {{
      { "point", {"-p", "--point"},
        "point as a comma separated list of components (e.g. '1,2,3')", 1},
    }};
  SUBCASE("zero components") {
    std::vector<const char*> argv {
      "test", "-p", ""};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("point") == true);
    CHECK_THROWS({
      args["point"].as<cv::Point3i>();
    });
  }
  SUBCASE("one component") {
    std::vector<const char*> argv {
      "test", "-p", "1"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("point") == true);
    auto point = args["point"].as<cv::Point3i>();
    CHECK(point.x == 1);
    CHECK(point.y == 0);
    CHECK(point.z == 0);
  }
  SUBCASE("two components") {
    std::vector<const char*> argv {
      "test", "-p", "1,3"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("point") == true);
    auto point = args["point"].as<cv::Point3i>();
    CHECK(point.x == 1);
    CHECK(point.y == 3);
    CHECK(point.z == 0);
  }
  SUBCASE("three components") {
    std::vector<const char*> argv {
      "test", "-p", "1,3,5"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("point") == true);
    auto point = args["point"].as<cv::Point3i>();
    CHECK(point.x == 1);
    CHECK(point.y == 3);
    CHECK(point.z == 5);
  }
  SUBCASE("four components (too many)") {
    std::vector<const char*> argv {
      "test", "-p", "1,3,5,7"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("point") == true);
    auto point = args["point"].as<cv::Point3i>();
    CHECK(point.x == 1);
    CHECK(point.y == 3);
    CHECK(point.z == 5);
  }
  SUBCASE("float") {
    std::vector<const char*> argv {
      "test", "-p", "1.123,4.56789,3.14159"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("point") == true);
    auto point = args["point"].as<cv::Point3_<float>>();
    CHECK(point.x == 1.123f);
    CHECK(point.y == 4.56789f);
    CHECK(point.z == 3.14159f);
  }
  SUBCASE("double") {
    std::vector<const char*> argv {
      "test", "-p", "1.123,4.56789,3.14159"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("point") == true);
    auto point = args["point"].as<cv::Point3_<double>>();
    CHECK(point.x == 1.123);
    CHECK(point.y == 4.56789);
    CHECK(point.z == 3.14159);
  }
}


TEST_CASE("cv::Size_ example")
{
  argagg::parser argparser {{
      { "size", {"-s", "--size"},
        "size as width and height separated by an 'x' (e.g. '1x2')", 1},
    }};
  SUBCASE("zero components") {
    std::vector<const char*> argv {
      "test", "-s", ""};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("size") == true);
    CHECK_THROWS({
      args["size"].as<cv::Size>();
    });
  }
  SUBCASE("one component") {
    std::vector<const char*> argv {
      "test", "-s", "1"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("size") == true);
    auto size = args["size"].as<cv::Size>();
    CHECK(size.width == 1);
    CHECK(size.height == 0);
  }
  SUBCASE("two components") {
    std::vector<const char*> argv {
      "test", "-s", "1x3"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("size") == true);
    auto size = args["size"].as<cv::Size>();
    CHECK(size.width == 1);
    CHECK(size.height == 3);
  }
  SUBCASE("three components (too many)") {
    std::vector<const char*> argv {
      "test", "-s", "1x3x5"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("size") == true);
    auto size = args["size"].as<cv::Size>();
    CHECK(size.width == 1);
    CHECK(size.height == 3);
  }
  SUBCASE("float") {
    std::vector<const char*> argv {
      "test", "-s", "1.123x4.56789"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("size") == true);
    auto size = args["size"].as<cv::Size_<float>>();
    CHECK(size.width == 1.123f);
    CHECK(size.height == 4.56789f);
  }
  SUBCASE("double") {
    std::vector<const char*> argv {
      "test", "-s", "1.123x4.56789"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("size") == true);
    auto size = args["size"].as<cv::Size_<double>>();
    CHECK(size.width == 1.123);
    CHECK(size.height == 4.56789);
  }
}


TEST_CASE("cv::Rect_ example")
{
  argagg::parser argparser {{
      { "rect", {"-r", "--rect"},
        "rect as a comma separated list of x, y, width, and height (e.g. "
        "'1,2,3,4')", 1},
    }};
  SUBCASE("zero components") {
    std::vector<const char*> argv {
      "test", "-r", ""};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("rect") == true);
    CHECK_THROWS({
      args["rect"].as<cv::Rect>();
    });
  }
  SUBCASE("one component") {
    std::vector<const char*> argv {
      "test", "-r", "1"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("rect") == true);
    auto rect = args["rect"].as<cv::Rect>();
    CHECK(rect.x == 1);
    CHECK(rect.y == 0);
    CHECK(rect.width == 0);
    CHECK(rect.height == 0);
  }
  SUBCASE("two components") {
    std::vector<const char*> argv {
      "test", "-r", "1,3"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("rect") == true);
    auto rect = args["rect"].as<cv::Rect>();
    CHECK(rect.x == 1);
    CHECK(rect.y == 3);
    CHECK(rect.width == 0);
    CHECK(rect.height == 0);
  }
  SUBCASE("three components") {
    std::vector<const char*> argv {
      "test", "-r", "1,3,5"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("rect") == true);
    auto rect = args["rect"].as<cv::Rect>();
    CHECK(rect.x == 1);
    CHECK(rect.y == 3);
    CHECK(rect.width == 5);
    CHECK(rect.height == 0);
  }
  SUBCASE("four components") {
    std::vector<const char*> argv {
      "test", "-r", "1,3,5,7"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("rect") == true);
    auto rect = args["rect"].as<cv::Rect>();
    CHECK(rect.x == 1);
    CHECK(rect.y == 3);
    CHECK(rect.width == 5);
    CHECK(rect.height == 7);
  }
  SUBCASE("five components (too many)") {
    std::vector<const char*> argv {
      "test", "-r", "1,3,5,7,9"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("rect") == true);
    auto rect = args["rect"].as<cv::Rect>();
    CHECK(rect.x == 1);
    CHECK(rect.y == 3);
    CHECK(rect.width == 5);
    CHECK(rect.height == 7);
  }
  SUBCASE("float") {
    std::vector<const char*> argv {
      "test", "-r", "1.123,4.56789,3.14159,42"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("rect") == true);
    auto rect = args["rect"].as<cv::Rect_<float>>();
    CHECK(rect.x == 1.123f);
    CHECK(rect.y == 4.56789f);
    CHECK(rect.width == 3.14159f);
    CHECK(rect.height == 42.0f);
  }
  SUBCASE("double") {
    std::vector<const char*> argv {
      "test", "-r", "1.123,4.56789,3.14159,42"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("rect") == true);
    auto rect = args["rect"].as<cv::Rect_<double>>();
    CHECK(rect.x == 1.123);
    CHECK(rect.y == 4.56789);
    CHECK(rect.width == 3.14159);
    CHECK(rect.height == 42.0);
  }
}
