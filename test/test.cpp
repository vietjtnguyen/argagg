#include "../include/argagg/argagg.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "./doctest/doctest.h"

#include <iostream>
#include <vector>


TEST_CASE("no specification, no arguments")
{
  using namespace argagg;
  parser parser {{
    }};
  std::vector<const char*> argv {
    "test"};
  args args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_flag("help") == false);
  CHECK(args.has_flag("verbose") == false);
  CHECK(args.has_flag("output") == false);
  CHECK(args.count() == 0);
}


TEST_CASE("no specification")
{
  using namespace argagg;
  parser parser {{
    }};
  std::vector<const char*> argv {
    "test", "foo", "bar", "baz"};
  args args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_flag("help") == false);
  CHECK(args.has_flag("verbose") == false);
  CHECK(args.has_flag("output") == false);
  CHECK(args.count() == 3);
  CHECK(::std::string(args.args[0]) == "foo");
  CHECK(::std::string(args.args[1]) == "bar");
  CHECK(::std::string(args.args[2]) == "baz");
}


TEST_CASE("unused flag")
{
  using namespace argagg;
  parser parser {{
    }};
  std::vector<const char*> argv {
    "test", "foo", "-h", "bar"};
  CHECK_THROWS_AS({
    args args = parser.parse(argv.size(), &(argv.front()));
  }, unexpected_flag_error);
}


TEST_CASE("unused short flag")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "--output", "foo", "-h", "bar", "-v"};
  CHECK_THROWS_AS({
    args args = parser.parse(argv.size(), &(argv.front()));
  }, unexpected_flag_error);
}


TEST_CASE("no arguments")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test"};
  args args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_flag("help") == false);
  CHECK(args.has_flag("verbose") == false);
  CHECK(args.has_flag("output") == false);
  CHECK(args.count() == 0);
}


TEST_CASE("no flags")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "foo", "bar", "baz"};
  args args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_flag("help") == false);
  CHECK(args.has_flag("verbose") == false);
  CHECK(args.has_flag("output") == false);
  CHECK(args.count() == 3);
  CHECK(::std::string(args.args[0]) == "foo");
  CHECK(::std::string(args.args[1]) == "bar");
  CHECK(::std::string(args.args[2]) == "baz");
}


TEST_CASE("only flags")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "--verbose", "--output", "foo", "-v", "-o", "bar"};
  args args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_flag("help") == false);
  CHECK(args.has_flag("verbose") == true);
  CHECK(args["verbose"].occurences == 2);
  CHECK(args["verbose"].args.size() == 0);
  CHECK(args.has_flag("output") == true);
  CHECK(args["output"].occurences == 2);
  CHECK(args["output"].args.size() == 2);
  CHECK(::std::string(args["output"].args[0]) == "foo");
  CHECK(::std::string(args["output"].args[1]) == "bar");
  CHECK(args.count() == 0);
}


TEST_CASE("general")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "-v", "--output", "foo", "bar"};
  args args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_flag("help") == false);
  CHECK(args.has_flag("verbose") == true);
  CHECK(args["verbose"].occurences == 1);
  CHECK(args["verbose"].args.size() == 0);
  CHECK(args.has_flag("output") == true);
  CHECK(args["output"].occurences == 1);
  CHECK(args["output"].args.size() == 1);
  CHECK(::std::string(args["output"].args[0]) == "foo");
  CHECK(args.count() == 1);
  CHECK(::std::string(args.args[0]) == "bar");
}


TEST_CASE("trailing flags")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "foo", "bar", "-v", "--output", "baz"};
  args args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_flag("help") == false);
  CHECK(args.has_flag("verbose") == true);
  CHECK(args["verbose"].occurences == 1);
  CHECK(args["verbose"].args.size() == 0);
  CHECK(args.has_flag("output") == true);
  CHECK(args["output"].occurences == 1);
  CHECK(args["output"].args.size() == 1);
  CHECK(::std::string(args["output"].args[0]) == "baz");
  CHECK(args.count() == 2);
  CHECK(::std::string(args.args[0]) == "foo");
  CHECK(::std::string(args.args[1]) == "bar");
}


TEST_CASE("interleaved positional args")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "foo", "-v", "bar", "--verbose", "baz", "--output", "dog", "cat"};
  args args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_flag("help") == false);
  CHECK(args.has_flag("verbose") == true);
  CHECK(args["verbose"].occurences == 2);
  CHECK(args["verbose"].args.size() == 0);
  CHECK(args.has_flag("output") == true);
  CHECK(args["output"].occurences == 1);
  CHECK(args["output"].args.size() == 1);
  CHECK(::std::string(args["output"].args[0]) == "dog");
  CHECK(args.count() == 4);
  CHECK(::std::string(args.args[0]) == "foo");
  CHECK(::std::string(args.args[1]) == "bar");
  CHECK(::std::string(args.args[2]) == "baz");
  CHECK(::std::string(args.args[3]) == "cat");
}


