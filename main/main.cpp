/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) Slava Barinov, <rayslava@gmail.com> 2019-2020
 */

/** \mainpage tgqif
 * \section intro_sec Introduction
 *
 * \b TgQIF is a Telegram bot app for QIF export
 *
 * \section install_sec Installation
 *
 * \section compile_sec Compilation
 * \subsection prereq_sec Prerequisites
 * - Boost >= 1.72
 * - tgbot-cpp >= 1.2.1
 * - C++ compiler with C++20 support (GCC 9.0+, Clang 8+)
 * \subsection comp_build_sec Building
 * For build just use \c make \c release \c JOBS=3
 * \subsection docker_sec Building with Docker
 * You can also build a static release binary in guaranteed working environment.
 * To use it you should install \c docker into your system
 * (for Ubuntu: \c sudo \c apt-get \c install \c docker and add your user to
 * \c docker group) and then run \c make \c dockerized-build
 * \section cmdline_sec Command line flags
 * \li \c \-\-help print help message
 *
 * \section license License information
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * \section dev Development
 * \subsection cpp Language standard
 * Now \c C++20 is used to build the project and you are free to use all
 * features provided by this language.
 * \subsection exceptions Exceptions
 * The \b RAII is used all over the project. So exceptions in the constructors
 * are considered normal and you must expect that any class can throw you an
 * exception accordingly to its documentation.
 *
 * Every exception must be documented in \c \\throws section of Doxygen doc
 * of method.
 * \subsection memory Memory management
 * Automated memory management is strongly recommended and is widely used
 * accordingly to Core Guidelines.
 * Please Do use refcounted pointers instead of manual \c new -ing and \c delete
 * -ing objects.
 * \subsection exttools External tools usage
 * If functional you implement needs external tools usage you should add it to
 * EXTERNAL_TOOLS list in CMakeLists.txt and use through tools::Toolset["tool"]
 * from tools.hpp file.
 * \subsection codestyle Code style
 * \subsubsection naming Naming conventions
 * \li \c Class names start from Upper case and use CamelCase
 * \li \c method names start from lower case and use underscore_naming
 * \li \c member names start from _underscore and lower case and use
 * _underscore_notation
 * Example:
 * \code{cpp}
 * class Class {
 *   int _number;
 * public:
 *   void number_set();
 *   int  number() { return _number; };
 * }
 * \endcode
 * Trivial one-line getters and setters are allowed.
 * \subsubsection coding Spacing and general code style
 * The code style is maintained by \c uncrustify tool which can be applied by
 * \c make \c stylefix from tgqif directory.
 * \subsection output Adding output
 * If you want to add your own output see \ref factory section.
 *
 * After creating a class you should add it to \c CMakeLists.txt and set
 * documentation link in \c main.cpp
 * \subsection testing Testing
 * Automated testing is performed using tests from \c test directory.
 * When you add new code please cover is with tests by adding new test file and
 * registering the test in \c CMakeLists.txt
 *
 * Existing coverage can be seen at <a href="coverage/index.html">LCOV page</a>
 */

#include <boost/program_options/variables_map.hpp>
#include <functional>
#include <iostream>
#include <filesystem>
#include <string>
#include <optional>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

#include "tg/tg.hpp"

#define version_info "tgqif " VERSION " built at " BUILDDATE
#define usage_info "This program is free software and is destributed under terms of GPLv2"

constexpr std::string_view version_string = version_info;
constexpr std::string_view info_string = usage_info;

constexpr auto default_config_name {"tgqif.conf"};

namespace po = boost::program_options;
namespace fs = std::filesystem;
namespace logging = boost::log;

int main(int argc, char* argv[]) {

  if (argc == 1) {
    std::cout << version_string << std::endl
              << "Try '" << argv[0] << " --help' for usage information" << std::endl;
    return 0;
  };

  po::variables_map vm;
  bool verbose;
  bool debug;
  try {
    po::options_description glob("Global options");
    glob.add_options()
      ("help,h", "Print this help message")
      ("version,v", "Print version information")
      ("verbose,V", po::bool_switch(&verbose), "Enable verbose messages")
      ("debug,D", po::bool_switch(&debug), "Enable debug messages")
      ("config,c", po::value<fs::path>()->default_value(default_config_name), "Config file path")
    ;

    po::options_description tgopt("Telegram options");
    tgopt.add_options()
      ("name,n", po::value<std::string>(), "Telegram bot name")
      ("password,p", po::value<std::string>(), "Telegram API hash for bot")
    ;

    po::options_description cmdline_options;
    cmdline_options.add(glob).add(tgopt);

    po::options_description config_file_options;
    config_file_options.add(tgopt);

    // Parse global options
    po::store(po::command_line_parser(argc, argv).
              options(cmdline_options).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << version_info << std::endl;
      std::cout << "Usage: " <<
        fs::path(argv[0]).stem() <<
        " [options]" << std::endl;
      std::cout << std::endl << glob << std::endl << tgopt << std::endl;
      std::cout << usage_info << std::endl;
      return 0;
    }

    if (vm.count("version")) {
      std::cout << version_string << std::endl;
      return 0;
    }

    // Read environment
    po::store(po::parse_environment(config_file_options, "TGQIF_"), vm);
    po::notify(vm);

    // Read config defaults from file
    if (vm.count("config")) {
      BOOST_LOG_TRIVIAL(debug) << "Parsing config file";
      if (fs::exists(vm["config"].as<fs::path>())) {
        po::store(po::parse_config_file(fs::canonical(vm["config"].as<fs::path>()).c_str(), tgopt), vm);
        po::notify(vm);
      } else {
        BOOST_LOG_TRIVIAL(error) << "Config file does not exists";
      }
    }
  }

  catch(std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  catch(...) {
    std::cerr << "Something really bad happened" << std::endl;
  }

  /** TODO: Debug why this won't compile
     logging::core::get()->set_filter (
     logging::trivial::severity >= logging::trivial::warning
     );

     if (verbose)
     logging::core::get()->set_filter (
      logging::trivial::severity >= logging::trivial::info
      );

     if (debug)
     logging::core::get()->set_filter (
      logging::trivial::severity >= logging::trivial::debug
      );
   */

  tgbot_main(vm["password"].as<std::string>());
}
