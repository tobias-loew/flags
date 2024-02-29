cmake_minimum_required(VERSION 3.5)
project(boost_flags)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

##################################################
# C++ standard version selection
##################################################
set(CXX_STD 17 CACHE STRING "Set to 14, 17, etc., to enable C++14, C++17, etc.")
message("-- Using -std=c++${CXX_STD}")

# SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/proj/boost_1_84_0")
# set(Boost_DEBUG ON)
# set(BOOST_ROOT "/mnt/c/proj/boost_1_84_0")
# set(BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/lib/)
# set(Boost_INCLUDE_DIR ${BOOST_ROOT})
# set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_MULTITHREADED OFF)
# set(Boost_USE_STATIC_RUNTIME OFF)
set( Boost_ADDITIONAL_VERSIONS 1.84.0 1.84)
# find_package(Boost)
# list(APPEND CMAKE_MODULE_PATH "C:\\proj")

if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    add_definitions(-g -Wall)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    add_definitions(-g -Wall)
endif ()

##################################################
# Build config
##################################################
# set(BUILD_WITHOUT_HANA false CACHE BOOL "Build with BOOST_PARSER_DISABLE_HANA_TUPLE defined.")
# if (BUILD_WITHOUT_HANA)
#   add_definitions(-DBOOST_PARSER_DISABLE_HANA_TUPLE)
# endif()


##################################################
# Dependencies
##################################################
include(dependencies)
# 
# find_package(PythonInterp)
# if (PYTHONINTERP_FOUND)
#     message("-- Found Python ${PYTHON_VERSION_STRING} (${PYTHON_EXECUTABLE})")
# endif ()


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
#target_include_directories(boost_flags PUBLIC ${Boost_INCLUDE_DIRS}) 
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
# add_subdirectory(example)