TEST_CASE("flag stop")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::vector<const char*> argv {
    "test", "-v", "--", "bar", "--verbose", "baz", "--output", "dog", "cat"};
  args args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_flag("help") == false);
  CHECK(args.has_flag("verbose") == true);
  CHECK(args["verbose"].occurences == 1);
  CHECK(args["verbose"].args.size() == 0);
  CHECK(args.has_flag("output") == false);
  CHECK(args.count() == 6);
  CHECK(::std::string(args.args[0]) == "bar");
  CHECK(::std::string(args.args[1]) == "--verbose");
  CHECK(::std::string(args.args[2]) == "baz");
  CHECK(::std::string(args.args[3]) == "--output");
  CHECK(::std::string(args.args[4]) == "dog");
  CHECK(::std::string(args.args[5]) == "cat");
}


TEST_CASE("exactly four flag arguments")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"numbers", {"-n", "--numbers"}, "numbers", 4},
    }};
  std::vector<const char*> argv {
    "test", "-n", "1", "2", "3", "4"};
  args args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_flag("verbose") == false);
  CHECK(args.has_flag("numbers") == true);
  CHECK(args["numbers"].occurences == 1);
  CHECK(args["numbers"].args.size() == 4);
  auto v = args["numbers"].all_as<int>();
  REQUIRE(v.size() == 4);
  for (int i = 0; i < 4; ++i) {
    CHECK(v[i] == i + 1);
  }
}


TEST_CASE("exactly four flag arguments, given zero")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"numbers", {"-n", "--numbers"}, "numbers", 4},
    }};
  std::vector<const char*> argv {
    "test", "-n", "-v"};
  CHECK_THROWS_AS({
    args args = parser.parse(argv.size(), &(argv.front()));
  }, unexpected_flag_error);
}


TEST_CASE("exactly four flag arguments, given zero, end of args")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"numbers", {"-n", "--numbers"}, "numbers", 4},
    }};
  std::vector<const char*> argv {
    "test", "-n"};
  CHECK_THROWS_AS({
    args args = parser.parse(argv.size(), &(argv.front()));
  }, std::out_of_range);
}


TEST_CASE("zero or more flag arguments")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"numbers", {"-n", "--numbers"}, "numbers", flag_spec::zero_plus},
    }};
  std::vector<const char*> argv {
    "test", "-n", "1", "2", "3", "-v", "4"};
  args args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_flag("verbose") == true);
  CHECK(args["verbose"].occurences == 1);
  CHECK(args.has_flag("numbers") == true);
  CHECK(args["numbers"].occurences == 1);
  CHECK(args["numbers"].args.size() == 3);
  auto v = args["numbers"].all_as<int>();
  REQUIRE(v.size() == 3);
  for (int i = 0; i < 3; ++i) {
    CHECK(v[i] == i + 1);
  }
  CHECK(args.count() == 1);
  CHECK(::std::string(args.args[0]) == "4");
}


TEST_CASE("zero or more flag arguments, given zero")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"numbers", {"-n", "--numbers"}, "numbers", flag_spec::zero_plus},
    }};
  std::vector<const char*> argv {
    "test", "-n", "-v"};
  args args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_flag("verbose") == true);
  CHECK(args["verbose"].occurences == 1);
  CHECK(args.has_flag("numbers") == true);
  CHECK(args["numbers"].occurences == 1);
  CHECK(args["numbers"].args.size() == 0);
}


TEST_CASE("at least one flag arguments")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"numbers", {"-n", "--numbers"}, "numbers", flag_spec::one_plus},
    }};
  std::vector<const char*> argv {
    "test", "-n", "1", "2", "3", "-v", "4",};
  args args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_flag("verbose") == true);
  CHECK(args["verbose"].occurences == 1);
  CHECK(args.has_flag("numbers") == true);
  CHECK(args["numbers"].occurences == 1);
  CHECK(args["numbers"].args.size() == 3);
  auto v = args["numbers"].all_as<int>();
  REQUIRE(v.size() == 3);
  for (int i = 0; i < 3; ++i) {
    CHECK(v[i] == i + 1);
  }
  CHECK(args.count() == 1);
  CHECK(::std::string(args.args[0]) == "4");
}


TEST_CASE("at least one flag arguments, given zero")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"numbers", {"-n", "--numbers"}, "numbers", flag_spec::one_plus},
    }};
  std::vector<const char*> argv {
    "test", "-n", "-v"};
  CHECK_THROWS_AS({
    args args = parser.parse(argv.size(), &(argv.front()));
  }, unexpected_flag_error);
}


