cmake_minimum_required(VERSION 3.12...3.30)
project(boost_flags)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

##################################################
# C++ standard version selection
##################################################
set(CXX_STD 20 CACHE STRING "Set to 14, 17, etc., to enable C++14, C++17, etc.")
message("-- Using -std=c++${CXX_STD}")

if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    add_definitions(-g -Wall)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    add_definitions(-g -Wall)
endif ()

##################################################
# Build config
##################################################


##################################################
# Dependencies
##################################################
include(dependencies)


##################################################
# Sanitizers
##################################################
set(USE_ASAN false CACHE BOOL "Set to true to enable -fsanitize=address when building tests.")
set(USE_UBSAN false CACHE BOOL "Set to true to enable -fsanitize=undefined when building tests.")
if (USE_ASAN AND USE_UBSAN)
    message(FATAL_ERROR "USE_ASAN and USE_UBSAN must not be enabled at the same time")
elseif (USE_ASAN)
    set(link_flags -fsanitize=address)
elseif (USE_UBSAN)
    set(link_flags -fsanitize=undefined)
endif()


##################################################
# flags sub-library
##################################################
add_library(boost_flags INTERFACE)
target_include_directories(boost_flags INTERFACE ${CMAKE_SOURCE_DIR}/include)
if (Boost_FOUND)
   target_include_directories(boost_flags INTERFACE ${Boost_INCLUDE_DIRS})
endif()

include_directories(INTERFACE ${CMAKE_SOURCE_DIR}/include)
if (Boost_FOUND)
   include_directories(INTERFACE ${Boost_INCLUDE_DIRS})
endif()

##################################################
# Subdirectories
##################################################
add_subdirectory(test)
