// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>

#define TEST_NAMESPACE test_disable_complement
#include "include_test.hpp"

#ifdef TEST_FLAGS_LINKING
namespace TEST_NAMESPACE {
#endif // TEST_FLAGS_LINKING

enum class flags_enum {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable flags_enum
TEST_FLAGS_LINKING_PREAMBLE
template<> struct boost_flags_enable<TEST_FLAGS_LINKING_NAMESPACE flags_enum> : std::true_type, boost::flags::option_disable_complement {};
TEST_FLAGS_LINKING_EPILOGUE


// helpers
template<typename E>
constexpr auto to_underlying(E value)
#ifdef TEST_NO_CXX14_DECLTYPE_AUTO
-> typename std::underlying_type<E>::type
#endif // TEST_NO_CXX14_DECLTYPE_AUTO
{
    return static_cast<typename std::underlying_type<E>::type>(value);
}



void test_negation_operators() {
    static constexpr flags_enum test_cases[] = {
        flags_enum{},
        flags_enum::bit_0,
        flags_enum::bit_1,
        flags_enum::bit_2,
        flags_enum::bit_3,
        flags_enum::bit_0 | flags_enum::bit_1,
        flags_enum::bit_0 | flags_enum::bit_2,
        flags_enum::bit_0 | flags_enum::bit_3,
        flags_enum::bit_0 | flags_enum::bit_1 | flags_enum::bit_2,
        flags_enum::bit_0 | flags_enum::bit_1 | flags_enum::bit_2 | flags_enum::bit_3,
    };

    for (auto value : test_cases) {
        BOOST_TEST((~to_underlying(value)) == (to_underlying(~value)));
        BOOST_TEST(value == ~~value);
        BOOST_TEST(~value == ~~~value);
    }
}

void test_complement_types() {
    flags_enum c0 = flags_enum::bit_0;

    auto c1 = ~c0;
    auto c2 = ~c1;
    auto c3 = ~c2;
    auto c4 = ~c3;
    auto c5 = ~c4;

    static_assert(!std::is_same<decltype(c1), boost::flags::complement<decltype(c0)>>::value, "error in complement type");

    static_assert(std::is_same<decltype(c0), decltype(c1)>::value, "error in complement type");
    static_assert(std::is_same<decltype(c1), decltype(c2)>::value, "error in complement type");
    static_assert(std::is_same<decltype(c2), decltype(c3)>::value, "error in complement type");
    static_assert(std::is_same<decltype(c3), decltype(c4)>::value, "error in complement type");
    static_assert(std::is_same<decltype(c4), decltype(c5)>::value, "error in complement type");
}





int main() {
    test_negation_operators();
    test_complement_types();

    return boost::report_errors();
}

#ifdef TEST_FLAGS_LINKING
} // namespace TEST_NAMESPACE
#endif // TEST_FLAGS_LINKING
