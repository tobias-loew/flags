// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <iostream>


// helper to report compiler environment

#define TEST_REPORT_CONFIG_STREAM BOOST_LIGHTWEIGHT_TEST_OSTREAM
//#define TEST_REPORT_CONFIG_STREAM std::cout

#define TEST_REPORT_CONFIG(name)  TEST_REPORT_CONFIG_STREAM << #name << " : " << name << "\n"

inline bool report_config() {

#if defined(__GNUC__)
    TEST_REPORT_CONFIG(__GNUC__);
#else 
    TEST_REPORT_CONFIG_STREAM << "__GNUC__ not defined\n";
#endif

#if defined(__clang__)
    TEST_REPORT_CONFIG(__clang__);
#else 
    TEST_REPORT_CONFIG_STREAM << "__clang__ not defined\n";
#endif

#if defined(__MINGW32__)
    TEST_REPORT_CONFIG(__MINGW32__);
#else 
    TEST_REPORT_CONFIG_STREAM << "__MINGW32__ not defined\n";
#endif

    TEST_REPORT_CONFIG(BOOST_FLAGS_IS_GCC_COMPILER);
    TEST_REPORT_CONFIG(BOOST_FLAGS_HAS_THREE_WAY_COMPARISON);
    TEST_REPORT_CONFIG(BOOST_FLAGS_HAS_PARTIAL_ORDERING);
    TEST_REPORT_CONFIG(BOOST_FLAGS_HAS_CONCEPTS);
    TEST_REPORT_CONFIG(BOOST_FLAGS_HAS_IS_SCOPED_ENUM);
    TEST_REPORT_CONFIG(BOOST_FLAGS_HAS_LOGICAL_TRAITS);
    TEST_REPORT_CONFIG(BOOST_FLAGS_HAS_INLINE_VARIABLES);

    return true;
}

