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
 * This exception is thrown when a flag is parsed unexpectedly such as when an
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
 * the flag::as(), flags::as(), flags::as(), args::as(), and args::all_as()
 * methods are placed in this namespace.
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
 * Contains the parser result for a single flag instance.
 */
struct flag {

  /**
   * @brief
   * Argument parsed for this flag.
   */
  const char* arg;

  /**
   * @brief
   * Converts the argument parsed for this flag instance into the given type
   * using the type matched conversion function ::argagg::convert::arg(). If
   * there was not an argument parsed for this flag instance then a
   * std::invalid_argument exception is thrown.
   */
  template <typename T>
  T as() const
  {
    if (this->arg) {
      return convert::arg<T>(this->arg);
    } else {
      throw std::invalid_argument("flag has no argument");
    }
  }

  /**
   * @brief
   * Converts the argument parsed for this flag instance into the given type
   * using the type matched conversion function ::argagg::convert::arg(). If
   * there was not an argument parsed for this flag instance then the provided
   * default value is returned instead.
   */
  template <typename T>
  T as(const T& t) const
  {
    if (this->arg) {
      return convert::arg<T>(this->arg);
    } else {
      return t;
    }
  }

  /**
   * @brief
   * Since we have the flag::as() API we might as well alias it as an implicit
   * conversion operator. This performs implicit conversion using the as()
   * method.
   */
  template <typename T>
  operator T () const
  {
    return this->as<T>();
  }

  /**
   * @brief
   * Implicit boolean conversion function which returns true if there is an
   * argument for this single flag.
   */
  operator bool () const
  {
    return this->arg != nullptr;
  }

};


/**
 * @brief
 * Represents multiple parse results for a single flag_spec. If treated as
 * a single parse result it defaults to the last parse result. Note that an
 * instance of this struct is not created if NO flags a parsed for a given
 * flag_spec.
 */
struct flags {

  /**
   * @brief
   * All flag parse results for this flag spec.
   */
  std::vector<::argagg::flag> all;

  /**
   * @brief
   * Gets the number of times the flag_spec shows up.
   */
  std::size_t count() const
  {
    return this->all.size();
  }

  /**
   * @brief
   * Gets a single flag result by index.
   */
  ::argagg::flag& operator [] (std::size_t index)
  {
    return this->all[index];
  }

  /**
   * @brief
   * Gets a single flag result by index.
   */
  const ::argagg::flag& operator [] (std::size_t index) const
  {
    return this->all[index];
  }

  /**
   * @brief
   * Converts the argument parsed for this flag instance into the given type
   * using the type matched conversion function ::argagg::convert::arg(). If
   * there was not an argument parsed for this flag instance then a
   * std::invalid_argument exception is thrown.
   */
  template <typename T>
  T as() const
  {
    return this->all.back().as<T>();
  }

  /**
   * @brief
   * Converts the argument parsed for this flag instance into the given type
   * using the type matched conversion function ::argagg::convert::arg(). If
   * there was not an argument parsed for this flag instance then the provided
   * default value is returned instead.
   */
  template <typename T>
  T as(const T& t) const
  {
    if (this->all.size() == 0) {
      return t;
    }
    return this->all.back().as<T>(t);
  }

  /**
   * @brief
   * Since we have the flag::as() API we might as well alias it as an implicit
   * conversion operator. This performs implicit conversion using the as()
   * method.
   */
  template <typename T>
  operator T () const
  {
    return this->as<T>();
  }

  /**
   * @brief
   * Implicit boolean conversion function which returns true if there is at
   * least one parser result for this flag_spec.
   */
  operator bool () const
  {
    return this->all.size() > 0;
  }

};


/**
 * @brief
 * Represents all results of the parser including flags and positional
 * arguments.
 */
struct result {

  /**
   * @brief
   * Returns the name of the program from the original arguments list.
   */
  const char* program;

  /**
   * @brief
   * Maps from flag_spec name to the structure which contains the parser
   * results for that flag_spec.
   */
  std::unordered_map<std::string, ::argagg::flags> flags;

  /**
   * @brief
   * Vector of positional arguments.
   */
  std::vector<const char*> pos;

  /**
   * @brief
   * Used to check if a flag_spec was specified at all.
   */
  bool has_flag(const std::string& name) const
  {
    const auto it = this->flags.find(name);
    return ( it != this->flags.end()) && it->second.all.size() > 0;
  }

  /**
   * @brief
   * Get the parser results for the given flag_spec. If the flag_spec never
   * showed up then the exception from the unordered_map access will bubble
   * through so check if the flag exists in the first place with has_flag().
   */
  ::argagg::flags& operator [] (const std::string& name)
  {
    return this->flags.at(name);
  }

  /**
   * @brief
   * Get the parser results for the given flag_spec. If the flag_spec never
   * showed up then the exception from the unordered_map access will bubble
   * through so check if the flag exists in the first place with has_flag().
   */
  const ::argagg::flags& operator [] (const std::string& name) const
  {
    return this->flags.at(name);
  }

