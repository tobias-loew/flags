// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#define TEST_NAMESPACE test_disable_complement
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


// enable flags_enum
#ifndef TEST_COMPILE_FAIL_NO_LOGICAL_OPERATOR
constexpr inline boost::flags::options_constant<boost::flags::options::enable | boost::flags::options::logical_and> boost_flags_enable(flags_enum) { return {}; }
#else // TEST_COMPILE_FAIL_NO_LOGICAL_OPERATOR
constexpr inline boost::flags::options_constant<boost::flags::options::enable> boost_flags_enable(flags_enum) { return {}; }
#endif // TEST_COMPILE_FAIL_NO_LOGICAL_OPERATOR
//BOOST_FLAGS_FORWARD_OPERATORS(flags_enum)
BOOST_FLAGS_USING_OPERATORS()
    

// helpers
//template<typename E>
//constexpr auto to_underlying(E value)
//#if defined(TEST_NO_CXX14_DECLTYPE_AUTO)
//-> typename std::underlying_type<E>::type
//#endif // defined(TEST_NO_CXX14_DECLTYPE_AUTO)
//{
//    return static_cast<typename std::underlying_type<E>::type>(value);
//}



void test_logical_operators() {

    {
        bool b = flags_enum::bit_0 && flags_enum::bit_0;
        BOOST_TEST(b);
    }
    {
        bool b = flags_enum::bit_0 && flags_enum::bit_1;
        BOOST_TEST(!b);
    }

    {
        bool b = flags_enum::bit_0 && ~flags_enum::bit_0;
        BOOST_TEST(!b);
    }
    {
        bool b = flags_enum::bit_0 && ~flags_enum::bit_1;
        BOOST_TEST(b);
    }

    {
        bool b = ~flags_enum::bit_0 && flags_enum::bit_0;
        BOOST_TEST(!b);
    }
    {
        bool b = ~flags_enum::bit_0 && flags_enum::bit_1;
        BOOST_TEST(b);
    }

}



int main() {
    report_config();
    test_logical_operators();

    return boost::report_errors();
}

#if defined(TEST_FLAGS_LINKING)
} // namespace TEST_NAMESPACE
#endif // defined(TEST_FLAGS_LINKING)
