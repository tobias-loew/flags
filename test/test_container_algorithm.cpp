
// Copyright 2024, 2025 Tobias Loew.
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
BOOST_FLAGS_ENABLE(relops_builtin_enum)


enum class relops_delete_enum : unsigned int {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable relops_delete_enum
BOOST_FLAGS_ENABLE(relops_delete_enum)

BOOST_FLAGS_DELETE_REL(relops_delete_enum)


enum class relops_std_less_enum : unsigned int {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable relops_delete_enum
BOOST_FLAGS_ENABLE(relops_std_less_enum)


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
    test_std_less();
    return boost::report_errors();
}

#if defined(TEST_FLAGS_LINKING)
} // namespace TEST_NAMESPACE
#endif // defined(TEST_FLAGS_LINKING)
