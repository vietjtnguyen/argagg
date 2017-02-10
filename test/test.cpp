#include "../include/argagg/argagg.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "./doctest/doctest.h"

#include <iostream>
#include <vector>


TEST_CASE("no specification, no arguments")
{
  argagg::parser parser {{
    }};
  std::vector<const char*> argv {
    "test"};
  argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_option("help") == false);
  CHECK(args.has_option("verbose") == false);
  CHECK(args.has_option("output") == false);
  CHECK(args.count() == 0);
}


TEST_CASE("no specification")
{
  argagg::parser parser {{
    }};
  std::vector<const char*> argv {
    "test", "foo", "bar", "baz"};
  argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_option("help") == false);
  CHECK(args.has_option("verbose") == false);
  CHECK(args.has_option("output") == false);
  CHECK(args.count() == 3);
  CHECK(::std::string(args.pos[0]) == "foo");
  CHECK(::std::string(args.pos[1]) == "bar");
  CHECK(::std::string(args.pos[2]) == "baz");
}


TEST_CASE("unused flag")
{
  argagg::parser parser {{
    }};
  std::vector<const char*> argv {
    "test", "foo", "-h", "bar"};
  CHECK_THROWS_AS({
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  }, argagg::unexpected_option_error);
}


TEST_CASE("unused short flag")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "--output", "foo", "-h", "bar", "-v"};
  CHECK_THROWS_AS({
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  }, argagg::unexpected_option_error);
}


TEST_CASE("no arguments")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test"};
  argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_option("help") == false);
  CHECK(args.has_option("verbose") == false);
  CHECK(args.has_option("output") == false);
  CHECK(args.count() == 0);
}


TEST_CASE("no flags")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "foo", "bar", "baz"};
  argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_option("help") == false);
  CHECK(args.has_option("verbose") == false);
  CHECK(args.has_option("output") == false);
  CHECK(args.count() == 3);
  CHECK(::std::string(args.pos[0]) == "foo");
  CHECK(::std::string(args.pos[1]) == "bar");
  CHECK(::std::string(args.pos[2]) == "baz");
}


TEST_CASE("only flags")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "--verbose", "--output", "foo", "-v", "-o", "bar"};
  argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_option("help") == false);
  CHECK(args.has_option("verbose") == true);
  CHECK(args["verbose"].count() == 2);
  CHECK(args["verbose"][0].arg == nullptr);
  CHECK(args["verbose"][1].arg == nullptr);
  CHECK(args.has_option("output") == true);
  CHECK(args["output"].count() == 2);
  CHECK(args["output"][0].as<std::string>() == "foo");
  CHECK(args["output"][1].as<std::string>() == "bar");
  CHECK(args.count() == 0);
}


TEST_CASE("general")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "-v", "--output", "foo", "bar"};
  argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_option("help") == false);
  CHECK(args.has_option("verbose") == true);
  CHECK(args["verbose"].count() == 1);
  CHECK(args["verbose"][0].arg == nullptr);
  CHECK(args.has_option("output") == true);
  CHECK(args["output"].count() == 1);
  CHECK(args["output"].as<std::string>() == "foo");
  CHECK(args.count() == 1);
  CHECK(args.as<std::string>(0) == "bar");
}


TEST_CASE("trailing flags")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "foo", "bar", "-v", "--output", "baz"};
  argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_option("help") == false);
  CHECK(args.has_option("verbose") == true);
  CHECK(args["verbose"].count() == 1);
  CHECK(args["verbose"][0].arg == nullptr);
  CHECK(args.has_option("output") == true);
  CHECK(args["output"].count() == 1);
  CHECK(args["output"].as<std::string>() == "baz");
  CHECK(args.count() == 2);
  CHECK(args.as<std::string>(0) == "foo");
  CHECK(args.as<std::string>(1) == "bar");
}


TEST_CASE("interleaved positional args")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "foo", "-v", "bar", "--verbose", "baz", "--output", "dog", "cat"};
  argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_option("help") == false);
  CHECK(args.has_option("verbose") == true);
  CHECK(args["verbose"].count() == 2);
  CHECK(args["verbose"][0].arg == nullptr);
  CHECK(args.has_option("output") == true);
  CHECK(args["output"].count() == 1);
  CHECK(args["output"].as<std::string>() == "dog");
  CHECK(args.count() == 4);
  CHECK(args.as<std::string>(0) == "foo");
  CHECK(args.as<std::string>(1) == "bar");
  CHECK(args.as<std::string>(2) == "baz");
  CHECK(args.as<std::string>(3) == "cat");
}


TEST_CASE("flag stop")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "-v", "--", "bar", "--verbose", "baz", "--output", "dog", "cat"};
  argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_option("help") == false);
  CHECK(args.has_option("verbose") == true);
  CHECK(args["verbose"].count() == 1);
  CHECK(args["verbose"][0].arg == nullptr);
  CHECK(args.has_option("output") == false);
  CHECK(args.count() == 6);
  CHECK(args.as<std::string>(0) == "bar");
  CHECK(args.as<std::string>(1) == "--verbose");
  CHECK(args.as<std::string>(2) == "baz");
  CHECK(args.as<std::string>(3) == "--output");
  CHECK(args.as<std::string>(4) == "dog");
  CHECK(args.as<std::string>(5) == "cat");
}


TEST_CASE("flag stop for dash argument")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"delim", {"-d", "--delim"}, "delimiter", argagg::optional},
    }};
  std::vector<const char*> argv {
    "test", "-d", "--", "-", "boo"};
  argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_option("help") == false);
  CHECK(args.has_option("verbose") == false);
  CHECK(args.has_option("delim") == true);
  CHECK(args["delim"].as<std::string>() == "-");
  CHECK(args.count() == 1);
  CHECK(args.as<std::string>(0) == "boo");
}


