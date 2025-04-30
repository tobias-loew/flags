// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#define TEST_NAMESPACE test_bitfield
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

namespace ns1 {
    struct bf_struct {
        int i : 3;
        flags_enum f : 4;
    };
}

void test_bitfield_const() {
    ns1::bf_struct s;
    s.i = 1;
    s.f = flags_enum::bit_0 | flags_enum::bit_1;
    BOOST_TEST_EQ(s.f, flags_enum::bit_0 | flags_enum::bit_1);
    s.f = s.f | flags_enum::bit_2;
    BOOST_TEST_EQ(s.f, flags_enum::bit_0 | flags_enum::bit_1 | flags_enum::bit_2);
    s.f = s.f & ~flags_enum::bit_1;
    BOOST_TEST_EQ(s.f, flags_enum::bit_0 | flags_enum::bit_2);
}

namespace ns2 {
    struct bf_struct {
        int i : 3;
        flags_enum f : 4;

        BOOST_FLAGS_BITFIELD_PROXY(f)
    };
}

void test_bitfield_assigment() {
    {
        ns2::bf_struct s;
        s.i = 1;
        s.f = flags_enum::bit_0 | flags_enum::bit_1;
        BOOST_TEST_EQ(s.f, flags_enum::bit_0 | flags_enum::bit_1);
        s.f = s.f | flags_enum::bit_2;
        BOOST_TEST_EQ(s.f, flags_enum::bit_0 | flags_enum::bit_1 | flags_enum::bit_2);
        s.f = s.f & ~flags_enum::bit_1;
        BOOST_TEST_EQ(s.f, flags_enum::bit_0 | flags_enum::bit_2);
    }

    {
        ns2::bf_struct s;
        s.i = 1;
        s.f = flags_enum::bit_0 | flags_enum::bit_1;
        BOOST_TEST_EQ(s.f, flags_enum::bit_0 | flags_enum::bit_1);
        s.f_ref() |= flags_enum::bit_2;
        BOOST_TEST_EQ(s.f, flags_enum::bit_0 | flags_enum::bit_1 | flags_enum::bit_2);
        s.f_ref() &= ~flags_enum::bit_1;
        BOOST_TEST_EQ(s.f, flags_enum::bit_0 | flags_enum::bit_2);
    }

}




int main() {
    report_config();
    test_bitfield_const();
    test_bitfield_assigment();

    return boost::report_errors();
}

#if defined(TEST_FLAGS_LINKING)
} // namespace TEST_NAMESPACE
#endif // defined(TEST_FLAGS_LINKING)

