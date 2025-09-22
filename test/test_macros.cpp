
// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#define TEST_NAMESPACE test_macros
#include "include_test.hpp"

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>
#include <array>
#include <vector>
#include <algorithm>
#include <set>

#include "include_test_post.hpp"

#if defined(TEST_FLAGS_LINKING)
namespace TEST_NAMESPACE {
#endif // defined(TEST_FLAGS_LINKING)

enum class macro_1_enum : unsigned int {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable macro_1_enum
#if defined(_MSC_VER) && (!defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL)
BOOST_FLAGS_ENABLE(macro_1_enum)
BOOST_FLAGS_REL_OPS_DELETE(macro_1_enum)
#else
BOOST_FLAGS(macro_1_enum, BOOST_FLAGS_DELETE_REL)
#endif



enum class macro_2_enum : unsigned int {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable macro_2_enum
#if defined(_MSC_VER) && (!defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL)
BOOST_FLAGS_ENABLE_DISABLE_COMPLEMENT(macro_2_enum)
#else
BOOST_FLAGS(macro_2_enum, BOOST_FLAGS_DISABLE_COMPLEMENT)
#endif


enum class macro_3_enum : unsigned int {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable macro_3_enum
#if defined(_MSC_VER) && (!defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL)
BOOST_FLAGS_ENABLE_LOGICAL_AND(macro_2_enum)
#else
BOOST_FLAGS(macro_3_enum, BOOST_FLAGS_LOGICAL_AND)
#endif


enum class macro_4_enum : unsigned int {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable macro_4_enum
#if defined(_MSC_VER) && (!defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL)
BOOST_FLAGS_ENABLE_DISABLE_COMPLEMENT_LOGICAL_AND(macro_4_enum)
BOOST_FLAGS_REL_OPS_DELETE(macro_4_enum)
#else
BOOST_FLAGS(macro_4_enum, BOOST_FLAGS_DELETE_REL, BOOST_FLAGS_DISABLE_COMPLEMENT, BOOST_FLAGS_LOGICAL_AND)
#endif



enum class macro_5_enum : unsigned int {
	bit_0 = boost::flags::nth_bit(0), // == 0x01
	bit_1 = boost::flags::nth_bit(1), // == 0x02
	bit_2 = boost::flags::nth_bit(2), // == 0x04
	bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable macro_5_enum
#if defined(_MSC_VER) && (!defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL)
BOOST_FLAGS_ENABLE(macro_5_enum)
#else
BOOST_FLAGS(macro_5_enum)
#endif

BOOST_FLAGS_SPECIALIZE_STD_LESS(macro_5_enum)




enum class macro_6_enum : unsigned int {
	bit_0 = boost::flags::nth_bit(0), // == 0x01
	bit_1 = boost::flags::nth_bit(1), // == 0x02
	bit_2 = boost::flags::nth_bit(2), // == 0x04
	bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable macro_6_enum
#if defined(_MSC_VER) && (!defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL)
BOOST_FLAGS_ENABLE(macro_6_enum)
BOOST_FLAGS_REL_OPS_DELETE(macro_6_enum)
#else
BOOST_FLAGS(macro_6_enum, BOOST_FLAGS_DELETE_REL)
#endif

BOOST_FLAGS_SPECIALIZE_STD_LESS(macro_6_enum)




#if defined(TEST_FLAGS_LINKING)

} // namespace TEST_NAMESPACE

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

template<typename T>
auto make_off_on(T v) 
#if defined(TEST_NO_CXX14_DECLTYPE_AUTO)
-> typename std::array<T, 2>
#endif // defined(TEST_NO_CXX14_DECLTYPE_AUTO)
{
    return std::array<T, 2>{{T{}, v}};  // use double braces to prevent C++11 clang warning "suggest braces around initialization of subobject"
}

void test_macro_1() {
	macro_1_enum v1 = macro_1_enum::bit_0;
	macro_1_enum v2 = macro_1_enum::bit_1;
	macro_1_enum v12 = v1 | v2;
	auto u1 = to_underlying(macro_1_enum::bit_0);
	auto u2 = to_underlying(macro_1_enum::bit_1);
	auto u12 = u1 | u2;
	BOOST_TEST((to_underlying(v12) == u12));

	bool bu = u1 < u12;
	BOOST_TEST((bu || !bu));	// to prevent unused warnings

#ifdef TEST_COMPILE_FAIL_MACROS_DELETE_REL1
	bool bv = v1 < v12;
	BOOST_TEST((bv || !bv));	// to prevent unused warnings
#endif // TEST_COMPILE_FAIL_MACROS_DELETE_REL1

}

void test_macro_2() {
	macro_2_enum v1 = macro_2_enum::bit_0;
	v1 = ~v1;
}

void test_macro_3() {
	macro_3_enum v1 = macro_3_enum::bit_0;
	macro_3_enum v2 = macro_3_enum::bit_1;
	macro_3_enum v12 = v1 | v2;
	bool b = (v12 && v2);
}


void test_macro_4() {
	macro_4_enum v1 = macro_4_enum::bit_0;
	macro_4_enum v2 = macro_4_enum::bit_3;
	macro_4_enum v12 = v1 | v2;
	auto u1 = to_underlying(macro_4_enum::bit_0);
	auto u2 = to_underlying(macro_4_enum::bit_3);
	auto u12 = u1 | u2;
	BOOST_TEST((to_underlying(v12) == u12));

	bool bu = u1 < u12;
	BOOST_TEST((bu || !bu));	// to prevent unused warnings
	bool b = (v12 && v2);
	BOOST_TEST((b || !b));	// to prevent unused warnings
	v1 = ~v1;

#ifdef TEST_COMPILE_FAIL_MACROS_DELETE_REL2
	bool bv = v1 < v12;
	BOOST_TEST((bv || !bv));	// to prevent unused warnings
#endif // TEST_COMPILE_FAIL_MACROS_DELETE_REL2

}


void test_macro_5() {
	macro_5_enum v1 = macro_5_enum::bit_0;
	macro_5_enum v2 = macro_5_enum::bit_3;
	macro_5_enum v12 = v1 | v2;
	auto u1 = to_underlying(macro_5_enum::bit_0);
	auto u2 = to_underlying(macro_5_enum::bit_3);
	auto u12 = u1 | u2;
	BOOST_TEST((to_underlying(v12) == u12));

	bool bu = u1 < u12;
	BOOST_TEST((bu || !bu));	// to prevent unused warnings
	bool bv = v1 < v12;
	BOOST_TEST((bv || !bv));	// to prevent unused warnings


	std::vector<macro_5_enum> vec = { v1, v2, v12 };
	std::sort(vec.begin(), vec.end());
	std::sort(vec.begin(), vec.end(), std::less{});
	std::sort(vec.begin(), vec.end(), std::less<macro_5_enum>{});
#if defined(__cpp_lib_ranges) && (__cpp_lib_ranges >= 201911L)
	std::ranges::sort(vec);
	std::ranges::sort(vec, std::less{});
	std::ranges::sort(vec, std::less<macro_5_enum>{});
#endif // defined(__cpp_lib_ranges) && (__cpp_lib_ranges >= 201911L

	{
		std::set<macro_5_enum> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v12);
	}
	{
		std::set<macro_5_enum, std::less<void>> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v12);
	}
	{
		std::set<macro_5_enum, std::less<macro_5_enum>> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v12);
	}
}


void test_macro_6() {
	macro_6_enum v1 = macro_6_enum::bit_0;
	macro_6_enum v2 = macro_6_enum::bit_3;
	macro_6_enum v12 = v1 | v2;
	auto u1 = to_underlying(macro_6_enum::bit_0);
	auto u2 = to_underlying(macro_6_enum::bit_3);
	auto u12 = u1 | u2;
	BOOST_TEST((to_underlying(v12) == u12));

	bool bu = u1 < u12;
	BOOST_TEST((bu || !bu));	// to prevent unused warnings

#ifdef TEST_COMPILE_FAIL_MACROS_DELETE_REL3
	bool bv = v1 < v12;
	BOOST_TEST((bv || !bv));	// to prevent unused warnings
#endif // TEST_COMPILE_FAIL_MACROS_DELETE_REL3


	std::vector<macro_6_enum> vec = { v1, v2, v12 };
#ifdef TEST_COMPILE_FAIL_MACROS_DELETE_REL_SORT1
	std::sort(vec.begin(), vec.end());
#endif // TEST_COMPILE_FAIL_MACROS_DELETE_REL_SORT1
#ifdef TEST_COMPILE_FAIL_MACROS_DELETE_REL_SORT2
	std::sort(vec.begin(), vec.end(), std::less{});
#endif // TEST_COMPILE_FAIL_MACROS_DELETE_REL_SORT2
	std::sort(vec.begin(), vec.end(), std::less<macro_6_enum>{});

#if defined(__cpp_lib_ranges) && (__cpp_lib_ranges >= 201911L)
#ifdef TEST_COMPILE_FAIL_MACROS_DELETE_REL_SORT1
	std::ranges::sort(vec);
#endif // TEST_COMPILE_FAIL_MACROS_DELETE_REL_SORT1
#ifdef TEST_COMPILE_FAIL_MACROS_DELETE_REL_SORT2
	std::ranges::sort(vec, std::less{});
#endif // TEST_COMPILE_FAIL_MACROS_DELETE_REL_SORT2
	std::ranges::sort(vec, std::less<macro_6_enum>{});
#endif // defined(__cpp_lib_ranges) && (__cpp_lib_ranges >= 201911L

	{
		std::set<macro_6_enum> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v12);
	}
#ifdef TEST_COMPILE_FAIL_MACROS_DELETE_REL_SET
	{
		std::set<macro_6_enum, std::less<void>> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v12);
	}
#endif // TEST_COMPILE_FAIL_MACROS_DELETE_REL_SET
	{
		std::set<macro_6_enum, std::less<macro_6_enum>> s;
		s.insert(v1);
		s.insert(v2);
		s.insert(v12);
	}
}



#if BOOST_FLAGS_HAS_PARTIAL_ORDERING



#endif // BOOST_FLAGS_HAS_PARTIAL_ORDERING




int main() {
    report_config();
	test_macro_1();
	test_macro_2();
	test_macro_3();
	test_macro_4();
	test_macro_5();
	test_macro_6();
    return boost::report_errors();
}

#if defined(TEST_FLAGS_LINKING)
} // namespace TEST_NAMESPACE
#endif // defined(TEST_FLAGS_LINKING)