TEST_CASE("need one flag argument")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"number", {"-n", "--number"}, "number", 1},
    }};
  std::vector<const char*> argv {
    "test", "-n", "1", "2", "-n", "4"};
  argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_option("verbose") == false);
  CHECK(args.has_option("number") == true);
  CHECK(args["number"].count() == 2);
  CHECK(args["number"][0].as<int>() == 1);
  CHECK(args["number"][1].as<int>() == 4);
  CHECK(args.count() == 1);
  CHECK(args.as<int>(0) == 2);
}


TEST_CASE("need one flag argument, given zero, interrupted by flag")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"number", {"-n", "--number"}, "number", 1},
    }};
  std::vector<const char*> argv {
    "test", "-n", "-v"};
  CHECK_THROWS_AS({
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  }, argagg::option_lacks_argument_error);
}


TEST_CASE("need one flag argument, interrupted by unused flag")
{
  argagg::parser parser {{
      {"number", {"-n", "--number"}, "number", 1},
    }};
  std::vector<const char*> argv {
    "test", "-n", "1", "2", "-v"};
  CHECK_THROWS_AS({
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  }, argagg::unexpected_option_error);
}


TEST_CASE("need one flag arguments, given zero, end of args")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"number", {"-n", "--number"}, "number", 1},
    }};
  std::vector<const char*> argv {
    "test", "-n"};
  CHECK_THROWS_AS({
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  }, argagg::option_lacks_argument_error);
}


TEST_CASE("optional flag arguments")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"number", {"-n", "--number"}, "number", argagg::optional},
    }};
  std::vector<const char*> argv {
    "test", "-n", "1", "-n", "-n", "3", "-v", "4"};
  argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_option("verbose") == true);
  CHECK(args["verbose"].count() == 1);
  CHECK(args.has_option("number") == true);
  CHECK(args["number"].count() == 3);
  CHECK(args["number"][0].as<int>() == 1);
  CHECK(args["number"][1].as<int>(0) == 0);
  CHECK(args["number"][2].as<int>() == 3);
  CHECK(args.count() == 1);
  CHECK(args.as<int>(0) == 4);
}


TEST_CASE("positional argument integer conversions")
{
  argagg::parser parser {{
      {"number", {"-n", "--num", "--number"}, "number", 1},
    }};
  {
    std::vector<const char*> argv {
      "test", "1", "2"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.pos.size() == 2);
    CHECK(args.as<int>() == 1);
    CHECK(args.as<long>() == 1);
    CHECK(args.as<long long>() == 1);
    CHECK(args.as<std::string>() == "1");
    CHECK(args.as<int>(1) == 2);
    CHECK(args.as<long>(1) == 2);
    CHECK(args.as<long long>(1) == 2);
    CHECK(args.as<std::string>(1) == "2");
  }
}


TEST_CASE("positional argument floating point conversions")
{
  argagg::parser parser {{
      {"number", {"-n", "--num", "--number"}, "number", 1},
    }};
  {
    std::vector<const char*> argv {
      "test", "3.141592653", "2.71828182846"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.pos.size() == 2);
    CHECK(args.as<float>() == doctest::Approx(3.141592653f));
    CHECK(args.as<double>() == doctest::Approx(3.141592653));
    CHECK(args.as<std::string>() == "3.141592653");
    CHECK(args.as<float>(1) == doctest::Approx(2.71828182846f));
    CHECK(args.as<double>(1) == doctest::Approx(2.71828182846));
    CHECK(args.as<std::string>(1) == "2.71828182846");
  }
}


TEST_CASE("argument vector conversions")
{
  argagg::parser parser {{
      {"number", {"-n", "--num", "--number"}, "number", 1},
    }};
  {
    std::vector<const char*> argv {
      "test", "0", "1", "2"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.pos.size() == 3);
    auto v = args.all_as<int>();
    CHECK(v[0] == 0);
    CHECK(v[1] == 1);
    CHECK(v[2] == 2);
  }
}


TEST_CASE("flag argument integer conversions")
{
  argagg::parser parser {{
      {"number", {"-n", "--num", "--number"}, "number", 1},
    }};
  {
    std::vector<const char*> argv {
      "test", "-n", "1"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    CHECK(args["number"].count() == 1);
    CHECK(args["number"].as<int>() == 1);
    CHECK(args["number"].as<long>() == 1);
    CHECK(args["number"].as<long long>() == 1);
    CHECK(args["number"].as<std::string>() == "1");
  }
}


TEST_CASE("flag argument floating point conversions")
{
  argagg::parser parser {{
      {"number", {"-n", "--num", "--number"}, "number", 1},
    }};
  {
    std::vector<const char*> argv {
      "test", "-n", "3.141592653"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    CHECK(args["number"].count() == 1);
    CHECK(args["number"].as<float>() == doctest::Approx(3.141592653f));
    CHECK(args["number"].as<double>() == doctest::Approx(3.141592653));
    CHECK(args["number"].as<std::string>() == "3.141592653");
  }
}


TEST_CASE("flag argument implicit conversions")
{
  argagg::parser parser {{
      {"number", {"-n", "--num", "--number"}, "number", 1},
    }};
  {
    std::vector<const char*> argv {
      "test", "-n", "3.141592653", "-n", "2"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    CHECK(args["number"].count() == 2);
    float x = args["number"][0];
    int y = args["number"][1];
    CHECK(x == doctest::Approx(3.141592653f));
    CHECK(y == 2);
  }
}


TEST_CASE("write options help")
{
  argagg::parser parser {{
      {"help", {"-h", "--help"}, "print help", 0},
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::cout << parser;
}