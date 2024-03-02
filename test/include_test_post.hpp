// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <iostream>

#define TEST_REPORT_CONFIG(name)  BOOST_LIGHTWEIGHT_TEST_OSTREAM << #name << " : " << name << "\n"

inline bool report_config() {
    BOOST_LIGHTWEIGHT_TEST_OSTREAM << "\n\n\nreporting config to BOOST_LIGHTWEIGHT_TEST_OSTREAM \n\n\n";
    std::cout << "\n\n\nreporting config to std::cout\n\n\n";

#if defined(__GNUC__)
    TEST_REPORT_CONFIG(__GNUC__);
#else 
    BOOST_LIGHTWEIGHT_TEST_OSTREAM << "__GNUC__ not defined\n";
#endif

#if defined(__clang__)
    TEST_REPORT_CONFIG(__clang__);
#else 
    BOOST_LIGHTWEIGHT_TEST_OSTREAM << "__clang__ not defined\n";
#endif

#if defined(__MINGW32__)
    TEST_REPORT_CONFIG(__MINGW32__);
#else 
    BOOST_LIGHTWEIGHT_TEST_OSTREAM << "__MINGW32__ not defined\n";
#endif

    TEST_REPORT_CONFIG(BOOST_FLAGS_IS_GCC_COMPILER);
    TEST_REPORT_CONFIG(BOOST_FLAGS_EMULATE_THREE_WAY_COMPARISON);
    TEST_REPORT_CONFIG(BOOST_FLAGS_EMULATE_PARTIAL_ORDERING);
    TEST_REPORT_CONFIG(BOOST_FLAGS_HAS_CONCEPTS);
    TEST_REPORT_CONFIG(BOOST_FLAGS_HAS_IS_SCOPED_ENUM);
    TEST_REPORT_CONFIG(BOOST_FLAGS_HAS_LOGICAL_TRAITS);
    TEST_REPORT_CONFIG(BOOST_FLAGS_HAS_INLINE_VARIABLES);

    return true;
}

static bool output_config = report_config();