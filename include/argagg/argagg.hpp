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
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>


/**
 * @brief
 * There are only two hard things in Computer Science: cache invalidation and
 * naming things (Phil Karlton).
 *
 * The names of types have to be succint and clear. This has turned out to be a
 * more difficult thing than I expected. Here you'll find a quick overview of
 * the type names you'll find in this namespace (and thus "library").
 *
 * When a program is invoked it is passed a number of "command line arguments".
 * Each of these "arguments" is a string (C-string to be more precise). An
 * "option" is a command line argument that has special meaning. This library
 * recognizes a command line argument as a potential option if it starts with a
 * dash ('-') or double-dash ('--').
 *
 * A "parser" is a set of "definitions" (not a literal std::set but rather a
 * std::vector). A parser is represented by the argagg::parser struct.
 *
 * A "definition" is a structure with four components that define what
 * "options" are recognized. The four components are the name of the option,
 * the strings that represent the option, the option's help text, and how many
 * arguments the option should expect. "Flags" are the individual strings that
 * represent the option ("-v" and "--verbose" are flags for the "verbose"
 * option). A definition is represented by the argagg::definition struct.
 *
 * Note at this point that the word "option" can be used interchangeably to
 * mean the notion of an option and the actual instance of an option given a
 * set of command line arguments. To be unambiguous we use a "definition" to
 * represent the notion of an option and an "option result" to represent an
 * actual option parsed from a set of command line arguments. An "option
 * result" is represented by the argagg::option_result struct.
 *
 * There's one more wrinkle to this: an option can show up multiple times in a
 * given set of command line arguments. For example, "-n 1 -n 2 -n 3". This
 * will parse into three distinct argagg::option_result instances, but all of
 * them correspond to the same argagg::definition. We aggregate these into the
 * argagg::option_results struct which represents "all parser results for a
 * given option definition". This argagg::option_results is basically a
 * std::vector of argagg::option_result.
 *
 * Options aren't the only thing parsed though. Positional arguments are also
 * parsed. Thus a parser produces a result that contains both option results
 * and positional arguments. The parser results are represented by the
 * argagg::parser_results struct. All option results are stored in a mapping
 * from option name to the argagg::option_results. All positional arguments are
 * simply stored in a vector of C-strings.
 */
namespace argagg {


/**
 * @brief
 * This exception is thrown when an option is parsed unexpectedly such as when
 * an argument was expected for a previous option or if an option was found
 * that has not been defined.
 */
struct unexpected_option_error
: public std::invalid_argument {

  /**
   * @brief
   * Explicit constructor which passes "what" string argument to
   * std::invalid_argument constructor.
   */
  explicit unexpected_option_error(const std::string& what)
  : std::invalid_argument(what)
  {
  }

};


/**
 * @brief
 * This exception is thrown when an option requires an argument but is not
 * provided one. This can happen if another flag was found after the option or
 * if we simply reach the end of the command line arguments.
 */
struct option_lacks_argument_error
: public std::invalid_argument {

  /**
   * @brief
   * Explicit constructor which passes "what" string argument to
   * std::invalid_argument constructor.
   */
  explicit option_lacks_argument_error(const std::string& what)
  : std::invalid_argument(what)
  {
  }

};


/**
 * @brief
 * The set of template instantiations that convert C-strings to other types for
 * the option_result::as(), option_results::as(), parser_results::as(), and
 * parser_results::all_as() methods are placed in this namespace.
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
 * Represents a single option parse result.
 *
 * You can check if this has an argument by using the implicit boolean
 * conversion.
 */
struct option_result {

  /**
   * @brief
   * Argument parsed for this single option. If no argument was parsed this
   * will be set to nullptr.
   */
  const char* arg;

  /**
   * @brief
   * Converts the argument parsed for this single option instance into the
   * given type using the type matched conversion function
   * ::argagg::convert::arg(). If there was not an argument parsed for this
   * single option instance then a argagg::option_lacks_argument_error
   * exception is thrown.
   */
  template <typename T>
  T as() const
  {
    if (this->arg) {
      return convert::arg<T>(this->arg);
    } else {
      throw option_lacks_argument_error("option has no argument");
    }
  }

  /**
   * @brief
   * Converts the argument parsed for this single option instance into the
   * given type using the type matched conversion function
   * ::argagg::convert::arg(). If there was not an argument parsed for this
   * single option instance then the provided default value is returned
   * instead.
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
   * Since we have the argagg::option_result::as() API we might as well alias
   * it as an implicit conversion operator. This performs implicit conversion
   * using the argagg::option_result::as() method.
   */
  template <typename T>
  operator T () const
  {
    return this->as<T>();
  }

