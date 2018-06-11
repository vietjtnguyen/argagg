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
#ifndef ARGAGG_ARGAGG_CONVERT_OPENCV_HPP
#define ARGAGG_ARGAGG_CONVERT_OPENCV_HPP

#include "../argagg.hpp"

#include <opencv2/opencv.hpp>

#include <vector>


namespace argagg {
namespace convert {

  /**
   * @brief
   * Partially specializes @ref argagg::convert::converter for the @ref
   * cv::Point_ type. Parses as a comma separated list of components.
   */
  template <typename T>
  struct converter<cv::Point_<T>> {
    static cv::Point_<T> convert(const char* s);
  };

  /**
   * @brief
   * Partially specializes @ref argagg::convert::converter for the @ref
   * cv::Point3_ type. Parses as a comma separated list of components.
   */
  template <typename T>
  struct converter<cv::Point3_<T>> {
    static cv::Point3_<T> convert(const char* s);
  };

  /**
   * @brief
   * Partially specializes @ref argagg::convert::converter for the @ref
   * cv::Size_ type. Parses as a 'x' separated pair of width and
   * height.
   */
  template <typename T>
  struct converter<cv::Size_<T>> {
    static cv::Size_<T> convert(const char* s);
  };

  /**
   * @brief
   * Partially specializes @ref argagg::convert::converter for the @ref
   * cv::Rect_ type. Parses as a comma separated list of x, y, width, height.
   */
  template <typename T>
  struct converter<cv::Rect_<T>> {
    static cv::Rect_<T> convert(const char* s);
  };

} // namespace convert

} // namespace argagg


// ---- end of declarations, header-only implementations follow ----


namespace argagg {
namespace convert {


template <typename T>
cv::Point_<T>
converter<cv::Point_<T>>::convert(const char* s)
{
  cv::Point_<T> result {0, 0};
  if (!parse_next_component(s, result.x)) {
    return result;
  }
  if (!parse_next_component(s, result.y)) {
    return result;
  }
  return result;
}


template <typename T>
cv::Point3_<T>
converter<cv::Point3_<T>>::convert(const char* s)
{
  cv::Point3_<T> result {0, 0, 0};
  if (!parse_next_component(s, result.x)) {
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


template <typename T>
cv::Size_<T>
converter<cv::Size_<T>>::convert(const char* s)
{
  cv::Size_<T> result {0, 0};
  if (!parse_next_component(s, result.width, 'x')) {
    return result;
  }
  if (!parse_next_component(s, result.height, 'x')) {
    return result;
  }
  return result;
}


template <typename T>
cv::Rect_<T>
converter<cv::Rect_<T>>::convert(const char* s)
{
  cv::Rect_<T> result {0, 0, 0, 0};
  if (!parse_next_component(s, result.x)) {
    return result;
  }
  if (!parse_next_component(s, result.y)) {
    return result;
  }
  if (!parse_next_component(s, result.width)) {
    return result;
  }
  if (!parse_next_component(s, result.height)) {
    return result;
  }
  return result;
}


} // namespace convert
} // namespace argagg


#endif // ARGAGG_ARGAGG_CONVERT_OPENCV_HPP
