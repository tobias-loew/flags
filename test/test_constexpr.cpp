
// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#define TEST_NAMESPACE test_constexpr
#include "include_test.hpp"

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>

#include "include_test_post.hpp"

#if defined(TEST_FLAGS_LINKING)
namespace TEST_NAMESPACE {
#endif // defined(TEST_FLAGS_LINKING)

enum 
#ifndef TEST_COMPILE_UNSCOPED
    class
#endif // TEST_COMPILE_UNSCOPED
    flags_enum : uint8_t {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable flags_enum
BOOST_FLAGS_ENABLE(flags_enum)


// helpers
template<typename E>
constexpr auto to_underlying(E value) 
#if defined(TEST_NO_CXX14_DECLTYPE_AUTO)
-> typename std::underlying_type<E>::type
#endif // defined(TEST_NO_CXX14_DECLTYPE_AUTO)
{
    return static_cast<typename std::underlying_type<E>::type>(value);
}


#if __cplusplus >= 201402L
constexpr bool check_nth_bit() {
    // test whole range of int
    for (int i = 0; i < static_cast<int>(sizeof(int) * 8); ++i) {
        if (!(boost::flags::nth_bit(i) == 1 << i)) {
            return false;
        }
    }
    return true;
}
#endif

void test_nth_bit() {

    static_assert(to_underlying(flags_enum::bit_0) == 1, "constexpr test failed");
    static_assert(to_underlying(flags_enum::bit_1) == 2, "constexpr test failed");
    static_assert(to_underlying(flags_enum::bit_2) == 4, "constexpr test failed");
    static_assert(to_underlying(flags_enum::bit_3) == 8, "constexpr test failed");

    static_assert(boost::flags::nth_bit(0) == 1 << 0, "constexpr test failed");
    static_assert(boost::flags::nth_bit(1) == 1 << 1, "constexpr test failed");
    static_assert(boost::flags::nth_bit(2) == 1 << 2, "constexpr test failed");
    static_assert(boost::flags::nth_bit(3) == 1 << 3, "constexpr test failed");

    // requires C++14
#if __cplusplus >= 201402L
    static_assert(check_nth_bit(), "constexpr test failed");
#endif
}



#if __cplusplus >= 201402L
constexpr bool check_negation_operators() {
    const flags_enum test_cases[] = {
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

    // test whole range of int
    for (auto value : test_cases) {
        if (!(value == ~~value)) {
            return false;
        }
        if (!(~value == ~~~value)) {
            return false;
        }
        }
    return true;
}
#endif

void test_negation_operators() {
    // requires C++14
#if __cplusplus >= 201402L
    static_assert(check_negation_operators(), "constexpr test failed");
#endif
}

void test_binary_operators() {

    static constexpr flags_enum a = flags_enum::bit_0;
    static constexpr flags_enum b = flags_enum::bit_1;
    static constexpr flags_enum c = flags_enum::bit_2;
    static constexpr flags_enum d = flags_enum::bit_3;

    static constexpr auto ab = a | b;
    static constexpr auto ac = a | c;
    static constexpr auto ad = a | d;
    static constexpr auto acd = a | c | d;
    static constexpr auto ab_and_ac = ab & ac;
    static constexpr auto neg_a = ~a;
    static constexpr auto neg_ac = ~ac;
    static constexpr auto neg_ad = ~ad;
    static constexpr auto neg_acd = ~acd;

    static constexpr auto neg_ad_neg_ac = neg_ad | neg_ac;
    static constexpr auto neg_ad_and_neg_ac = neg_ad & neg_ac;

    // silence unused warnings
    (void)ab;
    (void)ac;
    (void)ad;
    (void)acd;
    (void)ab_and_ac;
    (void)neg_a;
    (void)neg_ac;
    (void)neg_ad;
    (void)neg_acd;
    (void)neg_ad_neg_ac;
    (void)neg_ad_and_neg_ac;


    static_assert(ab_and_ac == a, "constexpr test failed");
    static_assert((neg_a & d) == d, "constexpr test failed");
    static_assert((neg_ac & d) == d, "constexpr test failed");
    static_assert(neg_ad_neg_ac == neg_a, "constexpr test failed");
    static_assert(neg_ad_and_neg_ac == neg_acd, "constexpr test failed");
    static_assert(neg_a == (neg_ac|c), "constexpr test failed");

    static_assert((ac ^ d) == acd, "constexpr test failed");
    static_assert((acd ^ d) == ac, "constexpr test failed");
}


void test_null() {
    static constexpr flags_enum a = flags_enum::bit_0;

    // silence unused warnings
    (void)a;

    static_assert((a != nullptr), "constexpr test failed");
    static_assert(!(a == nullptr), "constexpr test failed");

#if !defined(TEST_COMPILE_UNSCOPED) || defined(TEST_COMPILE_FAIL_UNSCOPED)
    static_assert((a != 0), "constexpr test failed");
    static_assert(!(a == 0), "constexpr test failed");
#endif

    static_assert((a != BOOST_FLAGS_NULL), "constexpr test failed");
    static_assert(!(a == BOOST_FLAGS_NULL), "constexpr test failed");
}

void test_bfand() {
    static constexpr flags_enum a = flags_enum::bit_0;
    static constexpr flags_enum b = flags_enum::bit_1;
    static constexpr flags_enum c = flags_enum::bit_2;
    static constexpr flags_enum d = flags_enum::bit_3;

    // silence unused warnings
    (void)a;
    (void)b;
    (void)c;
    (void)d;

    static_assert(!(a BOOST_FLAGS_AND b), "constexpr test failed");
    static_assert(((a | b) BOOST_FLAGS_AND b), "constexpr test failed");
    static_assert((a | b) BOOST_FLAGS_AND (b | c), "constexpr test failed");
    
    static_assert((a | b) BOOST_FLAGS_AND (b | c), "constexpr test failed");
    static_assert(!((a | b) BOOST_FLAGS_AND (b & c)), "constexpr test failed");
    static_assert(!((a | b) BOOST_FLAGS_AND (c | d)), "constexpr test failed");
}


int main() {
    report_config();
    test_nth_bit();
    test_negation_operators();
    test_binary_operators();
    test_null();
    test_bfand();

    return boost::report_errors();
}

#if defined(TEST_FLAGS_LINKING)
} // namespace TEST_NAMESPACE
#endif // defined(TEST_FLAGS_LINKING)

