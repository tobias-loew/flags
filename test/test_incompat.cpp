// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#define TEST_NAMESPACE 
#include "include_test.hpp"

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>

// only testing for unscoped enums

enum class flags_enum {};

// enable flags_enum
TEST_FLAGS_LINKING_PREAMBLE
template<> struct boost_flags_enable<TEST_FLAGS_LINKING_NAMESPACE flags_enum> : std::true_type {};
TEST_FLAGS_LINKING_EPILOGUE




enum class flags2_enum {};

// enable flags_enum
TEST_FLAGS_LINKING_PREAMBLE
template<> struct boost_flags_enable<TEST_FLAGS_LINKING_NAMESPACE flags2_enum> : std::true_type {};
TEST_FLAGS_LINKING_EPILOGUE



enum class not_enabled_enum {};



// helpers
template<typename E>
constexpr auto to_underlying(E value)
#if defined(TEST_NO_CXX14_DECLTYPE_AUTO)
-> typename std::underlying_type<E>::type
#endif // defined(TEST_NO_CXX14_DECLTYPE_AUTO)
{
    return static_cast<typename std::underlying_type<E>::type>(value);
}




void test_fail_with_enabled() {
#if defined(TEST_COMPILE_FAIL_WITH_ENABLED)
    auto v = flags_enum{} & flags2_enum{};
#endif // defined(TEST_COMPILE_FAIL_WITH_ENABLED)
}

void test_fail_with_not_enabled() {
#if defined(TEST_COMPILE_FAIL_WITH_NOT_ENABLED)
    auto v = flags_enum{} & not_enabled_enum{};
#endif // defined(TEST_COMPILE_FAIL_WITH_NOT_ENABLED)
}

int main() {
    test_fail_with_enabled();
    test_fail_with_not_enabled();

    return boost::report_errors();
}