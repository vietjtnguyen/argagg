#include "../include/argagg/argagg.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstring>
#include <iostream>
#include <vector>


TEST_CASE("cmd_line_arg_is_option_flag")
{
  CHECK(argagg::cmd_line_arg_is_option_flag("") == false);
  CHECK(argagg::cmd_line_arg_is_option_flag("a") == false);
  CHECK(argagg::cmd_line_arg_is_option_flag("abc") == false);
  CHECK(argagg::cmd_line_arg_is_option_flag("-") == false);
  CHECK(argagg::cmd_line_arg_is_option_flag("-a") == true);
  CHECK(argagg::cmd_line_arg_is_option_flag("-abc") == true);
  CHECK(argagg::cmd_line_arg_is_option_flag("-I/usr/local/include") == true);
  CHECK(argagg::cmd_line_arg_is_option_flag("--") == false);
  CHECK(argagg::cmd_line_arg_is_option_flag("---a") == false);
  CHECK(argagg::cmd_line_arg_is_option_flag("--a") == true);
  CHECK(argagg::cmd_line_arg_is_option_flag("--abc") == true);
  CHECK(argagg::cmd_line_arg_is_option_flag("---abc") == false);
  CHECK(argagg::cmd_line_arg_is_option_flag("--a@b") == false);
  CHECK(argagg::cmd_line_arg_is_option_flag("--a+b") == false);
  CHECK(argagg::cmd_line_arg_is_option_flag("--foo-bar") == true);
  CHECK(argagg::cmd_line_arg_is_option_flag("--output=~/out.txt") == true);
}


TEST_CASE("is_valid_flag_definition")
{
  CHECK(argagg::is_valid_flag_definition("") == false);
  CHECK(argagg::is_valid_flag_definition("a") == false);
  CHECK(argagg::is_valid_flag_definition("abc") == false);
  CHECK(argagg::is_valid_flag_definition("-") == false);
  CHECK(argagg::is_valid_flag_definition("-a") == true);
  CHECK(argagg::is_valid_flag_definition("-abc") == false);
  CHECK(argagg::is_valid_flag_definition("-I/usr/local/include") == false);
  CHECK(argagg::is_valid_flag_definition("--") == false);
  CHECK(argagg::is_valid_flag_definition("---a") == false);
  CHECK(argagg::is_valid_flag_definition("--a") == true);
  CHECK(argagg::is_valid_flag_definition("--abc") == true);
  CHECK(argagg::is_valid_flag_definition("---abc") == false);
  CHECK(argagg::is_valid_flag_definition("--a@b") == false);
  CHECK(argagg::is_valid_flag_definition("--a+b") == false);
  CHECK(argagg::is_valid_flag_definition("--foo-bar") == true);
  CHECK(argagg::is_valid_flag_definition("--output=~/out.txt") == false);
}


TEST_CASE("flag_is_short")
{
  CHECK(argagg::flag_is_short("-a") == true);
  CHECK(argagg::flag_is_short("-abc") == true);
  CHECK(argagg::flag_is_short("--a") == false);
  CHECK(argagg::flag_is_short("--abc") == false);
  CHECK(argagg::flag_is_short("--a-b") == false);
}


