// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>

enum class flags_enum {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable flags_enum
template<> struct boost::flags::enable<flags_enum> : std::true_type, boost::flags::option_disable_complement {};


// helpers
template<typename E>
constexpr auto to_underlying(E value) {
    return static_cast<std::underlying_type_t<E>>(value);
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

    static_assert(!std::is_same_v<decltype(c1), boost::flags::complement<decltype(c0)>>, "error in complement type");

    static_assert(std::is_same_v<decltype(c0), decltype(c1)>, "error in complement type");
    static_assert(std::is_same_v<decltype(c1), decltype(c2)>, "error in complement type");
    static_assert(std::is_same_v<decltype(c2), decltype(c3)>, "error in complement type");
    static_assert(std::is_same_v<decltype(c3), decltype(c4)>, "error in complement type");
    static_assert(std::is_same_v<decltype(c4), decltype(c5)>, "error in complement type");
}





int main() {
    test_negation_operators();
    test_complement_types();

    return boost::report_errors();
}