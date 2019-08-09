cmake_minimum_required(VERSION 3.2)
project(tgqif)
cmake_policy(SET CMP0049 OLD)

enable_language(C CXX)
include (CheckCSourceCompiles)
include (CheckCXXSourceCompiles)
include (CMakeDependentOption)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  )

find_package (Threads REQUIRED)

if (STATIC)
  message(STATUS "Static build enabled")
  set(Boost_USE_MULTITHREADED      OFF)
  set(Boost_USE_STATIC_LIBS        ON)
  set(Boost_USE_STATIC_RUNTIME     ON)
  set(Boost_USE_MULTITHREADED      OFF)
else()
  set(Boost_USE_MULTITHREADED      ON)
  message(STATUS "Dynamic build enabled")
  add_definitions(-DBOOST_LOG_DYN_LINK)
endif()

# Boost Requirements
find_package(Boost 1.70 REQUIRED COMPONENTS system log program_options)

# tdlib
find_package(Td 1.4.0 REQUIRED)

set(COMMON_SOURCE_FILES
  src/config.cpp
  )

set(SOURCE_FILES
  ${COMMON_SOURCE_FILES}
  )

macro(create_test name files)
  message(STATUS "Creating test '${name}' of ${files}")

  if (NOT (${name} STREQUAL "unit"))
    # Collecting names for globas "unit_tests" binary
    list(APPEND all_test_files
      "${files}")
  endif()

  set(${name}_TEST_FILES
    tests/main.cpp
    ${files}
    )

  set(test_files
    "\${${name}_TEST_FILES}"
    )

  add_executable(
    ${name}_tests
    ${COMMON_SOURCE_FILES}
    ${GTEST_SOURCES}
    ${GMOCK_SOURCES}
    ${test_files}
    )

  target_link_libraries(
    ${name}_tests
    ${LIBCXX_LIBRARY}
    ${GTEST_BOTH_LIBRARIES}
    ${GMOCK_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${TOX_LIBRARY}
    ${SODIUM_LIBRARY}
    ${STATIC_LIBRT}
    )
  add_test(${name} ${name}_tests)
  set_target_properties(
            ${name}_tests
            PROPERTIES
            COMPILE_DEFINITIONS _UNIT_TEST_BUILD)
endmacro(create_test)

include(ConfigSafeGuards)

find_package(GTest)
if (DEFINED GTEST_INCLUDE_DIRS)
  include_directories(${GTEST_INCLUDE_DIRS})
endif()

find_package(GMock)
if (DEFINED GMOCK_INCLUDE_DIRS)
  include_directories(${GMOCK_INCLUDE_DIRS})
endif()

if (TRAVIS_BUILD)
  add_definitions(-DTRAVIS_BUILD)
endif()

# Enable thread-safe POSIX implementations of C library
add_definitions(-D_POSIX_C_SOURCE)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(STANDARD "-std=c++2a")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(STANDARD "-std=c++2a")
endif()

execute_process(COMMAND git log --pretty=format:\"%h\" -1 OUTPUT_VARIABLE VERSION)
string(TIMESTAMP BUILDDATE "\"%Y-%m-%d %H:%M:%S UTC\"" UTC)

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -DBUILDDATE='${BUILDDATE}' -DVERSION='${VERSION}' -Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wwrite-strings -Wno-unused ${STANDARD} -Wextra -pedantic -Werror -pthread")
set(CMAKE_CXX_FLAGS_PROFILED "${CMAKE_CXX_FLAGS} -pg")

if (STATIC)
  add_definitions(-DSTATIC)
  set(BUILD_SHARED_LIBS OFF)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -fPIC -fPIE")
  set(LIBTD_LIBRARIES "Td::TdJsonStatic" "Td::TdStatic")
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
  endif()
else()
  set(LIBTD_LIBRARIES "Td::TdJson" "Td::TdStatic")
endif()

if (SANITIZED)
  set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3 ${STANDARD}")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast ${STANDARD}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto -fvisibility=hidden")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unknown-warning-option -Wno-error=braced-scalar-init -stdlib=libc++")
  set(LIBCXX_LIBRARY "-lc++abi")
endif()

if ((COVERALLS OR COVERAGE) AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "will generate coverage data")
  include(CodeCoverage)
  include(Coveralls)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O0 -Wall -W -fprofile-arcs -ftest-coverage --coverage")
  set(CMAKE_SHARED_LINKER_FLAGS="${CMAKE_SHARED_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")
endif()

add_executable(tgqif src/main.cpp ${SOURCE_FILES})

if (STATIC)
  SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
  SET (STATIC_LIBRT "-lrt")
  set_target_properties(tgqif PROPERTIES LINK_SEARCH_START_STATIC 1)
  set_target_properties(tgqif PROPERTIES LINK_SEARCH_END_STATIC 1)
endif()

enable_testing()

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
  )

target_link_libraries(
  tgqif
  ${CMAKE_THREAD_LIBS_INIT}
  ${LIBTD_LIBRARIES}
  ${LIBCXX_LIBRARY}
  ${STATIC_LIBRT}
  ${Boost_LIBRARIES}
  )

include(CTest)
enable_testing()

create_test(config tests/config.cpp)

create_test(unit "${all_test_files}")

if (COVERAGE)
  setup_target_for_coverage(coverage unit_tests CMakeFiles/unit_tests.dir/src coverage)
endif()

if (COVERALLS)
  setup_target_for_coveralls(coveralls unit_tests src)
endif()