TEST_CASE("intro example")
{
  argagg::parser argparser {{
      { "help", {"-h", "--help"},
        "shows this help message", 0},
      { "delim", {"-d", "--delim"},
        "delimiter (default: ,)", 1},
      { "num", {"-n", "--num"},
        "number", 1},
    }};
  SUBCASE("example arguments") {
    std::vector<const char*> argv {
      "test", "3.141", "foo",
      "-h",
      "bar", "300",
      "-n", "100",
      "-d", "-",
      "-",
      "--",
      "-b", "--blah"};
    argagg::parser_results args;
    try {
      args = argparser.parse(argv.size(), &(argv.front()));
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
    }
    CHECK(args.has_option("help") == true);
    CHECK(static_cast<bool>(args["help"]) == true);
    CHECK(args.has_option("delim") == true);
    CHECK(static_cast<bool>(args["delim"]) == true);
    auto delim = args["delim"].as<std::string>(",");
    CHECK(delim == "-");
    CHECK(args.has_option("num") == true);
    CHECK(static_cast<bool>(args["num"]) == true);
    int x = 0;
    if (args["num"]) {
      x = args["num"];
    }
    CHECK(x == 100);
    auto y = 0.0;
    if (args.pos.size() > 0) {
      y = args.as<double>(0);
    }
    CHECK(y == doctest::Approx(3.141));
    CHECK(args.as<std::string>(1) == "foo");
    CHECK(args.as<std::string>(2) == "bar");
    CHECK(args.as<int>(3) == 300);
    CHECK(args.as<std::string>(4) == "-");
    CHECK(args.as<std::string>(5) == "-b");
    CHECK(args.as<std::string>(6) == "--blah");
  }
  SUBCASE("default delim") {
    std::vector<const char*> argv {
      "test", "3.141", "foo",
      "-h",
      "bar", "300",
      "-n", "100",
      "-",
      "--",
      "-b", "--blah"};
    argagg::parser_results args;
    try {
      args = argparser.parse(argv.size(), &(argv.front()));
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
    }
    CHECK(args.has_option("help") == true);
    CHECK(static_cast<bool>(args["help"]) == true);
    CHECK(args.has_option("delim") == false);
    CHECK(static_cast<bool>(args["delim"]) == false);
    auto delim = args["delim"].as<std::string>(",");
    CHECK(delim == ",");
    CHECK(args.has_option("num") == true);
    CHECK(static_cast<bool>(args["num"]) == true);
    int x = 0;
    if (args["num"]) {
      x = args["num"];
    }
    CHECK(x == 100);
    auto y = 0.0;
    if (args.pos.size() > 0) {
      y = args.as<double>(0);
    }
    CHECK(y == doctest::Approx(3.141));
    CHECK(args.as<std::string>(1) == "foo");
    CHECK(args.as<std::string>(2) == "bar");
    CHECK(args.as<int>(3) == 300);
    CHECK(args.as<std::string>(4) == "-");
    CHECK(args.as<std::string>(5) == "-b");
    CHECK(args.as<std::string>(6) == "--blah");
  }
  SUBCASE("underspecified delim") {
    std::vector<const char*> argv {"test", "-d"};
    argagg::parser_results args;
    CHECK_THROWS_AS({
      args = argparser.parse(argv.size(), &(argv.front()));
    }, const argagg::option_lacks_argument_error&);
  }
  SUBCASE("no arguments") {
    std::vector<const char*> argv {"test"};
    argagg::parser_results args;
    try {
      args = argparser.parse(argv.size(), &(argv.front()));
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
    }
    CHECK(args.has_option("help") == false);
    CHECK(static_cast<bool>(args["help"]) == false);
    CHECK(args.has_option("delim") == false);
    CHECK(static_cast<bool>(args["delim"]) == false);
    auto delim = args["delim"].as<std::string>(",");
    CHECK(delim == ",");
    CHECK(args.has_option("num") == false);
    CHECK(static_cast<bool>(args["num"]) == false);
    CHECK(args.pos.size() == 0);
  }
}


TEST_CASE("no definitions")
{
  argagg::parser parser {{
    }};
  SUBCASE("no arguments") {
    std::vector<const char*> argv {
      "test"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("help") == false);
    CHECK(args.has_option("verbose") == false);
    CHECK(args.has_option("output") == false);
    CHECK(args.count() == 0);
  }
  SUBCASE("with arguments") {
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
  SUBCASE("with flags") {
    std::vector<const char*> argv {
      "test", "--verbose", "-o", "baz"};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::unexpected_option_error&);
  }
  SUBCASE("invalid parser_results indexing") {
    std::vector<const char*> argv {
      "test"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK_THROWS_AS({
      args["help"];
    }, const argagg::unknown_option&);
    CHECK_THROWS_AS({
      args["verbose"];
    }, const argagg::unknown_option&);
    CHECK_THROWS_AS({
      args["output"];
    }, const argagg::unknown_option&);
  }
}


TEST_CASE("invalid definitions")
{
  std::vector<const char*> argv {
    "test"};
  SUBCASE("no flags") {
    argagg::parser parser {{
        {"bad", {}, "bad", 0},
      }};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::invalid_flag&);
  }
  SUBCASE("too short") {
    argagg::parser parser {{
        {"bad", {"-"}, "bad", 0},
      }};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::invalid_flag&);
  }
  SUBCASE("too short 2") {
    argagg::parser parser {{
        {"bad", {"a"}, "bad", 0},
      }};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::invalid_flag&);
  }
  SUBCASE("no hyphen") {
    argagg::parser parser {{
        {"bad", {"bad"}, "bad", 0},
      }};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::invalid_flag&);
  }
  SUBCASE("short flag group") {
    argagg::parser parser {{
        {"bad", {"-bad"}, "bad", 0},
      }};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::invalid_flag&);
  }
  SUBCASE("invalid character") {
    argagg::parser parser {{
        {"bad", {"-b ad"}, "bad", 0},
      }};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::invalid_flag&);
  }
  SUBCASE("too many hyphens") {
    argagg::parser parser {{
        {"bad", {"---bad"}, "bad", 0},
      }};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::invalid_flag&);
  }
  SUBCASE("long flag equal assignment") {
    argagg::parser parser {{
        {"bad", {"--bad=still-bad"}, "bad", 0},
      }};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::invalid_flag&);
  }
  SUBCASE("duplicate short flags") {
    argagg::parser parser {{
        {"bad", {"-b"}, "bad", 0},
        {"bad2", {"-b"}, "bad2", 0},
      }};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::invalid_flag&);
  }
  SUBCASE("duplicate long flags") {
    argagg::parser parser {{
        {"bad", {"--bad"}, "bad", 0},
        {"bad2", {"--bad"}, "bad2", 0},
      }};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::invalid_flag&);
  }
}


