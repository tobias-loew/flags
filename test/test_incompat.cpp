// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>

#include "include_test.hpp"

// only testing for unscoped enums

enum class flags_enum {};

// enable flags_enum
TEST_GNU_ERROR_WORKAROUND_PREAMBLE
template<> struct boost_flags_enable<flags_enum> : std::true_type {};
TEST_GNU_ERROR_WORKAROUND_EPILOGUE




enum class flags2_enum {};

// enable flags_enum
TEST_GNU_ERROR_WORKAROUND_PREAMBLE
template<> struct boost_flags_enable<flags2_enum> : std::true_type {};
TEST_GNU_ERROR_WORKAROUND_EPILOGUE



enum class not_enabled_enum {};



// helpers
template<typename E>
constexpr auto to_underlying(E value)
#ifdef TEST_NO_CXX14_DECLTYPE_AUTO
-> typename std::underlying_type<E>::type
#endif // TEST_NO_CXX14_DECLTYPE_AUTO
{
    return static_cast<typename std::underlying_type<E>::type>(value);
}




void test_fail_with_enabled() {
#ifdef TEST_COMPILE_FAIL_WITH_ENABLED
    auto v = flags_enum{} & flags2_enum{};
#endif
}

void test_fail_with_not_enabled() {
#ifdef TEST_COMPILE_FAIL_WITH_NOT_ENABLED
    auto v = flags_enum{} & not_enabled_enum{};
#endif
}

int main() {
    test_fail_with_enabled();
    test_fail_with_not_enabled();

    return boost::report_errors();
}