  /**
   * @brief
   * Implicit boolean conversion function which returns true if there is an
   * argument for this single option instance.
   */
  operator bool () const
  {
    return this->arg != nullptr;
  }

};


/**
 * @brief
 * Represents multiple option parse results for a single option. If treated as
 * a single parse result it defaults to the last parse result. Note that an
 * instance of this struct is always created even if no option results are
 * parsed for a given definition. In that case it will simply be empty.
 *
 * To check if the associated option showed up at all simply use the implicit
 * boolean conversion or check if count() is greater than zero.
 */
struct option_results {

  /**
   * @brief
   * All option parse results for this option.
   */
  std::vector<::argagg::option_result> all;

  /**
   * @brief
   * Gets the number of times the option shows up.
   */
  std::size_t count() const
  {
    return this->all.size();
  }

  /**
   * @brief
   * Gets a single option parse result by index.
   */
  ::argagg::option_result& operator [] (std::size_t index)
  {
    return this->all[index];
  }

  /**
   * @brief
   * Gets a single option result by index.
   */
  const ::argagg::option_result& operator [] (std::size_t index) const
  {
    return this->all[index];
  }

  /**
   * @brief
   * Converts the argument parsed for the LAST option parse result for the
   * parent definition to the provided type. For example, if this was for "-f 1
   * -f 2 -f 3" then calling this method for an integer type will return 3. If
   * there are no option parse results then a std::out_of_range exception is
   * thrown. Any exceptions thrown by argagg::option_result::as() are not
   * handled.
   */
  template <typename T>
  T as() const
  {
    if (this->all.size() == 0) {
      throw std::out_of_range("no option arguments to convert");
    }
    return this->all.back().as<T>();
  }

  /**
   * @brief
   * Converts the argument parsed for the LAST option parse result for the
   * parent definition to the provided type. For example, if this was for "-f 1
   * -f 2 -f 3" then calling this method for an integer type will return 3. If
   * there are no option parse results then the provided default value is
   * returned instead.
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
   * Since we have the argagg::option_results::as() API we might as well alias
   * it as an implicit conversion operator. This performs implicit conversion
   * using the argagg::option_results::as() method.
   */
  template <typename T>
  operator T () const
  {
    return this->as<T>();
  }

  /**
   * @brief
   * Implicit boolean conversion function which returns true if there is at
   * least one parser result for this definition.
   */
  operator bool () const
  {
    return this->all.size() > 0;
  }

};


/**
 * @brief
 * Represents all results of the parser including options and positional
 * arguments.
 */
struct parser_results {

  /**
   * @brief
   * Returns the name of the program from the original arguments list. This is
   * always the first argument.
   */
  const char* program;

  /**
   * @brief
   * Maps from definition name to the structure which contains the parser
   * results for that definition.
   */
  std::unordered_map<std::string, ::argagg::option_results> options;

  /**
   * @brief
   * Vector of positional arguments.
   */
  std::vector<const char*> pos;

  /**
   * @brief
   * Used to check if an option was specified at all.
   */
  bool has_option(const std::string& name) const
  {
    const auto it = this->options.find(name);
    return ( it != this->options.end()) && it->second.all.size() > 0;
  }

  /**
   * @brief
   * Get the parser results for the given definition. If the definition never
   * showed up then the exception from the unordered_map access will bubble
   * through so check if the flag exists in the first place with has_option().
   */
  ::argagg::option_results& operator [] (const std::string& name)
  {
    return this->options.at(name);
  }