TEST_CASE("simple")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  SUBCASE("no arguments") {
    std::vector<const char*> argv {
      "test"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("help") == false);
    CHECK(args.has_option("verbose") == false);
    CHECK(args.has_option("output") == false);
    CHECK(args.count() == 0);
  }
  SUBCASE("no flags") {
    std::vector<const char*> argv {
      "test", "foo", "bar", "baz"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("help") == false);
    CHECK(args.has_option("verbose") == false);
    CHECK_THROWS_AS({
      args["verbose"].as<int>();
    }, const std::out_of_range&);
    CHECK(args["verbose"].as<int>(999) == 999);
    CHECK(args.has_option("output") == false);
    CHECK(args.count() == 3);
    CHECK(args.as<std::string>(0) == "foo");
    CHECK(args.as<std::string>(1) == "bar");
    CHECK(args.as<std::string>(2) == "baz");
  }
  SUBCASE("only flags") {
    std::vector<const char*> argv {
      "test", "--verbose", "--output", "foo", "-v", "-o", "bar"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("help") == false);
    CHECK(args.has_option("verbose") == true);
    CHECK(args["verbose"].count() == 2);
    CHECK(args["verbose"][0].arg == nullptr);
    CHECK(args["verbose"][1].arg == nullptr);
    CHECK_THROWS_AS({
      args["verbose"][0].as<std::string>();
    }, const argagg::option_lacks_argument_error&);
    CHECK_THROWS_AS({
      args["verbose"][0].as<int>();
    }, const argagg::option_lacks_argument_error&);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].count() == 2);
    CHECK(args["output"][0].as<std::string>() == "foo");
    CHECK(args["output"][1].as<std::string>() == "bar");
    CHECK(args.count() == 0);
  }
  SUBCASE("simple mixed") {
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
  SUBCASE("trailing flags") {
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
  SUBCASE("interleaved positional arguments") {
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
  SUBCASE("unused short flag") {
    std::vector<const char*> argv {
      "test", "--output", "foo", "-h", "bar", "-v"};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::unexpected_option_error&);
  }
  SUBCASE("unused long flag") {
    std::vector<const char*> argv {
      "test", "--output", "foo", "--help", "bar", "-v"};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::unexpected_option_error&);
  }
}


TEST_CASE("long flag equal format for arguments")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"delim", {"-d", "--delim"}, "delimiter", 1},
      {"output", {"-o", "--output"}, "output", 1},
    }};
  SUBCASE("basic") {
    std::vector<const char*> argv {
      "test", "-v", "--output=foo", "--delim=bar", "baz"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == true);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].as<std::string>() == "foo");
    CHECK(args.has_option("delim") == true);
    CHECK(args["delim"].as<std::string>() == "bar");
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "baz");
  }
  SUBCASE("empty") {
    std::vector<const char*> argv {
      "test", "-v", "--output=", "--delim=", "baz"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == true);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].as<std::string>() == "");
    CHECK(args.has_option("delim") == true);
    CHECK(args["delim"].as<std::string>() == "");
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "baz");
  }
  SUBCASE("symbols") {
    std::vector<const char*> argv {
      "test", "-v", "--output=--foo!!", "--delim=,", "baz"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == true);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].as<std::string>() == "--foo!!");
    CHECK(args.has_option("delim") == true);
    CHECK(args["delim"].as<std::string>() == ",");
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "baz");
  }
  SUBCASE("unnecessary argument") {
    std::vector<const char*> argv {
      "test", "--verbose=bad"};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::unexpected_argument_error&);
  }
}


