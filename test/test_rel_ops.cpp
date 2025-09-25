
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

// enable relops_std_less_enum
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

#if BOOST_FLAGS_HAS_THREE_WAY_COMPARISON
                                    {
#define OP <=>
                                        auto bf = v1 OP v2;
                                        auto bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif // BOOST_FLAGS_HAS_THREE_WAY_COMPARISON
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
									
									// something different than E
									auto w2 = relops_builtin_enum::bit_0;
                                    (void)w2;   // prevent unused warning

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

#if BOOST_FLAGS_HAS_THREE_WAY_COMPARISON
                                    {
#define OP <=>
                                        auto bf = v1 OP v2;
                                        auto bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#else // BOOST_FLAGS_HAS_THREE_WAY_COMPARISON

#error Spaceship has not yet landed or is destroyed be builtin operator! Cannot test for fail-on-delete.

#endif // BOOST_FLAGS_HAS_THREE_WAY_COMPARISON


#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_SPACESHIP)






#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LHS_LT)
                                    {
#define OP <
                                        bool bf = v1 OP to_underlying(v2);
                                        BOOST_TEST((bf == bf));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LHS_LT)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LHS_LE)
                                    {
#define OP <=
                                        bool bf = v1 OP to_underlying(v2);
                                        BOOST_TEST((bf == bf));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LHS_LE)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LHS_GT)
                                    {
#define OP >
                                        bool bf = v1 OP to_underlying(v2);
                                        BOOST_TEST((bf == bf));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LHS_GT)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LHS_GE)
                                    {
#define OP >=
                                        bool bf = v1 OP to_underlying(v2);
                                        BOOST_TEST((bf == bf));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LHS_GE)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LHS_SPACESHIP)

#if BOOST_FLAGS_HAS_THREE_WAY_COMPARISON
                                    {
#define OP <=>
                                        bool bf = v1 OP to_underlying(v2);
                                        BOOST_TEST((bf == bf));
#undef OP
                                    }
#else // BOOST_FLAGS_HAS_THREE_WAY_COMPARISON

#error Spaceship has not yet landed or is destroyed be builtin operator! Cannot test for fail-on-delete.

#endif // BOOST_FLAGS_HAS_THREE_WAY_COMPARISON


#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_LHS_SPACESHIP)







#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_RHS_LT)
                                    {
#define OP <
                                        bool bf = to_underlying(v1) OP v2;
                                        BOOST_TEST((bf == bf));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_RHS_LT)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_RHS_LE)
                                    {
#define OP <=
                                        bool bf = to_underlying(v1) OP v2;
                                        BOOST_TEST((bf == bf));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_RHS_LE)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_RHS_GT)
                                    {
#define OP >
                                        bool bf = to_underlying(v1) OP v2;
                                        BOOST_TEST((bf == bf));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_RHS_GT)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_RHS_GE)
                                    {
#define OP >=
                                        bool bf = to_underlying(v1) OP v2;
                                        BOOST_TEST((bf == bf));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_RHS_GE)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_RHS_SPACESHIP)

#if BOOST_FLAGS_HAS_THREE_WAY_COMPARISON
                                    {
#define OP <=>
                                        bool bf = to_underlying(v1) OP v2;
                                        BOOST_TEST((bf == bf));
#undef OP
                                    }
#else // BOOST_FLAGS_HAS_THREE_WAY_COMPARISON

#error Spaceship has not yet landed or is destroyed be builtin operator! Cannot test for fail-on-delete.

#endif // BOOST_FLAGS_HAS_THREE_WAY_COMPARISON


#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_RHS_SPACESHIP)





#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_MIXED_LT)
                                    {
#define OP <
                                        bool bf = v1 OP w2;
                                        bool bu = to_underlying(v1) OP to_underlying(w2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_MIXED_LT)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_MIXED_LE)
                                    {
#define OP <=
                                        bool bf = v1 OP w2;
                                        bool bu = to_underlying(v1) OP to_underlying(w2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_MIXED_LE)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_MIXED_GT)
                                    {
#define OP >
                                        bool bf = v1 OP w2;
                                        bool bu = to_underlying(v1) OP to_underlying(w2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_MIXED_GT)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_MIXED_GE)
                                    {
#define OP >=
                                        bool bf = v1 OP w2;
                                        bool bu = to_underlying(v1) OP to_underlying(w2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_MIXED_GE)

#if defined(TEST_COMPILE_FAIL_RELOPS_DELETE_MIXED_SPACESHIP)

#if BOOST_FLAGS_HAS_THREE_WAY_COMPARISON
                                    {
#define OP <=>
                                        auto bf = v1 OP w2;
                                        auto bu = to_underlying(v1) OP to_underlying(w2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#else // BOOST_FLAGS_HAS_THREE_WAY_COMPARISON

#error Spaceship has not yet landed or is destroyed be builtin operator! Cannot test for fail-on-delete.

#endif // BOOST_FLAGS_HAS_THREE_WAY_COMPARISON


#endif // defined(TEST_COMPILE_FAIL_RELOPS_DELETE_MIXED_SPACESHIP)



                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


#if BOOST_FLAGS_HAS_PARTIAL_ORDERING

// calculates bit inclusion bit for bit
// deliberatly NOT using binary operators
template<typename T>
std::partial_ordering check_bit_incusion(T l, T r) {

    // start with equivalent
    std::partial_ordering result = std::partial_ordering::equivalent;

    for (int i = 0; i < static_cast<int>(sizeof(T) * 8); ++i) {
        auto val = 1 << i;
        bool sl = l & val;
        bool sr = r & val;

        if (sl == sr) {
            // no change
        }
        else if (sr) {
            // less
            if (result == std::partial_ordering::greater) {
                // incompatible with previous result
                return std::partial_ordering::unordered;
            }
            // set result to less
            result = std::partial_ordering::less;
        }
        else if (sl) {
            // greater
            if (result == std::partial_ordering::less) {
                // incompatible with previous result
                return std::partial_ordering::unordered;
            }
            // set result to greater
            result = std::partial_ordering::greater;
        }
    }

    return result;
}


#endif // BOOST_FLAGS_HAS_PARTIAL_ORDERING


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

#if BOOST_FLAGS_HAS_THREE_WAY_COMPARISON
                                    {
#define OP <=>
                                        auto bf = v1 OP v2;
                                        auto bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif // BOOST_FLAGS_HAS_THREE_WAY_COMPARISON
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
    report_config();
    test_builtin();
    test_delete();
    test_std_less();
    return boost::report_errors();
}

#if defined(TEST_FLAGS_LINKING)
} // namespace TEST_NAMESPACE
#endif // defined(TEST_FLAGS_LINKING)
