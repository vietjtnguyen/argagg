/*
 * @file
 * @brief
 * Defines the argagg::comma_separated_strings type and an argument conversion
 * specialization that parses an argument as an argagg::comma_separated_strings.
 *
 * @copyright
 * Copyright (c) 2018 Viet The Nguyen
 *
 * @copyright
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * @copyright
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * @copyright
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#pragma once
#ifndef ARGAGG_ARGAGG_CONVERT_COMMA_SEPARATED_VALUES_HPP
#define ARGAGG_ARGAGG_CONVERT_COMMA_SEPARATED_VALUES_HPP

#include "../argagg.hpp"

#include <string>
#include <vector>


namespace argagg {

/**
 * @brief
 * Represents a list of comma-separated strings. This is defined as a new type
 * to make creating a specialization of argagg::convert::arg() easier.
 */
struct comma_separated_strings {
  std::vector<std::string> values;
};

namespace convert {

  template <>
  comma_separated_strings arg(const char* s);

} // namespace convert

} // namespace argagg


// ---- end of declarations, header-only implementations follow ----


namespace argagg {
namespace convert {


template <> inline
comma_separated_strings arg(const char* s)
{
  comma_separated_strings result {{}};
  std::string value;
  while (parse_next_component(s, value, ',')) {
    result.values.emplace_back(std::move(value));
  }
  result.values.emplace_back(std::move(value));
  return result;
}


} // namespace convert
} // namespace argagg


#endif // ARGAGG_ARGAGG_CONVERT_COMMA_SEPARATED_VALUES_HPP
