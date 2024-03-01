// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>
#include <array>

enum class relops_builtin_enum {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable relops_builtin_enum
template<> struct boost::flags::enable<relops_builtin_enum> : std::true_type {};



enum class relops_delete_enum {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable relops_delete_enum
template<> struct boost::flags::enable<relops_delete_enum> : std::true_type {};

BOOST_FLAGS_REL_OPS_DELETE(relops_delete_enum)



enum class relops_partial_order_enum {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable relops_delete_enum
template<> struct boost::flags::enable<relops_partial_order_enum> : std::true_type {};

BOOST_FLAGS_REL_OPS_PARTIAL_ORDER(relops_partial_order_enum)



enum class relops_std_less_enum {
    bit_0 = boost::flags::nth_bit(0), // == 0x01
    bit_1 = boost::flags::nth_bit(1), // == 0x02
    bit_2 = boost::flags::nth_bit(2), // == 0x04
    bit_3 = boost::flags::nth_bit(3), // == 0x08
};

// enable relops_delete_enum
template<> struct boost::flags::enable<relops_std_less_enum> : std::true_type {};

BOOST_FLAGS_SPECIALIZE_STD_LESS(relops_std_less_enum)


// helpers
template<typename E>
constexpr auto to_underlying(E value)
#ifdef BOOST_FLAGS_NO_CXX14_DECLTYPE_AUTO
-> typename std::underlying_type<E>::type
#endif // BOOST_FLAGS_NO_CXX14_DECLTYPE_AUTO
{
    return static_cast<typename std::underlying_type<E>::type>(value);
}

template<typename T>
auto make_off_on(T v) 
#ifdef BOOST_FLAGS_NO_CXX14_DECLTYPE_AUTO
-> typename std::array<T, 2>
#endif // BOOST_FLAGS_NO_CXX14_DECLTYPE_AUTO
{
    return std::array<T, 2>{T{}, v};
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

                                    {
#define OP <=>
                                        auto bf = v1 OP v2;
                                        auto bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
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

#ifdef TEST_COMPILE_FAIL_RELOPS_DELETE_LT
                                    {
#define OP <
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif

#ifdef TEST_COMPILE_FAIL_RELOPS_DELETE_LE
                                    {
#define OP <=
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif

#ifdef TEST_COMPILE_FAIL_RELOPS_DELETE_GT
                                    {
#define OP >
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif

#ifdef TEST_COMPILE_FAIL_RELOPS_DELETE_GE
                                    {
#define OP >=
                                        bool bf = v1 OP v2;
                                        bool bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif

#ifdef TEST_COMPILE_FAIL_RELOPS_DELETE_SPACESHIP
                                    {
#define OP <=>
                                        auto bf = v1 OP v2;
                                        auto bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
#endif
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

                                    {
#define OP <=>
                                        auto bf = v1 OP v2;
                                        auto bu = check_bit_incusion(to_underlying(v1), to_underlying(v2));
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
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

                                    {
#define OP <=>
                                        auto bf = v1 OP v2;
                                        auto bu = to_underlying(v1) OP to_underlying(v2);
                                        BOOST_TEST((bf == bu));
#undef OP
                                    }
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
