include_directories(${CMAKE_HOME_DIRECTORY})

include(CTest)

enable_testing()

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -j4 -C ${CMAKE_CFG_INTDIR})

##################################################
# Flags tests
##################################################
#add_executable(parser_ parser.cpp)
#set_property(TARGET parser_ PROPERTY CXX_STANDARD ${CXX_STD})
#target_link_libraries(parser_ parser gtest gtest_main ${link_flags})
#if (MSVC)
#    target_compile_options(parser_ PRIVATE /source-charset:utf-8 /bigobj)
#endif ()

#add_test(NAME parser_ COMMAND parser_ ---gtest_catch_exceptions=1)
#
#add_executable(parser_api parser_api.cpp)
#set_property(TARGET parser_api PROPERTY CXX_STANDARD ${CXX_STD})
#target_link_libraries(parser_api parser gtest gtest_main ${link_flags})
#if (MSVC)
#    target_compile_options(parser_api PRIVATE /source-charset:utf-8 /bigobj)
#endif ()
#add_test(NAME parser_api COMMAND parser_api ---gtest_catch_exceptions=1)
#
#add_executable(
#    compile_tests
#    compile_tests_main.cpp
#    compile_attribute.cpp
#    compile_seq_attribute.cpp
#    compile_or_attribute.cpp
#    compile_combining_groups.cpp
#)
#set_property(TARGET compile_tests PROPERTY CXX_STANDARD ${CXX_STD})
#target_link_libraries(compile_tests parser)

macro(add_test_executable name)
    add_executable(${name} ${name}.cpp)
    target_include_directories(${name} INTERFACE ${Boost_INCLUDE_DIRS})
	target_include_directories(${name} INTERFACE ${CMAKE_SOURCE_DIR}/include)
	if (Boost_FOUND)
	   target_include_directories(${name} INTERFACE ${Boost_INCLUDE_DIRS})
	endif()
    set_property(TARGET ${name} PROPERTY CXX_STANDARD ${CXX_STD})
    add_test(NAME ${name} COMMAND ${name} --gtest_catch_exceptions=1)
endmacro()

add_test_executable(test_basics)
