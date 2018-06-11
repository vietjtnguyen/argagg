/*
 * @file
 * @brief
 * Defines the argagg::csv type and an argument conversion specialization that
 * parses an argument as an argagg::csv.
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
#ifndef ARGAGG_ARGAGG_CONVERT_CSV_HPP
#define ARGAGG_ARGAGG_CONVERT_CSV_HPP

#include "../argagg.hpp"

#include <vector>


namespace argagg {

/**
 * @brief
 * Represents a list of comma-separated values. This is defined as a new type
 * to embed the delimiter semantics into the type (if it was just a std::vector
 * then it's not clear what the delimiter is).
 */
template <typename T>
struct csv {
  std::vector<T> values;
};

namespace convert {

  /**
   * @brief
   * Partially specializes @ref argagg::convert::converter for the @ref
   * argagg::csv type.
   */
  template <typename T>
  struct converter<csv<T>> {
    static csv<T> convert(const char* s);
  };

} // namespace convert

} // namespace argagg


// ---- end of declarations, header-only implementations follow ----


namespace argagg {
namespace convert {


template <typename T>
csv<T>
converter<csv<T>>::convert(const char* s)
{
  csv<T> result {{}};
  T value;
  while (parse_next_component(s, value, ',')) {
    result.values.emplace_back(std::move(value));
  }
  result.values.emplace_back(std::move(value));
  return result;
}


} // namespace convert
} // namespace argagg


#endif // ARGAGG_ARGAGG_CONVERT_CSV_HPP
