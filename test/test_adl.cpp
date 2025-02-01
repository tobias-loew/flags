// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#define TEST_NAMESPACE test_adl
#include "include_test.hpp"

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>

#include "include_test_post.hpp"

#if defined(TEST_FLAGS_LINKING)
namespace TEST_NAMESPACE {
#endif // defined(TEST_FLAGS_LINKING)

// helpers
template<typename E>
constexpr auto to_underlying(E value) 
#if defined(TEST_NO_CXX14_DECLTYPE_AUTO)
-> typename std::underlying_type<E>::type
#endif // defined(TEST_NO_CXX14_DECLTYPE_AUTO)
{
    return static_cast<typename std::underlying_type<E>::type>(value);
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
#ifdef TEST_COMPILE_FAIL_ADL_1
            friend constexpr inline std::integral_constant<bool, true> boost_flags_enable(flags_enum) { return {}; }
#else
            BOOST_FLAGS_ENABLE_LOCAL(flags_enum)
#endif
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

#ifdef TEST_COMPILE_FAIL_ADL_2
            friend constexpr inline std::integral_constant<bool, true> boost_flags_enable(flags_enum) { return {}; }
#else
            BOOST_FLAGS_ENABLE_LOCAL(flags_enum)
#endif
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

        test_in_class_ns::test_1_ns::s::flags_enum v = s::a | s::b;
        v ^= s::c;

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

        test_in_class_ns::test_2_ns::s::flags_enum v = s::a | s::b;
        v ^= s::c;

        // check De Morgan's laws
        BOOST_TEST(~(s::a & s::b) == (~s::a | ~s::b));
        BOOST_TEST(~(s::a | s::b) == (~s::a & ~s::b));
    }

}

int main() {
    test_in_class();

    return boost::report_errors();
}

#if defined(TEST_FLAGS_LINKING)
} // namespace TEST_NAMESPACE
#endif // defined(TEST_FLAGS_LINKING)