TEST_CASE("short flag groups")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"help", {"-h", "--help"}, "help", 0},
      {"foo", {"-f", "--foo"}, "foo", 0},
      {"output", {"-o", "--output"}, "output", 1},
    }};
  SUBCASE("basic") {
    std::vector<const char*> argv {
      "test", "-vhf", "bar"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == true);
    CHECK(args.has_option("help") == true);
    CHECK(args.has_option("foo") == true);
    CHECK(args.has_option("output") == false);
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "bar");
  }
  SUBCASE("basic 2") {
    std::vector<const char*> argv {
      "test", "-fvh", "bar"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == true);
    CHECK(args.has_option("help") == true);
    CHECK(args.has_option("foo") == true);
    CHECK(args.has_option("output") == false);
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "bar");
  }
  SUBCASE("basic 3") {
    std::vector<const char*> argv {
      "test", "-fh", "-v", "bar"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == true);
    CHECK(args.has_option("help") == true);
    CHECK(args.has_option("foo") == true);
    CHECK(args.has_option("output") == false);
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "bar");
  }
  SUBCASE("basic 4") {
    std::vector<const char*> argv {
      "test", "--vfh", "bar"};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::unexpected_option_error&);
  }
  SUBCASE("unexpected symbol") {
    std::vector<const char*> argv {
      "test", "-v-fh", "bar"};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const std::domain_error&);
  }
  SUBCASE("trailing flag with argument") {
    std::vector<const char*> argv {
      "test", "-vhfo", "bar"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == true);
    CHECK(args.has_option("help") == true);
    CHECK(args.has_option("foo") == true);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].as<std::string>() == "bar");
    CHECK(args.count() == 0);
  }
  SUBCASE("leading flag with argument") {
    std::vector<const char*> argv {
      "test", "-ohfv", "bar"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == false);
    CHECK(args.has_option("help") == false);
    CHECK(args.has_option("foo") == false);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].as<std::string>() == "hfv");
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "bar");
  }
  SUBCASE("middling flag with argument") {
    std::vector<const char*> argv {
      "test", "-vfoh", "bar"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == true);
    CHECK(args.has_option("help") == false);
    CHECK(args.has_option("foo") == true);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].as<std::string>() == "h");
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "bar");
  }
}


TEST_CASE("flag stop")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"delim", {"-d", "--delim"}, "delimiter", 1},
    }};
  SUBCASE("ignore flags after stop") {
    std::vector<const char*> argv {
      "test", "-v", "--", "bar", "--verbose", "baz",
      "--delim", "dog", "-d", "cat"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("help") == false);
    CHECK(args.has_option("verbose") == true);
    CHECK(args["verbose"].count() == 1);
    CHECK(args["verbose"][0].arg == nullptr);
    CHECK(args.has_option("output") == false);
    CHECK(args.count() == 7);
    CHECK(args.as<std::string>(0) == "bar");
    CHECK(args.as<std::string>(1) == "--verbose");
    CHECK(args.as<std::string>(2) == "baz");
    CHECK(args.as<std::string>(3) == "--delim");
    CHECK(args.as<std::string>(4) == "dog");
    CHECK(args.as<std::string>(5) == "-d");
    CHECK(args.as<std::string>(6) == "cat");
  }
  SUBCASE("flag stop consumed as argument for option") {
    std::vector<const char*> argv {
      "test", "-d", "--", "--", "-", "boo"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("help") == false);
    CHECK(args.has_option("verbose") == false);
    CHECK(args.has_option("delim") == true);
    CHECK(args["delim"].as<std::string>() == "--");
    CHECK(args.count() == 2);
    CHECK(args.as<std::string>(0) == "-");
    CHECK(args.as<std::string>(1) == "boo");
  }
}


