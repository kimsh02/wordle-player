CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(wordle)

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

add_executable(wordle ${SOURCES})

# enable_testing()
# add_subdirectory(tests)

# if (ENABLE_COVERAGE)
#   set(TEST_IGNORE_REGEX ".*/tests/.*")  # Ignore anything in the tests directory

#   # Explicit coverage data paths
#   set(COVERAGE_RAW_FILE "${CMAKE_BINARY_DIR}/wordle.profraw")
#   set(COVERAGE_RAW_FILE_TEST "${CMAKE_BINARY_DIR}/wordletest.profraw")
#   set(COVERAGE_DATA_FILE "${CMAKE_BINARY_DIR}/coverage.profdata")
#   set(COVERAGE_HTML_DIR "${CMAKE_BINARY_DIR}/coverage_html")
#   set(COVERAGE_HTML_DIR_TEST "${CMAKE_BINARY_DIR}/coverage_html_test")

#   # Custom target to run `wordle` with coverage
#   add_custom_target(run-wordle
#     COMMAND LLVM_PROFILE_FILE=${COVERAGE_RAW_FILE} ./wordle
#     DEPENDS wordle
#     COMMENT "Running wordle with coverage..."
#     VERBATIM
#   )

#   # Custom target to run `wordletest` with coverage
#   add_custom_target(run-wordletest
#     COMMAND LLVM_PROFILE_FILE=${COVERAGE_RAW_FILE_TEST} ./tests/wordletest
#     DEPENDS tests/wordletest
#     COMMENT "Running wordletest with coverage..."
#     VERBATIM
#   )

#   # Merge coverage data from wordle and wordletest
#   add_custom_target(coverage-merge
#     COMMAND llvm-profdata merge -sparse ${COVERAGE_RAW_FILE} ${COVERAGE_RAW_FILE_TEST} -o ${COVERAGE_DATA_FILE}
#     DEPENDS run-wordle run-wordletest
#     COMMENT "Merging coverage data..."
#     VERBATIM
#   )

#   # Generate text coverage report for wordle
#   add_custom_target(coverage-wordle-text
#     COMMAND llvm-cov report ./wordle -instr-profile=${COVERAGE_DATA_FILE}
#     DEPENDS coverage-merge
#     COMMENT "Generating text coverage report for wordle..."
#     VERBATIM
#   )

#   # Generate text coverage report for wordletest
#   add_custom_target(coverage-wordletest-text
#     COMMAND llvm-cov report ./tests/wordletest -instr-profile=${COVERAGE_DATA_FILE}
#     DEPENDS coverage-merge
#     COMMENT "Generating text coverage report for wordletest..."
#     VERBATIM
#   )

#   # Generate HTML coverage report for wordle
#   add_custom_target(coverage-wordle-html
#     COMMAND llvm-cov show ./wordle -instr-profile=${COVERAGE_DATA_FILE} -format=html
#   -output-dir=${COVERAGE_HTML_DIR} -ignore-filename-regex=${TEST_IGNORE_REGEX}
#     DEPENDS coverage-merge
#     COMMENT "Generating HTML coverage report for wordle..."
#     VERBATIM
#   )

#   # Generate HTML coverage report for wordletest
#   add_custom_target(coverage-wordletest-html
#     COMMAND llvm-cov show ./tests/wordletest -instr-profile=${COVERAGE_DATA_FILE}
#   -format=html -output-dir=${COVERAGE_HTML_DIR_TEST} -ignore-filename-regex=${TEST_IGNORE_REGEX}
#     DEPENDS coverage-merge
#     COMMENT "Generating HTML coverage report for wordletest..."
#     VERBATIM
#   )
# endif()
