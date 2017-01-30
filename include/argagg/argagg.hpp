/*
 * @file
 * @brief
 * Defines a very simple command line argument parser.
 *
 * @copyright
 * Copyright (c) 2017 Viet The Nguyen
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
#ifndef ARGAGG_ARGAGG_ARGAGG_HPP
#define ARGAGG_ARGAGG_ARGAGG_HPP

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>


namespace argagg {


/**
 * @brief
 * This exception is thrown when a flag is parsed unexpected such as when an
 * argument was expected for a previous flag or if a flag was found that has
 * not been defined.
 */
struct unexpected_flag_error
: public std::invalid_argument {

  /**
   * @brief
   * Explicit constructor which passes "what" string argument to
   * std::invalid_argument constructor.
   */
  explicit unexpected_flag_error(const std::string& what)
  : std::invalid_argument(what)
  {
  }

};


/**
 * @brief
 * The set of template instantiations that convert C-strings to other types for
 * the flag::as(), flag::all_as(), args::as(), and args::all_as() methods are
 * placed in this namespace.
 */
namespace convert {

  /**
   * @brief
   * Explicit instantiations of this function are used to convert arguments to
   * types.
   */
  template <typename T>
  T arg(const char* arg);

}


/**
 * @brief
 * Contains the parser results for a flag.
 */
struct flag {

  /**
   * @brief
   * Converts the first argument associated with this flag into the given type
   * using the conversion functions in the arg namespace. If there are no
   * arguments then an out of bounds access on the args vector will occur.
   */
  template <typename T>
  T as(std::size_t i = 0) const
  {
    return convert::arg<T>(this->args[i]);
  }

  /**
   * @brief
   * Converts all arguments associated with this flag into a vector of the
   * given type using the conversion functions in the arg namespace.
   */
  template <typename T>
  std::vector<T> all_as() const
  {
    std::vector<T> v(this->args.size());
    std::transform(
      this->args.begin(), this->args.end(), v.begin(),
      [](const char* arg) {
        return convert::arg<T>(arg);
      });
    return v;
  }

  /**
   * @brief
   * Number of times this flag occurs.
   */
  int occurences;

  /**
   * @brief
   * Arguments parsed for this flag.
   */
  std::vector<const char*> args;

};


/**
 * @brief
 * Represents the results of the parser including flags, flag arguments, and
 * positional arguments.
 */
struct args {

  /**
   * @brief
   * Used to check if a flag was specified at all.
   */
  bool has_flag(const std::string& name) const
  {
    return this->flags.find(name) != this->flags.end();
  }

  /**
   * @brief
   * Get the parser results for the given flag. If the flag was not parsed at
   * all then the exception from the unordered_map will bubble through so check
   * if the flag exists in the first place with has_flag().
   */
  flag& operator [] (const std::string& name)
  {
    return this->flags.at(name);
  }

  /**
   * @brief
   * Get the parser results for the given flag. If the flag was not parsed at
   * all then the exception from the unordered_map will bubble through so check
   * if the flag exists in the first place with has_flag().
   */
  const flag& operator [] (const std::string& name) const
  {
    return this->flags.at(name);
  }

  /**
   * @brief
   * Gets the number of positional arguments.
   */
  std::size_t count() const
  {
    return this->args.size();
  }

  /**
   * @brief
   * Gets a positional argument by index.
   */
  const char* operator [] (std::size_t index) const
  {
    return this->args[index];
  }

  /**
   * @brief
   * Gets a positional argument converted to the type.
   */
  template <typename T>
  T as(std::size_t i = 0) const
  {
    return convert::arg<T>(this->args[i]);
  }

  /**
   * @brief
   * Gets all positional arguments converted to the given type.
   */
  template <typename T>
  std::vector<T> all_as() const
  {
    std::vector<T> v(this->args.size());
    std::transform(
      this->args.begin(), this->args.end(), v.begin(),
      [](const char* arg) {
        return convert::arg<T>(arg);
      });
    return v;
  }

  /**
   * @brief
   * Returns the name of the program from the original arguments list.
   */
  const char* program;

  /**
   * @brief
   * Maps from flag name to the structure which contains the parser results for
   * that flag.
   */
  std::unordered_map<std::string, flag> flags;

