cmake_minimum_required( VERSION 2.8 )
project( argagg CXX )

option(
  ARGAGG_BUILD_EXAMPLES
  "build examples"
  ON
)

option(
  ARGAGG_BUILD_TESTS
  "build tests"
  ON
)

option(
  ARGAGG_BUILD_DOCS
  "build docs"
  ON
)

set(
  ARGAGG_TEST_COMPILE_FLAGS
  "-g -Wall -Wextra -Wpedantic -Werror -std=c++11"
  CACHE STRING "Compiler flags for all project targets"
)

# When RPM packages are built CMake is invoked with a -DINCLUDE_INSTALL_DIR
# that we should respect. If it isn't present then we default it to "include".
set(
  INCLUDE_INSTALL_DIR "include"
  CACHE STRING "Include install folder name (default: include)"
)

# When RPM packages are built CMake is invoked with a -DLIB_INSTALL_DIR that we
# should respect. This is important because this is how the RPM build process
# specifies installation into "lib64" instead of "lib". If it isn't present
# then we default it to "lib".
set(
  LIB_INSTALL_DIR "lib"
  CACHE STRING "Library install folder name (default: lib)"
)

# When RPM packages are built CMake is invoked with a -DSHARE_INSTALL_PREFIX
# that we should respect. This is particularly important because this is how
# the RPM build process specifies installation of shared data files into
# "/usr/share". If it isn't present then we default it to "share" relative to
# the CMAKE_INSTALL_PREFIX which is incidentally the same path when the install
# prefix is "/usr".
set(
  SHARE_INSTALL_PREFIX share
  CACHE STRING "Shared data install folder name (default: share)"
)


# Set up a target to install the program which amounts to copying the single
# header file.
install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include"
  DESTINATION "${INCLUDE_INSTALL_DIR}/.."
)


# Build examples if configured to.
if( ARGAGG_BUILD_EXAMPLES )
  add_executable( joinargs "examples/joinargs.cpp" )
  set_target_properties(
    joinargs
    PROPERTIES
      COMPILE_FLAGS "${ARGAGG_TEST_COMPILE_FLAGS}"
      INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )

  add_executable( gengetopt_main1 "examples/gengetopt_main1.cpp" )
  set_target_properties(
    gengetopt_main1
    PROPERTIES
      COMPILE_FLAGS "${ARGAGG_TEST_COMPILE_FLAGS}"
      INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
      OUTPUT_NAME sample1
  )

  add_executable( gcc_like "examples/gcc_like.cpp" )
  set_target_properties(
    gcc_like
    PROPERTIES
      COMPILE_FLAGS "${ARGAGG_TEST_COMPILE_FLAGS}"
      INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
endif()


# Build and register the unit tests if configured to.
if( ARGAGG_BUILD_TESTS )
  enable_testing()

  list( APPEND ARGAGG_TEST_SOURCES "test/test.cpp" )
  list( APPEND ARGAGG_TEST_SOURCES "test/test_csv.cpp" )

  find_path( OPENCV_INCLUDE_DIR "opencv2/opencv.hpp" )
  find_library( OPENCV_CORE_LIBRARY opencv_core )
  if( OPENCV_INCLUDE_DIR AND OPENCV_CORE_LIBRARY )
    list( APPEND ARGAGG_TEST_SOURCES "test/test_opencv.cpp" )
    list( APPEND ARGAGG_TEST_INCLUDE_DIRS ${OPENCV_INCLUDE_DIR} )
    list( APPEND ARGAGG_TEST_LIB_DEPS ${OPENCV_CORE_LIBRARY} )
  else()
    message( "Unable to find OpenCV, disabling test_opencv.cpp" )
  endif()

  add_executable( argagg_test ${ARGAGG_TEST_SOURCES} )

  list( LENGTH ARGAGG_TEST_INCLUDE_DIRS ARGAGG_TEST_INCLUDE_DIRS_LENGTH )
  if( ARGAGG_TEST_INCLUDE_DIRS_LENGTH GREATER 0 )
    target_include_directories(
      argagg_test
      PRIVATE
      ${ARGAGG_TEST_INCLUDE_DIRS}
    )
  endif()

  list( LENGTH ARGAGG_TEST_LIB_DEPS ARGAGG_TEST_LIB_DEPS_LENGTH )
  if( ARGAGG_TEST_LIB_DEPS_LENGTH GREATER 0 )
    target_link_libraries(
      argagg_test
      PRIVATE
      ${ARGAGG_TEST_LIB_DEPS}
    )
  endif()

  set_target_properties(
    argagg_test
    PROPERTIES
      COMPILE_FLAGS "${ARGAGG_TEST_COMPILE_FLAGS}"
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
  add_test(
    NAME argagg_test
    COMMAND argagg_test
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
endif()


# Build Doxygen documentation if we can find Doxygen and we're configured to
# build documentation.
find_program( DOXYGEN doxygen )
if( ARGAGG_BUILD_DOCS AND DOXYGEN )

  # Everyone loves documentation! Lets set up a target to generate documentation.
  # First lets collect everything that we think can change the documentation.
  # This basically means everything inside the "docs" folder along with every
  # header and source file. If one of those changes then we want to mark the
  # documentation for regeneration.
  file(
    GLOB_RECURSE DOC_INPUTS
    "${CMAKE_CURRENT_SOURCE_DIR}/doc/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*"
    "${CMAKE_CURRENT_SOURCE_DIR}/test/*"
  )

  # Like the version.hpp header file we're going to replace some CMake variables
  # in the doxygen configuration with actual values.
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/doxygen.cfg.in
    ${CMAKE_BINARY_DIR}/doxygen.cfg
    @ONLY
  )

  # Now we add a command for CMake to run to generate documentation.
  add_custom_command(
    # We tell CMake that this command will generate the `html` and `xml`
    # folders inside the project's documentation share folder inside the build
    # folder. Specifying these outputs explicitly adds them to the "clean"
    # target. Note that we put the documentation into the "share" folder in the
    # build folder. This is to structure the build folder as a prefix.
    OUTPUT
      "${CMAKE_BINARY_DIR}/share/doc/${PROJECT_NAME}/html"
      "${CMAKE_BINARY_DIR}/share/doc/${PROJECT_NAME}/xml"
    # This command makes sure that the output folder exists first.
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/share/doc/${PROJECT_NAME}"
    # This command performs the doxygen documentation generation.
    COMMAND ${DOXYGEN} doxygen.cfg
    # Run the above commands in the build folder.
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    # Tell CMake that if any of the documentation input files we collected change
    # then this command needs to be re-run.
    DEPENDS
      ${CMAKE_BINARY_DIR}/doxygen.cfg
      ${DOC_INPUTS}
  )

  # This creates an "empty target" named "docs" that doesn't produce output, but
  # depends on the generated documentation. The result is we can run "make docs"
  # and it behaves the way we expect. It also doesn't regenerate documentation if
  # the outputs specified below are up-to-date.
  add_custom_target(
    docs ALL
    DEPENDS
      "${CMAKE_BINARY_DIR}/share/doc/${PROJECT_NAME}/html"
      "${CMAKE_BINARY_DIR}/share/doc/${PROJECT_NAME}/xml"
  )

  # Finally set up an installation target for the documentation.
  install(
    DIRECTORY "${CMAKE_BINARY_DIR}/share/"
    DESTINATION "${SHARE_INSTALL_PREFIX}"
  )

endif()
