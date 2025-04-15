#include <sstream>
#include <string>

template<typename T>
std::string issue_39_log(T param) {
  std::ostringstream oss;
  oss << param;
  return oss.str();
}

#include "../include/argagg/argagg.hpp"

#include "doctest.h"


TEST_CASE("issue 39")
{
  const argagg::parser parser {{
      {"help", {"-h", "--help"}, "print help", 0},
      {"verbose", {"-v", "--verbose"}, "be verbose", 0},
      {"output", {"-o", "--output"}, "output filename", 1},
    }};

  const std::string help_text = issue_39_log(parser);
  CHECK(
    help_text ==
    "    -h, --help\n"
    "        print help\n"
    "    -v, --verbose\n"
    "        be verbose\n"
    "    -o, --output\n"
    "        output filename\n");
}