  /**
   * @brief
   * Vector of positional arguments.
   */
  std::vector<const char*> args;

};


/**
 * @brief
 * A flag specification which essentially represents what a flag is, not a
 * parsed flag.
 */
struct flag_spec {

  /**
   * @brief
   * Name of the flag. Flag parser results are keyed by this name.
   */
  const std::string name;

  /**
   * @brief
   * List of strings to match that correspond to this flag. Should be fully
   * specified with dashes (e.g. "-v" or "--verbose") but this is not enforced.
   */
  std::vector<const char*> flags;

  /**
   * @brief
   * Help string for this flag.
   */
  const char* help;

  /**
   * @brief
   * Number of arguments this flag requires. A positive number means this flag
   * explicitly requires exactly that many arguments. Use 0 for flags that
   * require no arguments. Negative numbers are undefined except for the
   * flag_spec::zero_plus, flag_spec::one_plus, and flag_spec::optional values.
   */
  int num_args;

  /**
   * @brief
   * A sentinel value that num_args can be set to. When used it means that this
   * flag expects zero or more arguments.
   */
  constexpr static int zero_plus = -1;

  /**
   * @brief
   * A sentinel value that num_args can be set to. When used it means that this
   * flag expects one or more arguments and causes a parse failure when no
   * arguments are provided to the flag.
   */
  constexpr static int one_plus = -2;

  /**
   * @brief
   * A sentinal value that num_args can be set to. When used it means that this
   * flag expects zero or one arguments.
   */
  constexpr static int optional = -3;

};


/**
 * @brief
 * A list of flag specifications used to inform how to parse arguments.
 */
struct parser {