TEST_CASE("at least one flag arguments, given zero, end of args")
{
  using namespace argagg;
  parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"numbers", {"-n", "--numbers"}, "numbers", flag_spec::one_plus},
    }};
  std::vector<const char*> argv {
    "test", "-n"};
  CHECK_THROWS_AS({
    args args = parser.parse(argv.size(), &(argv.front()));
  }, std::out_of_range);
}


TEST_CASE("flag arguments interrupted by flag")
{
  using namespace argagg;
  parser parser {{
      {"numbers", {"-n", "--numbers"}, "numbers", 3},
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
    }};
  std::vector<const char*> argv {
    "test", "-n", "1", "2", "-v"};
  CHECK_THROWS_AS({
    args args = parser.parse(argv.size(), &(argv.front()));
  }, unexpected_flag_error);
}


TEST_CASE("flag arguments interrupted by unused flag")
{
  using namespace argagg;
  parser parser {{
      {"numbers", {"-n", "--numbers"}, "numbers", 3},
    }};
  std::vector<const char*> argv {
    "test", "-n", "1", "2", "-v"};
  CHECK_THROWS_AS({
    args args = parser.parse(argv.size(), &(argv.front()));
  }, unexpected_flag_error);
}


TEST_CASE("run out of arguments for flag")
{
  using namespace argagg;
  parser parser {{
      {"numbers", {"-n", "--numbers"}, "numbers", 3},
    }};
  std::vector<const char*> argv {
    "test", "-n", "1", "2"};
  CHECK_THROWS_AS({
    args args = parser.parse(argv.size(), &(argv.front()));
  }, std::out_of_range);
}


TEST_CASE("argument integer conversions")
{
  using namespace argagg;
  parser parser {{
      {"number", {"-n", "--num", "--number"}, "number", 1},
    }};
  {
    std::vector<const char*> argv {
      "test", "1", "2"};
    args args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.args.size() == 2);
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


TEST_CASE("argument floating point conversions")
{
  using namespace argagg;
  parser parser {{
      {"number", {"-n", "--num", "--number"}, "number", 1},
    }};
  {
    std::vector<const char*> argv {
      "test", "3.141592653", "2.71828182846"};
    args args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.args.size() == 2);
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
  using namespace argagg;
  parser parser {{
      {"number", {"-n", "--num", "--number"}, "number", 1},
    }};
  {
    std::vector<const char*> argv {
      "test", "0", "1", "2"};
    args args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.args.size() == 3);
    auto v = args.all_as<int>();
    CHECK(v[0] == 0);
    CHECK(v[1] == 1);
    CHECK(v[2] == 2);
  }
}


TEST_CASE("flag argument integer conversions")
{
  using namespace argagg;
  parser parser {{
      {"number", {"-n", "--num", "--number"}, "number", 1},
    }};
  {
    std::vector<const char*> argv {
      "test", "-n", "1"};
    args args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_flag("number") == true);
    CHECK(args["number"].args.size() == 1);
    CHECK(args["number"].as<int>() == 1);
    CHECK(args["number"].as<long>() == 1);
    CHECK(args["number"].as<long long>() == 1);
    CHECK(args["number"].as<std::string>() == "1");
  }
}


TEST_CASE("flag argument floating point conversions")
{
  using namespace argagg;
  parser parser {{
      {"number", {"-n", "--num", "--number"}, "number", 1},
    }};
  {
    std::vector<const char*> argv {
      "test", "-n", "3.141592653"};
    args args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_flag("number") == true);
    CHECK(args["number"].args.size() == 1);
    CHECK(args["number"].as<float>() == doctest::Approx(3.141592653f));
    CHECK(args["number"].as<double>() == doctest::Approx(3.141592653));
    CHECK(args["number"].as<std::string>() == "3.141592653");
  }
}


TEST_CASE("flag argument vector conversions")
{
  using namespace argagg;
  parser parser {{
      {"number", {"-n", "--num", "--number"}, "number", flag_spec::one_plus},
    }};
  {
    std::vector<const char*> argv {
      "test", "-n", "0", "1", "2"};
    args args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_flag("number") == true);
    CHECK(args["number"].args.size() == 3);
    auto v = args["number"].all_as<int>();
    CHECK(v[0] == 0);
    CHECK(v[1] == 1);
    CHECK(v[2] == 2);
  }
}


TEST_CASE("write options help")
{
  using namespace argagg;
  parser parser {{
      {"help", {"-h", "--help"}, "print help", 0},
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  std::cout << parser;
}
