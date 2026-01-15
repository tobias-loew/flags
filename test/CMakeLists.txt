# only used with VisualStudio/msvc for testing the tests

include_directories(${CMAKE_HOME_DIRECTORY})

include(CTest)

enable_testing()

if (MSVC)
   set(CXX_STANDARD "/std:c++latest")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_STANDARD} /EHsc /permissive- /Zc:wchar_t /Zc:forScope /Zc:inline /Zc:preprocessor")
else()

endif()


add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -j4 -C ${CMAKE_CFG_INTDIR})


set(Boost_INCLUDE_DIRS "C:/proj/boost_1_85_0/")

macro(add_test_executable name)
    add_executable(${name} ${name}.cpp)
    target_include_directories(${name} PUBLIC ${Boost_INCLUDE_DIRS})
    target_include_directories(${name} PUBLIC ${CMAKE_SOURCE_DIR}/../include)
    if (Boost_FOUND)
       target_include_directories(${name} INTERFACE ${Boost_INCLUDE_DIRS})
    endif()
    add_test(NAME ${name} COMMAND ${name} --gtest_catch_exceptions=1)

# add unscoped version
    add_executable(${name}_unscoped ${name}.cpp)
    target_compile_definitions(${name}_unscoped PRIVATE   
           TEST_COMPILE_UNSCOPED  
        )   
    target_include_directories(${name}_unscoped PUBLIC ${Boost_INCLUDE_DIRS})
    target_include_directories(${name}_unscoped PUBLIC ${CMAKE_SOURCE_DIR}/../include)
    if (Boost_FOUND)
       target_include_directories(${name}_unscoped INTERFACE ${Boost_INCLUDE_DIRS})
    endif()
    add_test(NAME ${name}_unscoped COMMAND ${name}_unscoped --gtest_catch_exceptions=1)
endmacro()

add_test_executable(test_adl)
add_test_executable(test_basics)
add_test_executable(test_constexpr)
add_test_executable(test_container_algorithm)
add_test_executable(test_disable_complement)
add_test_executable(test_fail)
add_test_executable(test_incompat)
add_test_executable(test_logical_and)
add_test_executable(test_rel_ops)
add_test_executable(test_utilities)
add_test_executable(test_macros)
add_test_executable(test_pre_increment)



macro(add_test_executable_link name)
    add_executable(${name} ${name}.cpp test_basics_link.cpp test_disable_complement_link.cpp test_rel_ops_link.cpp test_utilities_link.cpp order_pizza_link.cpp)
    target_include_directories(${name} PUBLIC ${Boost_INCLUDE_DIRS})
    target_include_directories(${name} PUBLIC ${CMAKE_SOURCE_DIR}/../include)
    if (Boost_FOUND)
       target_include_directories(${name} INTERFACE ${Boost_INCLUDE_DIRS})
    endif()
    target_compile_definitions(${name} PUBLIC TEST_FLAGS_LINKING)
    add_test(NAME ${name} COMMAND ${name} --gtest_catch_exceptions=1)
endmacro()

add_test_executable_link(test_linking)
