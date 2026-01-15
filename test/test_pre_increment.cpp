
// Copyright 2024, 2025 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#define TEST_NAMESPACE test_pre_increment
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
    flags_enum: unsigned int {
        bit_0 = boost::flags::nth_bit(0), // == 0x01
        bit_1 = boost::flags::nth_bit(1), // == 0x02
        bit_2 = boost::flags::nth_bit(2), // == 0x04
        bit_3 = boost::flags::nth_bit(3), // == 0x08
    };

    // enable relops_builtin_enum
    BOOST_FLAGS_ENABLE_PRE_INCREMENT(flags_enum)

    enum
#ifndef TEST_COMPILE_UNSCOPED
        class
#endif // TEST_COMPILE_UNSCOPED
    flags_no_pre_increment_enum: unsigned int {
        no_up_bit_0 = boost::flags::nth_bit(0), // == 0x01
        no_up_bit_1 = boost::flags::nth_bit(1), // == 0x02
        no_up_bit_2 = boost::flags::nth_bit(2), // == 0x04
        no_up_bit_3 = boost::flags::nth_bit(3), // == 0x08
    };

    // enable relops_builtin_enum
    BOOST_FLAGS_ENABLE(flags_no_pre_increment_enum)


void test_pre_increment() {


#ifdef TEST_COMPILE_FAIL_PRE_INCREMENT_1
    {
        if (flags_enum::bit_0) {
            BOOST_TEST(true);
        }
    }
#endif // TEST_COMPILE_FAIL_PRE_INCREMENT_1

#ifdef TEST_COMPILE_FAIL_PRE_INCREMENT_2
    {
        auto i = 2 + (++flags_enum::bit_0);
        BOOST_TEST(i == i);
    }
#endif // TEST_COMPILE_FAIL_PRE_INCREMENT_2

#ifdef TEST_COMPILE_FAIL_PRE_INCREMENT_3
    {
        auto i = 2 + flags_enum::bit_0;
        BOOST_TEST(i == i);
    }
#endif // TEST_COMPILE_FAIL_PRE_INCREMENT_3


#ifdef TEST_COMPILE_FAIL_PRE_INCREMENT_4
    {
        if (flags_no_pre_increment_enum::no_up_bit_0) {
            BOOST_TEST(true);
        }
    }
#endif // TEST_COMPILE_FAIL_PRE_INCREMENT_4

#ifdef TEST_COMPILE_FAIL_PRE_INCREMENT_5
    {
        auto i = 2 + (++flags_no_pre_increment_enum::no_up_bit_0);
        BOOST_TEST(i == i);
    }
#endif // TEST_COMPILE_FAIL_PRE_INCREMENT_5

#ifdef TEST_COMPILE_FAIL_PRE_INCREMENT_6
    {
        auto i = 2 + flags_no_pre_increment_enum::no_up_bit_0;
        BOOST_TEST(i == i);
    }
#endif // TEST_COMPILE_FAIL_PRE_INCREMENT_6






    {
        if (++flags_enum::bit_0) {
            BOOST_TEST(true);
        }
        else {
            BOOST_TEST(false);
        }
    }

    {
        if (++flags_enum::bit_0 | flags_enum::bit_1) {
            BOOST_TEST(true);
        }
        else {
            BOOST_TEST(false);
        }
    }

    {
        if (++flags_enum::bit_0 & flags_enum::bit_1) {
            BOOST_TEST(false);
        }
        else {
            BOOST_TEST(true);
        }
    }

    {
        if (flags_enum::bit_0 & ++flags_enum::bit_1) {
            BOOST_TEST(false);
        }
        else {
            BOOST_TEST(true);
        }
    }

    {
        if (++flags_enum::bit_0 & ++flags_enum::bit_1) {
            BOOST_TEST(false);
        }
        else {
            BOOST_TEST(true);
        }
    }

    {
        if (++(flags_enum::bit_0 & flags_enum::bit_1)) {
            BOOST_TEST(false);
        }
        else {
            BOOST_TEST(true);
        }
    }

    {
        if (++(++flags_enum::bit_0 & ++flags_enum::bit_1)) {
            BOOST_TEST(false);
        }
        else {
            BOOST_TEST(true);
        }
    }

}



int main() {
    report_config();
    test_pre_increment();

    return boost::report_errors();
}

#if defined(TEST_FLAGS_LINKING)
} // namespace TEST_NAMESPACE
#endif // defined(TEST_FLAGS_LINKING)
