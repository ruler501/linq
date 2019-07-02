# CMakeList.txt : CMake project for Linq, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

include(GoogleTest)

project (LinqTest)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Setup the test and configure it to use GoogleTest
# Need to include the header files to make visual studio happy
add_executable (LinqTest "tests/linq.cpp" "util.h" "linq.h")

target_link_libraries (LinqTest gtest_main)

set_property (TARGET LinqTest 
			  PROPERTY CXX_STANDARD
			  17)

if(MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else(MSVC)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/tests")
    include(CodeCoverage)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic ${CMAKE_CXX_FLAGS_COVERAGE}")
    SETUP_TARGET_FOR_COVERAGE(LinqCoverage LinqTest coverage)
endif(MSVC)

enable_testing ()

gtest_discover_tests (LinqTest)