  /**
   * @brief
   * Get the parser results for the given flag_spec. If the flag_spec never
   * showed up then the exception from the unordered_map access will bubble
   * through so check if the flag exists in the first place with has_flag().
   */
  ::argagg::flags& operator [] (const char* name)
  {
    return this->flags.at(std::string(name));
  }

  /**
   * @brief
   * Get the parser results for the given flag_spec. If the flag_spec never
   * showed up then the exception from the unordered_map access will bubble
   * through so check if the flag exists in the first place with has_flag().
   */
  const ::argagg::flags& operator [] (const char* name) const
  {
    return this->flags.at(std::string(name));
  }

  /**
   * @brief
   * Gets the number of positional arguments.
   */
  std::size_t count() const
  {
    return this->pos.size();
  }

  /**
   * @brief
   * Gets a positional argument by index.
   */
  const char* operator [] (std::size_t index) const
  {
    return this->pos[index];
  }

  /**
   * @brief
   * Gets a positional argument converted to the given type.
   */
  template <typename T>
  T as(std::size_t i = 0) const
  {
    return convert::arg<T>(this->pos[i]);
  }

  /**
   * @brief
   * Gets all positional arguments converted to the given type.
   */
  template <typename T>
  std::vector<T> all_as() const
  {
    std::vector<T> v(this->pos.size());
    std::transform(
      this->pos.begin(), this->pos.end(), v.begin(),
      [](const char* arg) {
        return convert::arg<T>(arg);
      });
    return v;
  }

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
   * Number of arguments this flag requires. Must be 0, 1, or
   * flag_spec::optional. All other values have undefined behavior.
   */
  char num_args;

  /**
   * @brief
   * A sentinal value that num_args can be set to. When used it means that this
   * flag expects zero or one arguments.
   */
  constexpr static int optional = -1;

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
  ::argagg::result parse(int argc, const char** argv)
  {
    bool ignore_flags = false;
    flag* last_flag_expecting_args = nullptr;
    char num_flag_args_to_consume = 0;

    result arg_res { argv[0], {}, {} };

    // Add a result for each flag_spec.
    for (auto& spec : this->specs) {
      ::argagg::flags x {{}};
      arg_res.flags.insert(std::make_pair(spec.name, x));
    }

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
            // arguments as positional arguments.
            ignore_flags = true;

          } else {
            // If we get a flag and it isn't one that was specified nor "--"
            // then this is an error.
            // TODO (vnguyen): Add the offending flag to the exception text.
            throw unexpected_flag_error("found unused flag");
          }

        } else if (num_flag_args_to_consume > 0) {
          // If we get an argument and the last flag is expecting some specific
          // positive number of arguments then give this positional argument to
          // that flag.
          last_flag_expecting_args->arg = *arg_i;
          --num_flag_args_to_consume;

        } else if (num_flag_args_to_consume == flag_spec::optional) {
          // If we get an argument and the last flag expects zero or one
          // arguments then give this positional argument to the flag and
          // change mode so that we aren't expecting anymore flag arguments.
          last_flag_expecting_args->arg = *arg_i;
          num_flag_args_to_consume = 0;

        } else {
          // If there are no expectations for flag arguments then simply use
          // this argument as a positional argument.
          arg_res.pos.push_back(*arg_i);

        }
      } else {
        // If we found a flag that was specified...

        if (num_flag_args_to_consume > 0) {
          // If we get a known flag but are expecting an argument for a
          // previous flag then throw an error.
          throw unexpected_flag_error(
            "expected arguments for previous flag but got another flag");

        } else if (num_flag_args_to_consume == flag_spec::optional) {
          // If we get a known flag but are expecting zero or one arguments for
          // the last flag then simply reset the number of expected flag
          // arguments.
          num_flag_args_to_consume = 0;
        }

        // Get the flag results.
        auto& spec = *matching_flag;
        flags& flags_i = arg_res.flags[spec.name];

        // Add a single flag result to flag results.
        flag flag_i {nullptr};
        flags_i.all.push_back(std::move(flag_i));

        // If this flag expects arguments then put our parser into the
        // corresponding "expected flag arguments" mode.
        if (spec.num_args != 0) {
          last_flag_expecting_args = &(flags_i.all.back());
          num_flag_args_to_consume = spec.num_args;
        }
      }

      // Go on to the next argument.
      ++arg_i;
    }

    // If we're done with all of the arguments but are still expecting
    // arguments for a previous flag then we haven't satisfied that flag. This
    // is an error.
    if (num_flag_args_to_consume > 0) {
      // TODO (vnguyen): Print which flag is expect an argument.
      throw std::out_of_range(
        "flag expected an argument but there are no more arguments");
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
  const char* arg(const char* arg)
  {
    return arg;
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
