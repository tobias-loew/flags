// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>

// only testing for unscoped enums

enum class flags_enum {};

// enable flags_enum
template<> struct boost::flags::enable<flags_enum> : std::true_type {};




enum class flags2_enum {};

// enable flags_enum
template<> struct boost::flags::enable<flags2_enum> : std::true_type {};



enum class not_enabled_enum {};



// helpers
template<typename E>
constexpr auto to_underlying(E value) {
    return static_cast<std::underlying_type_t<E>>(value);
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