  /**
   * @brief
   * Parses the provided command line arguments and returns the args result
   * structure.
   */
  args parse(int argc, const char** argv)
  {
    bool ignore_flags = false;
    flag* last_flag_expecting_args = nullptr;
    int num_flag_args_to_consume = 0;

    args arg_res { argv[0], {}, {} };

    const char** arg_i = argv + 1;
    const char** arg_end = argv + argc;

    while (arg_i != arg_end) {

      // If we're not ignoring flags then check to see if this argument looks
      // like a flag_spec that has been specified...
      auto matching_flag = ignore_flags ? specs.end() :
        std::find_if(
          this->specs.begin(), this->specs.end(),
          [=](const flag_spec& spec) {
            // ...and it looks like a flag_spec that has been specified if the
            // argument matches "any of" the flag strings for a given
            // flag_spec.
            return std::any_of(
              spec.flags.begin(), spec.flags.end(),
              [=](const char* flag) {
                return std::strcmp(flag, *arg_i) == 0;
              });
          });

      if (matching_flag == specs.end()) {
        // If we are ignoring flags or the argument didn't match a flag_spec
        // that was specified...

        auto arg_i_str = *arg_i;
        if (!ignore_flags && arg_i_str[0] == '-') {
          // If this argument actually looks like a flag...

          if (arg_i_str[1] == '-' && arg_i_str[2] == '\0') {
            // If the argument is "--" then that means treat the rest of the
            // arguments are positional arguments.
            ignore_flags = true;

          } else {
            // If we get a flag and it isn't one that was specified nor "--"
            // then this is an error.
            throw unexpected_flag_error("found unused flag");
          }

        } else if (num_flag_args_to_consume > 0) {
          // If we get an argument and the last flag is expecting some specific
          // positive number of arguments then give this positional argument to
          // that flag.
          last_flag_expecting_args->args.push_back(*arg_i);
          --num_flag_args_to_consume;

        } else if (num_flag_args_to_consume == flag_spec::zero_plus) {
          // If we get an argument and the last flag wants as many (or zero)
          // arguments as it can get then give this positional argument to that
          // flag and stay in this mode.
          last_flag_expecting_args->args.push_back(*arg_i);

        } else if (num_flag_args_to_consume == flag_spec::one_plus) {
          // If we get an argument and the last flag wants as many (but at
          // least one) arguments as it can get then give this positional
          // argument to that flag but go into the mode where we was as many or
          // zero arguments because that "at least one" we wanted has been
          // satisfied.
          last_flag_expecting_args->args.push_back(*arg_i);
          num_flag_args_to_consume = flag_spec::zero_plus;

        } else if (num_flag_args_to_consume == flag_spec::optional) {
          // If we get an argument and the last flag expects zero or one
          // arguments then give this positional argument to the flag and
          // change mode so that we aren't expecting anymore flag arguments.
          last_flag_expecting_args->args.push_back(*arg_i);
          num_flag_args_to_consume = 0;

        } else {
          // If there are no expectations for flag arguments then simply use
          // this argument as a positional argument.
          arg_res.args.push_back(*arg_i);

        }
      } else {
        // If we found a flag that was specified...

        if (num_flag_args_to_consume > 0) {
          // If we get a known flag but are expecting arguments for a previous
          // flag then throw an error.
          throw unexpected_flag_error(
            "expected arguments for previous flag but got another flag");

        } else if (num_flag_args_to_consume == flag_spec::zero_plus) {
          // If we get a known flag but are expecting as many (or zero)
          // arguments for the last flag then simply reset the number of
          // expected flag arguments.
          num_flag_args_to_consume = 0;

        } else if (num_flag_args_to_consume == flag_spec::one_plus) {
          // If we get a known flag but are still expecting at least one or
          // more arguments for the last flag then we haven't found "at least
          // one" argument for the last flag. This is an error.
          throw unexpected_flag_error(
            "expected arguments for previous flag but got another flag");

        } else if (num_flag_args_to_consume == flag_spec::optional) {
          // If we get a known flag but are expecting zero or one arguments for
          // the last flag then simply reset the number of expected flag
          // arguments.
          num_flag_args_to_consume = 0;
        }

        auto& spec = *matching_flag;

        // If this flag_spec has not been added before then do that now.
        if (!arg_res.has_flag(spec.name)) {
          flag new_flag {0, {}};
          arg_res.flags.emplace(
            std::make_pair(spec.name, std::move(new_flag)));
        }

        flag& flag_i = arg_res.flags[spec.name];

        // Increment the number of occurences of this flag
        ++flag_i.occurences;

        // If this flag expects arguments then put our parser into the
        // corresponding "expected flag arguments" mode.
        if (spec.num_args != 0) {
          last_flag_expecting_args = &flag_i;
          num_flag_args_to_consume = spec.num_args;
        }
      }

      // Go on to the next argument.
      ++arg_i;
    }

    // If we're done with all of the arguments but are still expecting
    // arguments for a previous flag then we haven't satisfied that flag. This
    // is an error.
    if (num_flag_args_to_consume > 0 ||
        num_flag_args_to_consume == flag_spec::one_plus) {
      throw std::out_of_range(
        "flag expected arguments but there are no more arguments");
    }

    return arg_res;
  }

  /**
   * @brief
   * Vector of the flag specifications which inform this parser how to parse
   * the command line arguments.
   */
  std::vector<flag_spec> specs;

};


namespace convert {

  template <>
  bool arg(const char* arg)
  {
    std::string arg_str(arg);
    // TODO (vnguyen): Make case insensitive.
    return (
      arg_str == "true" ||
      arg_str == "yes" ||
      arg_str == "on" ||
      arg_str == "1");
  }

  template <>
  int arg(const char* arg)
  {
    return std::atoi(arg);
  }

  template <>
  long arg(const char* arg)
  {
    return std::atol(arg);
  }

  template <>
  long long arg(const char* arg)
  {
    return std::atoll(arg);
  }

  template <>
  float arg(const char* arg)
  {
    return static_cast<float>(std::atof(arg));
  }

  template <>
  double arg(const char* arg)
  {
    return std::atof(arg);
  }

  template <>
  std::string arg(const char* arg)
  {
    return std::string(arg);
  }

}


} // namespace argagg


/**
 * @brief
 * Writes the flag/option help to the given stream.
 */
std::ostream& operator << (std::ostream& os, const argagg::parser& x)  
{  
  for (auto& spec : x.specs) {
    os << "    ";
    for (auto& flag : spec.flags) {
      os << flag;
      if (flag != spec.flags.back()) {
        os << ", ";
      }
    }
    os << std::endl;
    os << "        " << spec.help << std::endl;
  }
  return os;
}


#endif // ARGAGG_ARGAGG_ARGAGG_HPP
