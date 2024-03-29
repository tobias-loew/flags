// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#define TEST_NAMESPACE test_basics
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
    flags_enum : unsigned int{
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




void test_nth_bit() {

    BOOST_TEST_EQ(to_underlying(flags_enum::bit_0), 1);
    BOOST_TEST_EQ(to_underlying(flags_enum::bit_1), 2);
    BOOST_TEST_EQ(to_underlying(flags_enum::bit_2), 4);
    BOOST_TEST_EQ(to_underlying(flags_enum::bit_3), 8);

    // test whole range of int
    for (int i = 0; i < static_cast<int>(sizeof(int) * 8); ++i) {
        BOOST_TEST_EQ(boost::flags::nth_bit(i), 1 << i);
    }
}

namespace nth_bit_ns {
    enum
#ifndef TEST_COMPILE_UNSCOPED
        class
#endif // TEST_COMPILE_UNSCOPED
    flags_enum: unsigned int {
    a = boost::flags::nth_bit<uint8_t>(0), // == 0x01
        b = boost::flags::nth_bit<flags_enum>(1), // == 0x02
        c = boost::flags::nth_bit<int>(2), // == 0x04
        d = boost::flags::nth_bit(3), // == 0x08
        e = boost::flags::next_bit(d), // == 0x08
        f = boost::flags::next_bit(e), // == 0x08
    };
    // enable flags_enum
    BOOST_FLAGS_ENABLE(flags_enum)
}
void test_nth_bit_type() {

    static_assert(boost::flags::get_underlying(nth_bit_ns::flags_enum::e) == boost::flags::nth_bit<unsigned int>(4), "");
    static_assert(boost::flags::get_underlying(nth_bit_ns::flags_enum::f) == boost::flags::nth_bit<unsigned int>(5), "");
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

    // prevent unused warnings
    (void)(c1);
    (void)(c2);
    (void)(c3);
    (void)(c4);
    (void)(c5);


    static_assert(std::is_same<decltype(c1), boost::flags::complement<decltype(c0)>>::value, "error in complement type");

    static_assert(std::is_same<decltype(c0), decltype(c2)>::value, "error in complement type");
    static_assert(std::is_same<decltype(c2), decltype(c4)>::value, "error in complement type");
    static_assert(std::is_same<decltype(c1), decltype(c3)>::value, "error in complement type");
    static_assert(std::is_same<decltype(c3), decltype(c5)>::value, "error in complement type");

    static_assert(!std::is_same<decltype(c0), decltype(c1)>::value, "error in complement type");

}


void test_binary_operators() {

    flags_enum a = flags_enum::bit_0;
    flags_enum b = flags_enum::bit_1;
    flags_enum c = flags_enum::bit_2;
    flags_enum d = flags_enum::bit_3;

    auto ab = a | b;
    auto ac = a | c;
    auto ad = a | d;
    auto acd = a | c | d;
    auto ab_and_ac = ab & ac;
    auto neg_a = ~a;
    auto neg_ac = ~ac;
    auto neg_ad = ~ad;
    auto neg_acd = ~acd;

    auto neg_ad_neg_ac = neg_ad | neg_ac;
    auto neg_ad_and_neg_ac = neg_ad & neg_ac;

    BOOST_TEST(ab_and_ac == a);
    BOOST_TEST((neg_a & d) == d);
    BOOST_TEST((neg_ac & d) == d);
    BOOST_TEST(neg_ad_neg_ac == neg_a);
    BOOST_TEST(neg_ad_and_neg_ac == neg_acd);
    BOOST_TEST(neg_a == (neg_ac|c));

    BOOST_TEST((ac ^ d) == acd);
    BOOST_TEST((acd ^ d) == ac);
}


void test_binary_assignment() {

    flags_enum a = flags_enum::bit_0;
    flags_enum b = flags_enum::bit_1;
    flags_enum c = flags_enum::bit_2;
    flags_enum d = flags_enum::bit_3;

    auto value = a | b;

    BOOST_TEST(value == (a | b));

    value |= c;

    BOOST_TEST(value == (a | b | c));

    value &= ~b;

    BOOST_TEST(value == (a | c));

    value &= (c | d);

    BOOST_TEST(value == (c));

    auto mask = ~value;

    mask ^= value;

    BOOST_TEST(~mask == flags_enum{});

}


void test_null() {
    flags_enum a = flags_enum::bit_0;

    BOOST_TEST((a != nullptr));
    BOOST_TEST(!(a == nullptr));

#if !defined(TEST_COMPILE_UNSCOPED) || defined(TEST_COMPILE_FAIL_UNSCOPED)
    BOOST_TEST((a != 0));
    BOOST_TEST(!(a == 0));
#endif

    BOOST_TEST((a != BOOST_FLAGS_NULL));
    BOOST_TEST(!(a == BOOST_FLAGS_NULL));
}

void test_bfand() {
    flags_enum a = flags_enum::bit_0;
    flags_enum b = flags_enum::bit_1;
    flags_enum c = flags_enum::bit_2;
    flags_enum d = flags_enum::bit_3;

    BOOST_TEST(!(a BOOST_FLAGS_AND b));
    BOOST_TEST(((a | b) BOOST_FLAGS_AND b));
    BOOST_TEST((a | b) BOOST_FLAGS_AND(b | c));

    BOOST_TEST((a | b) BOOST_FLAGS_AND(b | c));
    BOOST_TEST(!((a | b) BOOST_FLAGS_AND(b & c)));
    BOOST_TEST(!((a | b) BOOST_FLAGS_AND(c | d)));
}



namespace test_in_class_ns {
    namespace test_1_ns {
        struct s {
            enum
#ifndef TEST_COMPILE_UNSCOPED
                class
#endif // TEST_COMPILE_UNSCOPED
            flags_enum: unsigned int {
                a = boost::flags::nth_bit(0), // == 0x01
                b = boost::flags::nth_bit(1), // == 0x02
                c = boost::flags::nth_bit(2), // == 0x04
                d = boost::flags::nth_bit(3), // == 0x08
            };
            // enable flags_enum
            BOOST_FLAGS_ENABLE_LOCAL(flags_enum)

        };
    }

    namespace test_2_ns {
        struct s {
            enum
#ifndef TEST_COMPILE_UNSCOPED
                class
#endif // TEST_COMPILE_UNSCOPED
            flags_enum: unsigned int {
            a = boost::flags::nth_bit(0), // == 0x01
                b = boost::flags::nth_bit(1), // == 0x02
                c = boost::flags::nth_bit(2), // == 0x04
                d = boost::flags::nth_bit(3), // == 0x08
            };

            // enable flags_enum
            BOOST_FLAGS_ENABLE_LOCAL(flags_enum)
        };
    }

}


void test_in_class() {

    {
#ifdef TEST_COMPILE_UNSCOPED
        using s = test_in_class_ns::test_1_ns::s;
#else // TEST_COMPILE_UNSCOPED
        using s = test_in_class_ns::test_1_ns::s::flags_enum;
#endif // TEST_COMPILE_UNSCOPED

        // check De Morgan's laws
        BOOST_TEST(~(s::a & s::b) == (~s::a | ~s::b));
        BOOST_TEST(~(s::a | s::b) == (~s::a & ~s::b));
    }

    {
#ifdef TEST_COMPILE_UNSCOPED
        using s = test_in_class_ns::test_2_ns::s;
#else // TEST_COMPILE_UNSCOPED
        using s = test_in_class_ns::test_2_ns::s::flags_enum;
#endif // TEST_COMPILE_UNSCOPED

        // check De Morgan's laws
        BOOST_TEST(~(s::a & s::b) == (~s::a | ~s::b));
        BOOST_TEST(~(s::a | s::b) == (~s::a & ~s::b));
    }

}


int main() {
    report_config();
    test_nth_bit();
    test_nth_bit_type();
    test_negation_operators();
    test_complement_types();
    test_binary_operators();
    test_binary_assignment();
    test_null();
    test_bfand();
    test_in_class();

    return boost::report_errors();
}

#if defined(TEST_FLAGS_LINKING)
} // namespace TEST_NAMESPACE
#endif // defined(TEST_FLAGS_LINKING)