TEST_CASE("option requires argument")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"number", {"-n", "--number"}, "number", 1},
    }};
  SUBCASE("arguments provided") {
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
  SUBCASE("negative numbers") {
    std::vector<const char*> argv {
      "test", "-n", "-1", "-n", "-4444", "--", "-22"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == false);
    CHECK(args.has_option("number") == true);
    CHECK(args["number"].count() == 2);
    CHECK(args["number"][0].as<int>() == -1);
    CHECK(args["number"][1].as<int>() == -4444);
    CHECK(args.count() == 1);
    CHECK(args.as<int>(0) == -22);
  }
  SUBCASE("use flag as argument even though it is a valid flag")
  {
    std::vector<const char*> argv {
      "test", "-n", "-v"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    CHECK(args["number"].count() == 1);
    CHECK(args["number"][0].as<std::string>() == "-v");
  }
  SUBCASE("interrupted by unused flag")
  {
    std::vector<const char*> argv {
      "test", "-n", "1", "2", "-c"};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::unexpected_option_error&);
  }
  SUBCASE("given zero, end of args") {
    std::vector<const char*> argv {
      "test", "-n"};
    CHECK_THROWS_AS({
      argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    }, const argagg::option_lacks_argument_error&);
  }
}


TEST_CASE("greedy processing")
{
  argagg::parser parser {{
      {"a", {"-a"}, "a", 0},
      {"b", {"-b", "--bar"}, "b", 0},
      {"c", {"-c"}, "c", 0},
      {"output", {"-o", "--output"}, "output", 1},
    }};
  SUBCASE("short group example 1") {
    std::vector<const char*> argv {
      "test", "-abco", "foo"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("a") == true);
    CHECK(args.has_option("b") == true);
    CHECK(args.has_option("c") == true);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].as<std::string>() == "foo");
    CHECK(args.count() == 0);
  }
  SUBCASE("short group example 2") {
    std::vector<const char*> argv {
      "test", "-aboc", "foo"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("a") == true);
    CHECK(args.has_option("b") == true);
    CHECK(args.has_option("c") == false);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].as<std::string>() == "c");
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "foo");
  }
  SUBCASE("short group example 3") {
    std::vector<const char*> argv {
      "test", "-aobc", "foo"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("a") == true);
    CHECK(args.has_option("b") == false);
    CHECK(args.has_option("c") == false);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].as<std::string>() == "bc");
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "foo");
  }
  SUBCASE("short group example 4") {
    std::vector<const char*> argv {
      "test", "-oabc", "foo"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("a") == false);
    CHECK(args.has_option("b") == false);
    CHECK(args.has_option("c") == false);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].as<std::string>() == "abc");
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "foo");
  }
  SUBCASE("long example 1") {
    std::vector<const char*> argv {
      "test", "--output=foo", "--", "--bar"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("a") == false);
    CHECK(args.has_option("b") == false);
    CHECK(args.has_option("c") == false);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].as<std::string>() == "foo");
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "--bar");
  }
  SUBCASE("long example 2") {
    std::vector<const char*> argv {
      "test", "--output", "--", "--bar"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("a") == false);
    CHECK(args.has_option("b") == true);
    CHECK(args.has_option("c") == false);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].as<std::string>() == "--");
    CHECK(args.count() == 0);
  }
  SUBCASE("long example 3") {
    std::vector<const char*> argv {
      "test", "--output", "--bar"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("a") == false);
    CHECK(args.has_option("b") == false);
    CHECK(args.has_option("c") == false);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].as<std::string>() == "--bar");
    CHECK(args.count() == 0);
  }
}


