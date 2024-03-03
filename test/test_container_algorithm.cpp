// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#define TEST_NAMESPACE test_container_algorithm
#include "include_test.hpp"

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>
#include <array>
#include <algorithm>
#include <set>
#include <vector>

#include "include_test_post.hpp"

#if defined(TEST_FLAGS_LINKING)
namespace TEST_NAMESPACE {
#endif // defined(TEST_FLAGS_LINKING)

enum class relops_builtin_enum : unsigned int {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable relops_builtin_enum
TEST_FLAGS_LINKING_PREAMBLE
template<> struct boost_flags_enable<TEST_FLAGS_LINKING_NAMESPACE relops_builtin_enum> : std::true_type {};
TEST_FLAGS_LINKING_EPILOGUE



enum class relops_delete_enum : unsigned int {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable relops_delete_enum
TEST_FLAGS_LINKING_PREAMBLE
template<> struct boost_flags_enable<TEST_FLAGS_LINKING_NAMESPACE relops_delete_enum> : std::true_type {};
TEST_FLAGS_LINKING_EPILOGUE

BOOST_FLAGS_REL_OPS_DELETE(relops_delete_enum)


#if defined(_MSC_VER) && _MSC_VER <= 1900

// BOOST_FLAGS_REL_OPS_PARTIAL_ORDER not supported for msvc <= v140

#else // defined(_MSC_VER) && _MSC_VER <= 1900

enum class relops_partial_order_enum : unsigned int {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable relops_delete_enum
TEST_FLAGS_LINKING_PREAMBLE
template<> struct boost_flags_enable<TEST_FLAGS_LINKING_NAMESPACE relops_partial_order_enum> : std::true_type {};
TEST_FLAGS_LINKING_EPILOGUE

BOOST_FLAGS_REL_OPS_PARTIAL_ORDER(relops_partial_order_enum)

#endif // defined(_MSC_VER) && _MSC_VER <= 1900


enum class relops_std_less_enum : unsigned int {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable relops_delete_enum
TEST_FLAGS_LINKING_PREAMBLE
template<> struct boost_flags_enable<TEST_FLAGS_LINKING_NAMESPACE relops_std_less_enum> : std::true_type {};
TEST_FLAGS_LINKING_EPILOGUE


#if defined(TEST_FLAGS_LINKING)

} // namespace TEST_NAMESPACE

BOOST_FLAGS_SPECIALIZE_STD_LESS(TEST_NAMESPACE::relops_std_less_enum)

namespace TEST_NAMESPACE {

#else  // defined(TEST_FLAGS_LINKING)

BOOST_FLAGS_SPECIALIZE_STD_LESS(relops_std_less_enum)

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

// turning around
template<typename T>
auto make_on_off(T v)
#if defined(TEST_NO_CXX14_DECLTYPE_AUTO)
-> typename std::array<T, 2>
#endif // defined(TEST_NO_CXX14_DECLTYPE_AUTO)
{
    return std::array<T, 2>{{v, T{}}};  // use double braces to prevent C++11 clang warning "suggest braces around initialization of subobject"
}

template<typename T>
void check_container_order_lt(T const& cont) {
    bool first = true;
    typename T::value_type prev{};
    for (auto v : cont) {
        if (first) {
            prev = v;
            first = false;
        }
        else {
            BOOST_TEST(prev < v);
        }
    }
}

template<typename T>
void check_container_order_le(T const& cont) {
    bool first = true;
    typename T::value_type prev{};
    for (auto v : cont) {
        if (first) {
            prev = v;
            first = false;
        }
        else {
            BOOST_TEST(prev <= v);
        }
    }
}


void test_builtin() {
    using E = relops_builtin_enum;

    std::set<E> s;
    std::multiset<E> m;
    std::vector<E> va;
    std::vector<E> vr;

    for (auto a1 : make_off_on(E::bit_0)) {
        for (auto b1 : make_on_off(E::bit_1)) {
            for (auto c1 : make_off_on(E::bit_2)) {
                for (auto d1 : make_off_on(E::bit_3)) {
                    auto v1 = a1 | b1 | c1 | d1;
                    s.insert(v1);
                    m.insert(v1);
                    va.push_back(v1);
                    vr.push_back(v1);
                }
            }
        }
    }

    // do it twice
    for (auto a1 : make_off_on(E::bit_0)) {
        for (auto b1 : make_on_off(E::bit_1)) {
            for (auto c1 : make_off_on(E::bit_2)) {
                for (auto d1 : make_off_on(E::bit_3)) {
                    auto v1 = a1 | b1 | c1 | d1;
                    s.insert(v1);
                    m.insert(v1);
                    va.push_back(v1);
                    vr.push_back(v1);
                }
            }
        }
    }

    BOOST_TEST((s.size() == 16));
    BOOST_TEST((s.count(E::bit_0) == 1));
    BOOST_TEST((s.count(E::bit_3) == 1));
    BOOST_TEST((s.count(E::bit_2 | E::bit_0) == 1));
    BOOST_TEST((s.count(E::bit_2 | E::bit_1) == 1));
    check_container_order_lt(s);

    BOOST_TEST((m.size() == 32));
    BOOST_TEST((m.count(E::bit_0) == 2));
    BOOST_TEST((m.count(E::bit_3) == 2));
    BOOST_TEST((m.count(E::bit_2 | E::bit_0) == 2));
    BOOST_TEST((m.count(E::bit_2 | E::bit_1) == 2));
    check_container_order_le(m);

    std::sort(va.begin(), va.end());
    check_container_order_le(va);

#if defined(__cpp_lib_ranges) && (__cpp_lib_ranges >= 201911L)
    std::ranges::sort(vr);
    check_container_order_le(vr);
#endif
}



void test_delete() {
    using E = relops_delete_enum;

    std::set<E> s;
    std::multiset<E> m;

#if defined(TEST_COMPILE_FAIL_CONTAINER_DELETE_LT)
#if defined(TEST_COMPILE_FAIL_SET)
    s.insert(E::bit_0);
    s.insert(E::bit_1);
#endif
#if defined(TEST_COMPILE_FAIL_MULTISET)
    m.insert(E::bit_0);
    m.insert(E::bit_1);
#endif
#endif
}

void test_partial_order() {
#if defined(_MSC_VER) && _MSC_VER <= 1900

    // BOOST_FLAGS_REL_OPS_PARTIAL_ORDER not supported for msvc <= v140

#else // defined(_MSC_VER) && _MSC_VER <= 1900

    using E = relops_partial_order_enum;

    std::set<E> s;
    std::multiset<E> m;

    // partial order relational ops -> bitset inclusion

    // a totally ordered subset
    auto a1 = E{};
    auto b1 = a1 | E::bit_0;
    auto c1 = b1 | E::bit_1;
    auto d1 = c1 | E::bit_2;
    auto e1 = d1 | E::bit_3;

    {
        s.insert(a1);
        s.insert(d1);
        s.insert(e1);
        s.insert(c1);

        m.insert(a1);
        m.insert(d1);
        m.insert(e1);
        m.insert(c1);
    }
    {
        s.insert(a1);
        s.insert(d1);
        s.insert(e1);
        s.insert(c1);

        m.insert(a1);
        m.insert(d1);
        m.insert(e1);
        m.insert(c1);
    }

    BOOST_TEST((s.size() == 4));
    BOOST_TEST((s.count(a1) == 1));
    BOOST_TEST((s.count(d1) == 1));
    BOOST_TEST((s.count(b1) == 0));
    check_container_order_lt(s);

    BOOST_TEST((m.size() == 8));
    BOOST_TEST((m.count(a1) == 2));
    BOOST_TEST((m.count(d1) == 2));
    BOOST_TEST((m.count(b1) == 0));
    check_container_order_le(m);


#endif // defined(_MSC_VER) && _MSC_VER <= 1900
}


void test_std_less() {
    using E = relops_std_less_enum;

    // built-in relational ops -> compare underlying

    std::set<E> s;
    std::multiset<E> m;
    std::vector<E> va;
    std::vector<E> vr;

    for (auto a1 : make_off_on(E::bit_0)) {
        for (auto b1 : make_on_off(E::bit_1)) {
            for (auto c1 : make_off_on(E::bit_2)) {
                for (auto d1 : make_off_on(E::bit_3)) {
                    auto v1 = a1 | b1 | c1 | d1;
                    s.insert(v1);
                    m.insert(v1);
                    va.push_back(v1);
                    vr.push_back(v1);
                }
            }
        }
    }

    // do it twice
    for (auto a1 : make_off_on(E::bit_0)) {
        for (auto b1 : make_on_off(E::bit_1)) {
            for (auto c1 : make_off_on(E::bit_2)) {
                for (auto d1 : make_off_on(E::bit_3)) {
                    auto v1 = a1 | b1 | c1 | d1;
                    s.insert(v1);
                    m.insert(v1);
                    va.push_back(v1);
                    vr.push_back(v1);
                }
            }
        }
    }

    BOOST_TEST((s.size() == 16));
    BOOST_TEST((s.count(E::bit_0) == 1));
    BOOST_TEST((s.count(E::bit_3) == 1));
    BOOST_TEST((s.count(E::bit_2 | E::bit_0) == 1));
    BOOST_TEST((s.count(E::bit_2 | E::bit_1) == 1));
    check_container_order_lt(s);

    BOOST_TEST((m.size() == 32));
    BOOST_TEST((m.count(E::bit_0) == 2));
    BOOST_TEST((m.count(E::bit_3) == 2));
    BOOST_TEST((m.count(E::bit_2 | E::bit_0) == 2));
    BOOST_TEST((m.count(E::bit_2 | E::bit_1) == 2));
    check_container_order_le(m);

    std::sort(va.begin(), va.end());
    check_container_order_le(va);

#if defined(__cpp_lib_ranges) && (__cpp_lib_ranges >= 201911L)
    std::ranges::sort(vr);
    check_container_order_le(vr);
#endif
}


int main() {
    report_config();
    test_builtin();
    test_delete();
    test_partial_order();
    test_std_less();
    return boost::report_errors();
}

#if defined(TEST_FLAGS_LINKING)
} // namespace TEST_NAMESPACE
#endif // defined(TEST_FLAGS_LINKING)
