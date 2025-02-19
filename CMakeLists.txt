CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(wp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(COMPILER_FLAGS "-Wall -Wextra -pedantic -Werror")
set(COMPILER_FLAGS "-Wall -Wextra -pedantic")
set(ADDRESS_SANITIZER_FLAGS "-fsanitize=address,undefined,leak -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(THREAD_SANITIZER_FLAGS "-fsanitize=thread,undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(DEBUG_FLAGS "-g")
set(OPTIMIZATION_FLAGS "-O3")
set(IGNORE_ERROR_FLAGS "-Wno-c99-designator")

if (CMAKE_BUILD_TYPE STREQUAL "memory")
  set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS} ${ADDRESS_SANITIZER_FLAGS} ${DEBUG_FLAGS} ${IGNORE_ERROR_FLAGS}")
elseif (CMAKE_BUILD_TYPE STREQUAL "thread")
  set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS} ${THREAD_SANITIZER_FLAGS} ${DEBUG_FLAGS} ${IGNORE_ERROR_FLAGS}")
elseif (CMAKE_BUILD_TYPE STREQUAL "release")
  set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS} ${OPTIMIZATION_FLAGS} ${IGNORE_ERROR_FLAGS}")
endif()

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if (ENABLE_COVERAGE)
  message(STATUS "Building with coverage flags...")
  add_compile_options(-fprofile-instr-generate -fcoverage-mapping)

  # Detect and link the correct `clang_rt.profile` library for the system
  if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")  # macOS
    set(COVERAGE_LIB "-lclang_rt.profile_osx")
  elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")  # Linux
    set(COVERAGE_LIB "-lclang_rt.profile")
  else()
    message(WARNING "Unknown system, trying generic coverage library")
    set(COVERAGE_LIB "-lclang_rt.profile")
  endif()
  add_link_options(${COVERAGE_LIB})
endif()

# Headers
include_directories(${CMAKE_SOURCE_DIR})
# Implementation
file(GLOB SOURCES
  ${CMAKE_SOURCE_DIR}/*.cpp)

add_executable(wp ${SOURCES})

# enable_testing()
# add_subdirectory(tests)

# if (ENABLE_COVERAGE)
#   set(TEST_IGNORE_REGEX ".*/tests/.*")  # Ignore anything in the tests directory

#   # Explicit coverage data paths
#   set(COVERAGE_RAW_FILE "${CMAKE_BINARY_DIR}/wp.profraw")
#   set(COVERAGE_RAW_FILE_TEST "${CMAKE_BINARY_DIR}/wptest.profraw")
#   set(COVERAGE_DATA_FILE "${CMAKE_BINARY_DIR}/coverage.profdata")
#   set(COVERAGE_HTML_DIR "${CMAKE_BINARY_DIR}/coverage_html")
#   set(COVERAGE_HTML_DIR_TEST "${CMAKE_BINARY_DIR}/coverage_html_test")

#   # Custom target to run `wp` with coverage
#   add_custom_target(run-wp
#     COMMAND LLVM_PROFILE_FILE=${COVERAGE_RAW_FILE} ./wp
#     DEPENDS wp
#     COMMENT "Running wp with coverage..."
#     VERBATIM
#   )

#   # Custom target to run `wptest` with coverage
#   add_custom_target(run-wptest
#     COMMAND LLVM_PROFILE_FILE=${COVERAGE_RAW_FILE_TEST} ./tests/wptest
#     DEPENDS tests/wptest
#     COMMENT "Running wptest with coverage..."
#     VERBATIM
#   )

#   # Merge coverage data from wp and wptest
#   add_custom_target(coverage-merge
#     COMMAND llvm-profdata merge -sparse ${COVERAGE_RAW_FILE} ${COVERAGE_RAW_FILE_TEST} -o ${COVERAGE_DATA_FILE}
#     DEPENDS run-wp run-wptest
#     COMMENT "Merging coverage data..."
#     VERBATIM
#   )

#   # Generate text coverage report for wp
#   add_custom_target(coverage-wp-text
#     COMMAND llvm-cov report ./wp -instr-profile=${COVERAGE_DATA_FILE}
#     DEPENDS coverage-merge
#     COMMENT "Generating text coverage report for wp..."
#     VERBATIM
#   )

#   # Generate text coverage report for wptest
#   add_custom_target(coverage-wptest-text
#     COMMAND llvm-cov report ./tests/wptest -instr-profile=${COVERAGE_DATA_FILE}
#     DEPENDS coverage-merge
#     COMMENT "Generating text coverage report for wptest..."
#     VERBATIM
#   )

#   # Generate HTML coverage report for wp
#   add_custom_target(coverage-wp-html
#     COMMAND llvm-cov show ./wp -instr-profile=${COVERAGE_DATA_FILE} -format=html
#   -output-dir=${COVERAGE_HTML_DIR} -ignore-filename-regex=${TEST_IGNORE_REGEX}
#     DEPENDS coverage-merge
#     COMMENT "Generating HTML coverage report for wp..."
#     VERBATIM
#   )

#   # Generate HTML coverage report for wptest
#   add_custom_target(coverage-wptest-html
#     COMMAND llvm-cov show ./tests/wptest -instr-profile=${COVERAGE_DATA_FILE}
#   -format=html -output-dir=${COVERAGE_HTML_DIR_TEST} -ignore-filename-regex=${TEST_IGNORE_REGEX}
#     DEPENDS coverage-merge
#     COMMENT "Generating HTML coverage report for wptest..."
#     VERBATIM
#   )
# endif()
