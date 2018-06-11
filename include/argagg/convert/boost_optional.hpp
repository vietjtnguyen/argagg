/*
 * @file
 * @brief
 * Defines argument conversion specializations for OpenCV.
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
#ifndef ARGAGG_ARGAGG_CONVERT_BOOST_OPTIONAL_HPP
#define ARGAGG_ARGAGG_CONVERT_BOOST_OPTIONAL_HPP

#include "../argagg.hpp"

#include <boost/optional.hpp>
#include <boost/none.hpp>

#include <vector>


namespace argagg {
namespace convert {

  /**
   * @brief
   * Partially specializes @ref argagg::convert::converter for the @ref
   * boost::optional<T> type. This basically passes through to
   * argagg::convert::arg<T>() but returns an empty optional if a conversion
   * error is encountered.
   */
  template <typename T>
  struct converter<boost::optional<T>> {
    static boost::optional<T> convert(const char* s);
  };

} // namespace convert

} // namespace argagg


// ---- end of declarations, header-only implementations follow ----


namespace argagg {
namespace convert {


template <typename T>
boost::optional<T>
converter<boost::optional<T>>::convert(const char* s)
try {
  return argagg::convert::arg<T>(s);
} catch (...) {
  return boost::none;
}


} // namespace convert
} // namespace argagg


#endif // ARGAGG_ARGAGG_CONVERT_BOOST_OPTIONAL_HPP
