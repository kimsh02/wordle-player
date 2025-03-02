CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(wp)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

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
include_directories(${CMAKE_SOURCE_DIR}/headers)
# Implementation
file(GLOB SOURCES
  ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(wp ${SOURCES})
target_link_libraries(wp PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