TEST_CASE("gcc example")
{
  argagg::parser parser {{
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"version", {"--version"}, "print version", 0},
      {"include path", {"-I"}, "include path", 1},
      {"library path", {"-L"}, "library path", 1},
      {"library", {"-l"}, "library", 1},
      {"output", {"-o"}, "output", 1},
    }};
  SUBCASE("version") {
    std::vector<const char*> argv {
      "gcc", "--version"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == false);
    CHECK(args.has_option("version") == true);
    CHECK(args.has_option("include path") == false);
    CHECK(args.has_option("library path") == false);
    CHECK(args.has_option("library") == false);
    CHECK(args.has_option("output") == false);
    CHECK(args.count() == 0);
  }
  SUBCASE("simple") {
    std::vector<const char*> argv {
      "gcc", "test.c"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == false);
    CHECK(args.has_option("version") == false);
    CHECK(args.has_option("include path") == false);
    CHECK(args.has_option("library path") == false);
    CHECK(args.has_option("library") == false);
    CHECK(args.has_option("output") == false);
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "test.c");
  }
  SUBCASE("simple 2") {
    std::vector<const char*> argv {
      "gcc", "-I/usr/local/include", "test.c", "-otest"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == false);
    CHECK(args.has_option("version") == false);
    CHECK(args.has_option("include path") == true);
    CHECK(args["include path"].count() == 1);
    CHECK(args["include path"].as<std::string>() == "/usr/local/include");
    CHECK(args.has_option("library path") == false);
    CHECK(args.has_option("library") == false);
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].count() == 1);
    CHECK(args["output"].as<std::string>() == "test");
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "test.c");
  }
  SUBCASE("simple 3") {
    std::vector<const char*> argv {
      "gcc", "-I/usr/local/include", "-I.", "-L/usr/local/lib", "-lz", "-lm",
      "test.c", "-otest"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("verbose") == false);
    CHECK(args.has_option("version") == false);
    CHECK(args.has_option("include path") == true);
    CHECK(args["include path"].count() == 2);
    CHECK(args["include path"][0].as<std::string>() == "/usr/local/include");
    CHECK(args["include path"][1].as<std::string>() == ".");
    CHECK(args.has_option("library path") == true);
    CHECK(args["library path"].count() == 1);
    CHECK(args["library path"][0].as<std::string>() == "/usr/local/lib");
    CHECK(args.has_option("library") == true);
    CHECK(args["library"].count() == 2);
    CHECK(args["library"][0].as<std::string>() == "z");
    CHECK(args["library"][1].as<std::string>() == "m");
    CHECK(args.has_option("output") == true);
    CHECK(args["output"].count() == 1);
    CHECK(args["output"].as<std::string>() == "test");
    CHECK(args.count() == 1);
    CHECK(args.as<std::string>(0) == "test.c");
  }
}