  /**
   * @brief
   * Get the parser results for the given definition. If the definition never
   * showed up then the exception from the unordered_map access will bubble
   * through so check if the flag exists in the first place with has_option().
   */
  const ::argagg::option_results& operator [] (const std::string& name) const
  {
    return this->options.at(name);
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
 * An option definition which essentially represents what an option is.
 */
struct definition {

  /**
   * @brief
   * Name of the option. Option parser results are keyed by this name.
   */
  const std::string name;

  /**
   * @brief
   * List of strings to match that correspond to this option. Should be fully
   * specified with dashes (e.g. "-v" or "--verbose") but this is not enforced.
   */
  std::vector<const char*> flags;

  /**
   * @brief
   * Help string for this option.
   */
  const char* help;

  /**
   * @brief
   * Number of arguments this option requires. Must be 0, 1, or
   * ::argagg::optional. All other values have undefined behavior. Okay, the
   * code actually works with positive values in general, but it's unorthodox
   * command line behavior.
   */
  char num_args;

};


/**
 * @brief
 * A sentinal value that argagg::definition::num_args can be set to. When used
 * it means that this option expects zero or one arguments.
 */
constexpr static int optional = -1;


/**
 * @brief
 * A list of option definitions used to inform how to parse arguments.
 */
struct parser {

  /**
   * @brief
   * Vector of the option definitions which inform this parser how to parse
   * the command line arguments.
   */
  std::vector<::argagg::definition> definitions;

  /**
   * @brief
   * Parses the provided command line arguments and returns the results as
   * argagg::parser::results.
   */
  ::argagg::parser_results parse(int argc, const char** argv)
  {
    bool ignore_flags = false;
    const char* last_flag_expecting_args = nullptr;
    option_result* last_option_expecting_args = nullptr;
    char num_option_args_to_consume = 0;

    ::argagg::parser_results results { argv[0], {}, {} };

    // Add a result for each definition.
    for (auto& defn : this->definitions) {
      ::argagg::option_results opt_results {{}};
      results.options.insert(std::make_pair(defn.name, opt_results));
    }

    // Get pointers to pointers so we can treat the raw pointer array as an
    // iterator for standard library algorithms.
    const char** arg_i = argv + 1;
    const char** arg_end = argv + argc;

    while (arg_i != arg_end) {
      auto arg_i_str = *arg_i;

      // If we're not ignoring flags then check to see if this argument looks
      // like a definition that has been specified...
      auto matching_flag = this->definitions.end();
      if (!ignore_flags) {
        matching_flag = std::find_if(
          this->definitions.begin(), this->definitions.end(),
          [=](const ::argagg::definition& defn) {
            // ...and it looks like a definition that has been specified if the
            // argument matches "any of" the flag strings for a given
            // definition.
            return std::any_of(
              defn.flags.begin(), defn.flags.end(),
              [=](const char* flag) {
                return std::strcmp(flag, arg_i_str) == 0;
              });
          });
      }

      if (matching_flag == this->definitions.end()) {
        // If we are ignoring flags or the argument didn't match a definition
        // that was specified...

        if (!ignore_flags && arg_i_str[0] == '-') {
          // If this argument actually looks like a flag...

          if (arg_i_str[1] == '-' && arg_i_str[2] == '\0') {
            // If the argument is "--" then that means treat the rest of the
            // arguments as positional arguments.
            ignore_flags = true;

          } else {
            // If we get a option and it isn't one that was specified nor "--"
            // then this is an error.
            std::ostringstream msg;
            msg << "found unexpected flag: " << arg_i_str;
            throw unexpected_option_error(msg.str());
          }

        } else if (num_option_args_to_consume > 0) {
          // If we get an argument and the last option is expecting some
          // specific positive number of arguments then give this positional
          // argument to that option.
          last_option_expecting_args->arg = arg_i_str;
          --num_option_args_to_consume;

        } else if (num_option_args_to_consume == ::argagg::optional) {
          // If we get an argument and the last option expects zero or one
          // arguments then give this positional argument to the option and
          // change mode so that we aren't expecting anymore option arguments.
          last_option_expecting_args->arg = arg_i_str;
          num_option_args_to_consume = 0;

        } else {
          // If there are no expectations for option arguments then simply use
          // this argument as a positional argument.
          results.pos.push_back(arg_i_str);

        }
      } else {
        // If we found a option that was specified...

        if (num_option_args_to_consume > 0) {
          // If we get a known option but are expecting an argument for a
          // previous option then throw an error.
          std::ostringstream msg;
          msg << "option \"" << last_flag_expecting_args
              << "\" expects an argument but got another option (\""
              << arg_i_str << "\") instead";
          throw option_lacks_argument_error(msg.str());

        } else if (num_option_args_to_consume == ::argagg::optional) {
          // If we get a known option but are expecting zero or one arguments
          // for the last option then simply reset the number of expected
          // option arguments.
          num_option_args_to_consume = 0;
        }

        // Get the option results.
        auto& defn = *matching_flag;
        auto& opt_results = results.options[defn.name];

        // Add a single option result to option results.
        ::argagg::option_result opt_result {nullptr};
        opt_results.all.push_back(std::move(opt_result));

        // If this option expects arguments then put our parser into the
        // corresponding "expecting option arguments" mode.
        if (defn.num_args != 0) {
          last_flag_expecting_args = arg_i_str;
          last_option_expecting_args = &(opt_results.all.back());
          num_option_args_to_consume = defn.num_args;
        } else {
          last_flag_expecting_args = nullptr;
          last_option_expecting_args = nullptr;
          num_option_args_to_consume = 0;
        }
      }

      // Go on to the next argument.
      ++arg_i;
    }

    // If we're done with all of the arguments but are still expecting
    // arguments for a previous option then we haven't satisfied that option.
    // This is an error.
    if (num_option_args_to_consume > 0) {
      std::ostringstream msg;
      msg << "last option \"" << last_flag_expecting_args
          << "\" expects an argument but the parser ran out of command line "
          << "arguments to parse";
      throw option_lacks_argument_error(msg.str());
    }

    return results;
  }

};


namespace convert {

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
 * Writes the option help to the given stream.
 */
std::ostream& operator << (std::ostream& os, const ::argagg::parser& x)
{
  for (auto& definition : x.definitions) {
    os << "    ";
    for (auto& flag : definition.flags) {
      os << flag;
      if (flag != definition.flags.back()) {
        os << ", ";
      }
    }
    os << std::endl;
    os << "        " << definition.help << std::endl;
  }
  return os;
}


#endif // ARGAGG_ARGAGG_ARGAGG_HPP
