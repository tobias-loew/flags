// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#define TEST_NAMESPACE test_rel_ops
#include "include_test.hpp"

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>
#include <array>

#if defined(TEST_FLAGS_LINKING)
namespace TEST_NAMESPACE {
#endif // defined(TEST_FLAGS_LINKING)

enum class relops_builtin_enum {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable relops_builtin_enum
TEST_FLAGS_LINKING_PREAMBLE
template<> struct boost_flags_enable<TEST_FLAGS_LINKING_NAMESPACE relops_builtin_enum> : std::true_type {};
TEST_FLAGS_LINKING_EPILOGUE



enum class relops_delete_enum {
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


enum class relops_partial_order_enum {
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


enum class relops_std_less_enum {
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
-> typename std::underlying_type<E>::type
#endif // defined(TEST_NO_CXX14_DECLTYPE_AUTO)
{
    return std::array<T, 2>{{T{}, v}};  // use double braces to prevent C++11 clang warning "suggest braces around initialization of subobject"
}

void test_builtin() {
    using E = relops_builtin_enum;

    // built-in relational ops -> compare underlying

    for (auto a1 : make_off_on(E::bit_0)) {
        for (auto b1 : make_off_on(E::bit_1)) {
            for (auto c1 : make_off_on(E::bit_2)) {
                for (auto d1 : make_off_on(E::bit_3)) {

                    for (auto a2 : make_off_on(E::bit_0)) {
                        for (auto b2 : make_off_on(E::bit_1)) {
                            for (auto c2 : make_off_on(E::bit_2)) {
                                for (auto d2 : make_off_on(E::bit_3)) {

                                    auto v1 = a1 | b1 | c1 | d1;
                                    auto v2 = a2 | b2 | c2 | d2;

                                    {
#define OP <
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }

                                    {
#define OP <=
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }

                                    {
#define OP >
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }

                                    {
#define OP >=
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }

#if !(BOOST_FLAGS_EMULATE_THREE_WAY_COMPARISON)
                                    {
#define OP <=>
                                        auto bf = v1 OP v2;
                                        auto bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif // !(BOOST_FLAGS_EMULATE_THREE_WAY_COMPARISON)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}



void test_delete() {
    using E = relops_delete_enum;

    // relational ops deleted

    for (auto a1 : make_off_on(E::bit_0)) {
        for (auto b1 : make_off_on(E::bit_1)) {
            for (auto c1 : make_off_on(E::bit_2)) {
                for (auto d1 : make_off_on(E::bit_3)) {

                    for (auto a2 : make_off_on(E::bit_0)) {
                        for (auto b2 : make_off_on(E::bit_1)) {
                            for (auto c2 : make_off_on(E::bit_2)) {
                                for (auto d2 : make_off_on(E::bit_3)) {

                                    auto v1 = a1 | b1 | c1 | d1;
                                    auto v2 = a2 | b2 | c2 | d2;
                                    (void)v1;   // prevent unused warning
                                    (void)v2;   // prevent unused warning

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LT)
                                    {
#define OP <
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LT)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LE)
                                    {
#define OP <=
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LE)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_GT)
                                    {
#define OP >
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_GT)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_GE)
                                    {
#define OP >=
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_GE)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_SPACESHIP)

#if !(BOOST_FLAGS_EMULATE_THREE_WAY_COMPARISON)
                                    {
#define OP <=>
                                        auto bf = v1 OP v2;
                                        auto bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#else // !(BOOST_FLAGS_EMULATE_THREE_WAY_COMPARISON)

#error Spaceship has not yet landed or is destroyed be builtin operator! Cannot test for fail-on-delete.

#endif // !(BOOST_FLAGS_EMULATE_THREE_WAY_COMPARISON)


#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_SPACESHIP)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// calculates bit inclusion bit for bit
// deliberatly NOT using binary operators
template<typename T>
boost::flags::partial_ordering check_bit_incusion(T l, T r) {

    // start with equivalent
    boost::flags::partial_ordering result = boost::flags::partial_ordering::equivalent;

    for (int i = 0; i < static_cast<int>(sizeof(T) * 8); ++i) {
        auto val = 1 << i;
        bool sl = l & val;
        bool sr = r & val;

        if (sl == sr) {
            // no change
        }
        else if (sr) {
            // less
            if (result == boost::flags::partial_ordering::greater) {
                // incompatible with previous result
                return boost::flags::partial_ordering::unordered;
            }
            // set result to less
            result = boost::flags::partial_ordering::less;
        }
        else if (sl) {
            // greater
            if (result == boost::flags::partial_ordering::less) {
                // incompatible with previous result
                return boost::flags::partial_ordering::unordered;
            }
            // set result to greater
            result = boost::flags::partial_ordering::greater;
        }
    }

    return result;
}


void test_partial_order() {
    using E = relops_partial_order_enum;

    // partial order relational ops -> bitset inclusion

    for (auto a1 : make_off_on(E::bit_0)) {
        for (auto b1 : make_off_on(E::bit_1)) {
            for (auto c1 : make_off_on(E::bit_2)) {
                for (auto d1 : make_off_on(E::bit_3)) {

                    for (auto a2 : make_off_on(E::bit_0)) {
                        for (auto b2 : make_off_on(E::bit_1)) {
                            for (auto c2 : make_off_on(E::bit_2)) {
                                for (auto d2 : make_off_on(E::bit_3)) {

                                    auto v1 = a1 | b1 | c1 | d1;
                                    auto v2 = a2 | b2 | c2 | d2;

                                    {
#define OP <
                                        bool bf = v1 OP v2;
                                        bool bu = check_bit_incusion(to_underlying(v1), to_underlying(v2)) OP 0;
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }

                                    {
#define OP <=
                                        bool bf = v1 OP v2;
                                        bool bu = check_bit_incusion(to_underlying(v1), to_underlying(v2)) OP 0;
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }

                                    {
#define OP >
                                        bool bf = v1 OP v2;
                                        bool bu = check_bit_incusion(to_underlying(v1), to_underlying(v2)) OP 0;
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }

                                    {
#define OP >=
                                        bool bf = v1 OP v2;
                                        bool bu = check_bit_incusion(to_underlying(v1), to_underlying(v2)) OP 0;
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }

#if !(BOOST_FLAGS_EMULATE_THREE_WAY_COMPARISON)
                                    {
#define OP <=>
                                        auto bf = v1 OP v2;
                                        auto bu = check_bit_incusion(to_underlying(v1), to_underlying(v2));
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif // !(BOOST_FLAGS_EMULATE_THREE_WAY_COMPARISON)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


void test_std_less() {
    using E = relops_std_less_enum;

    // built-in relational ops -> compare underlying

    for (auto a1 : make_off_on(E::bit_0)) {
        for (auto b1 : make_off_on(E::bit_1)) {
            for (auto c1 : make_off_on(E::bit_2)) {
                for (auto d1 : make_off_on(E::bit_3)) {

                    for (auto a2 : make_off_on(E::bit_0)) {
                        for (auto b2 : make_off_on(E::bit_1)) {
                            for (auto c2 : make_off_on(E::bit_2)) {
                                for (auto d2 : make_off_on(E::bit_3)) {

                                    auto v1 = a1 | b1 | c1 | d1;
                                    auto v2 = a2 | b2 | c2 | d2;

                                    {
#define OP <
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }

                                    {
#define OP <=
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }

                                    {
#define OP >
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }

                                    {
#define OP >=
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }

#if !(BOOST_FLAGS_EMULATE_THREE_WAY_COMPARISON)
                                    {
#define OP <=>
                                        auto bf = v1 OP v2;
                                        auto bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif // !(BOOST_FLAGS_EMULATE_THREE_WAY_COMPARISON)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


int main() {
    test_builtin();
    test_delete();
    test_partial_order();
    test_std_less();
    return boost::report_errors();
}

#if defined(TEST_FLAGS_LINKING)
} // namespace TEST_NAMESPACE
#endif // defined(TEST_FLAGS_LINKING)