TEST_CASE("argument conversions")
{
  argagg::parser parser {{
      {"number", {"-n", "--num", "--number"}, "number", 1},
    }};
  SUBCASE("positional integer") {
    std::vector<const char*> argv {
      "test", "1", "2"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.pos.size() == 2);
    CHECK(args.as<char>() == 1);
    CHECK(args.as<unsigned char>() == 1);
    CHECK(args.as<signed char>() == 1);
    CHECK(args.as<short>() == 1);
    CHECK(args.as<unsigned short>() == 1);
    CHECK(args.as<signed short>() == 1);
    CHECK(args.as<int>() == 1);
    CHECK(args.as<unsigned int>() == 1);
    CHECK(args.as<signed int>() == 1);
    CHECK(args.as<long>() == 1);
    CHECK(args.as<unsigned long>() == 1);
    CHECK(args.as<signed long>() == 1);
    CHECK(args.as<long long>() == 1);
    CHECK(args.as<unsigned long long>() == 1);
    CHECK(args.as<signed long long>() == 1);
    CHECK(std::strcmp(args.as<const char*>(), "1") == 0);
    CHECK(args.as<char>(1) == 2);
    CHECK(args.as<unsigned char>(1) == 2);
    CHECK(args.as<signed char>(1) == 2);
    CHECK(args.as<short>(1) == 2);
    CHECK(args.as<unsigned short>(1) == 2);
    CHECK(args.as<signed short>(1) == 2);
    CHECK(args.as<int>(1) == 2);
    CHECK(args.as<unsigned int>(1) == 2);
    CHECK(args.as<signed int>(1) == 2);
    CHECK(args.as<long>(1) == 2);
    CHECK(args.as<unsigned long>(1) == 2);
    CHECK(args.as<signed long>(1) == 2);
    CHECK(args.as<long long>(1) == 2);
    CHECK(args.as<unsigned long long>(1) == 2);
    CHECK(args.as<signed long long>(1) == 2);
    CHECK(args.as<std::string>(1) == "2");
    CHECK(std::strcmp(args.as<const char*>(1), "2") == 0);
  }
  SUBCASE("positional floating point") {
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
  SUBCASE("positional vector") {
    std::vector<const char*> argv {
      "test", "0", "1", "2"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.pos.size() == 3);
    auto v = args.all_as<int>();
    CHECK(v[0] == 0);
    CHECK(v[1] == 1);
    CHECK(v[2] == 2);
  }
  SUBCASE("option integer") {
    std::vector<const char*> argv {
      "test", "-n", "1"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    CHECK(args["number"].count() == 1);
    CHECK(args["number"].as<char>() == 1);
    CHECK(args["number"].as<unsigned char>() == 1);
    CHECK(args["number"].as<signed char>() == 1);
    CHECK(args["number"].as<short>() == 1);
    CHECK(args["number"].as<unsigned short>() == 1);
    CHECK(args["number"].as<signed short>() == 1);
    CHECK(args["number"].as<int>() == 1);
    CHECK(args["number"].as<unsigned int>() == 1);
    CHECK(args["number"].as<signed int>() == 1);
    CHECK(args["number"].as<long>() == 1);
    CHECK(args["number"].as<unsigned long>() == 1);
    CHECK(args["number"].as<signed long>() == 1);
    CHECK(args["number"].as<long long>() == 1);
    CHECK(args["number"].as<unsigned long long>() == 1);
    CHECK(args["number"].as<signed long long>() == 1);
    CHECK(args["number"].as<std::string>() == "1");
  }
  SUBCASE("option floating point") {
    std::vector<const char*> argv {
      "test", "-n", "3.141592653"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    CHECK(args["number"].count() == 1);
    CHECK(args["number"].as<float>() == doctest::Approx(3.141592653f));
    CHECK(args["number"].as<double>() == doctest::Approx(3.141592653));
    CHECK(args["number"].as<std::string>() == "3.141592653");
  }
  SUBCASE("option implicit conversions") {
    std::vector<const char*> argv {
      "test", "-n", "3.141592653", "-n", "2"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    if (args["number"]) {
    } else {
      throw std::runtime_error("implicit boolean conversion failed");
    }
    if (!args["number"]) {
      throw std::runtime_error("unary ! operator failed");
    } else {
    }
    CHECK(args["number"].count() == 2);
    float x = args["number"][0];
    int y = args["number"][1];
    CHECK(x == doctest::Approx(3.141592653f));
    CHECK(y == 2);
  }
  SUBCASE("exception on bad conversion") {
    std::vector<const char*> argv {
      "test", "-n", "not-an-number"};
    argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("number") == true);
    CHECK(args["number"].count() == 1);
    CHECK_THROWS_AS({
      args["number"].as<int>();
    }, const std::invalid_argument&);
    CHECK_THROWS_AS({
      args["number"].as<double>();
    }, const std::invalid_argument&);
    CHECK(args["number"].as<int>(-1) == -1);
    CHECK(args["number"].as<double>(3.141) == doctest::Approx(3.141));
  }
}


// Define a custom conversion function for the test that follows
namespace argagg {
namespace convert {
  template <>
  std::vector<std::string> arg(const char* s)
  {
    std::vector<std::string> ret {};
    if (std::strlen(s) == 0) {
      return ret;
    }
    while (true) {
      const char* token = std::strchr(s, ',');
      if (token == nullptr) {
        ret.emplace_back(s, std::strlen(s));
        break;
      }
      std::size_t len = static_cast<std::size_t>(token - s);
      ret.emplace_back(s, len);
      s += len + 1;
    }
    return ret;
  }
} // namespace convert
} // namespace argagg


TEST_CASE("custom conversion function")
{
  argagg::parser parser {{
      {"words", {"-w", "--words"}, "words", 1},
    }};
  std::vector<const char*> argv {
    "test", "-w", "hello,world,foo,bar,baz"};
  argagg::parser_results args = parser.parse(argv.size(), &(argv.front()));
  CHECK(args.has_option("words") == true);
  auto v = args["words"].as<std::vector<std::string>>();
  CHECK(v.size() == 5);
  CHECK(v[0] == "hello");
  CHECK(v[1] == "world");
  CHECK(v[2] == "foo");
  CHECK(v[3] == "bar");
  CHECK(v[4] == "baz");
}


// Define a custom conversion function for the test that follows
struct position3 {
  double x;
  double y;
  double z;
};
namespace argagg {
namespace convert {
  template <>
  position3 arg(const char* s)
  {
    position3 result {0.0, 0.0, 0.0};
    if (!parse_next_component(s, result.x)) {
      // could potentially throw an error if you require that at least two
      // components exist in the list
      return result;
    }
    if (!parse_next_component(s, result.y)) {
      return result;
    }
    if (!parse_next_component(s, result.z)) {
      return result;
    }
    return result;
  }
} // namespace convert
} // namespace argagg


TEST_CASE("parse_next_component() example")
{
  argagg::parser argparser {{
      { "origin", {"-o", "--origin"},
        "origin as position3 specifieid as a comma separated list of "
        "components (e.g. '1,2,3')", 1},
    }};
  SUBCASE("three components") {
    std::vector<const char*> argv {
      "test", "-o", "1.2,3.45,6.789"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("origin") == true);
    auto origin = args["origin"].as<position3>();
    CHECK(origin.x == 1.2);
    CHECK(origin.y == 3.45);
    CHECK(origin.z == 6.789);
  }
  SUBCASE("two components") {
    std::vector<const char*> argv {
      "test", "-o", "1.2,3.45"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("origin") == true);
    auto origin = args["origin"].as<position3>();
    CHECK(origin.x == 1.2);
    CHECK(origin.y == 3.45);
    CHECK(origin.z == 0.0);
  }
  SUBCASE("one component") {
    std::vector<const char*> argv {
      "test", "-o", "1.23456789"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("origin") == true);
    auto origin = args["origin"].as<position3>();
    CHECK(origin.x == 1.23456789);
    CHECK(origin.y == 0.0);
    CHECK(origin.z == 0.0);
  }
}


// Define a custom conversion function for the test that follows
struct velocity3 {
  double x;
  double y;
  double z;
};
namespace argagg {
namespace convert {
  template <>
  velocity3 arg(const char* s)
  {
    velocity3 result {0.0, 0.0, 0.0};
    if (!parse_next_component(s, result.x, '+')) {
      // could potentially throw an error if you require that at least two
      // components exist in the list
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


TEST_CASE("parse_next_component() example with non-default delimiter")
{
  argagg::parser argparser {{
      { "velocity", {"--velocity"},
        "velocity as velocity3 specifieid as a plus ('+') separated list of "
        "components (e.g. '1+2+3')", 1},
    }};
  SUBCASE("three components") {
    std::vector<const char*> argv {
      "test", "--velocity", "1.2+3.45+6.789"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("velocity") == true);
    auto velocity = args["velocity"].as<velocity3>();
    CHECK(velocity.x == 1.2);
    CHECK(velocity.y == 3.45);
    CHECK(velocity.z == 6.789);
  }
  SUBCASE("two components") {
    std::vector<const char*> argv {
      "test", "--velocity", "1.2+3.45"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("velocity") == true);
    auto velocity = args["velocity"].as<velocity3>();
    CHECK(velocity.x == 1.2);
    CHECK(velocity.y == 3.45);
    CHECK(velocity.z == 0.0);
  }
  SUBCASE("one component") {
    std::vector<const char*> argv {
      "test", "--velocity", "1.23456789"};
    argagg::parser_results args =
      argparser.parse(argv.size(), &(argv.front()));
    CHECK(args.has_option("velocity") == true);
    auto velocity = args["velocity"].as<velocity3>();
    CHECK(velocity.x == 1.23456789);
    CHECK(velocity.y == 0.0);
    CHECK(velocity.z == 0.0);
  }
}


TEST_CASE("write options help")
{
  argagg::parser parser {{
      {"help", {"-h", "--help"}, "print help", 0},
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};
  // Just checking for no exceptions for now.
  std::cout << parser;
}


static const std::string ipsum =
  "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod "
  "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam"
  ", quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
  "consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse "
  "cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat "
  "non proident, sunt in culpa qui officia deserunt mollit anim id est "
  "laborum.";


static const std::string fmt_ipsum =
  "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod\n"
  "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim\n"
  "veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea\n"
  "commodo consequat. Duis aute irure dolor in reprehenderit in voluptate\n"
  "velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat\n"
  "cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id\n"
  "est laborum.\n";


TEST_CASE("fmt_ostream")
{
  std::ostringstream os;
  {
    argagg::fmt_ostream test(os);
    test << ipsum;
  }
  CHECK(os.str() == fmt_ipsum);
}


TEST_CASE("fmt_string")
{
  std::string test_formatted = argagg::fmt_string(ipsum);
  CHECK(test_formatted == fmt_ipsum);
}


TEST_CASE("fmt_string trailing whitespace")
{
  const auto test_formatted = argagg::fmt_string("  abc  \t ");
  CHECK(test_formatted == "  abc\n");
}


TEST_CASE("fmt_string indent")
{
  const auto test_formatted = argagg::fmt_string(
    "        Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
    "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
  CHECK(
    test_formatted ==
    "        Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do\n"
    "        eiusmod tempor incididunt ut labore et dolore magna aliqua.\n");
}
