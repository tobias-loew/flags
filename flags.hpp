//////////////////////////////////////////////////////////////////
//
//  Boost.Flags
//  non-intrusive bitwise operators for flag-like enumerations
//
//  Copyright Tobias Loew 2024. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see https://github.com/tobias-loew/flags
//


#ifndef BOOST_FLAGS_HPP_INCLUDED
#define BOOST_FLAGS_HPP_INCLUDED

#include <type_traits>
#include <boost/config.hpp>
#include <boost/core/underlying_type.hpp>

/////////////////////////////////////////////////////////////////
//
// purpose: 
// 
// - provides type-safe bitwise operations for flag-like 
//   scoped / unscoped enumerations
// 
// - turns logical errors undetectable by th compiler
//   into type errors
// 
// = is non- ntrusive, only requires a template specialization
//
// Example:
/*

    enum class pizza_toppings {
        tomato      = boost::flags::nth_bit(0), // == 0x01
        cheese      = boost::flags::nth_bit(1), // == 0x02
        salami      = boost::flags::nth_bit(2), // == 0x04
        olives      = boost::flags::nth_bit(3), // == 0x08
    };
    // enable Boost.Flags for pizza_toppings
    template<> struct boost::flags::enable<pizza_toppings> : std::true_type {};

    enum class ice_cream_flavours {
        vanilla     = boost::flags::nth_bit(0), // == 0x01
        chocolate   = boost::flags::nth_bit(1), // == 0x02
        strawberry  = boost::flags::nth_bit(2), // == 0x04
    };
    // enable Boost.Flags for ice_cream_flavours
    template<> struct boost::flags::enable<ice_cream_flavours> : std::true_type {};

    void order_pizza(pizza_toppings toppings) { ... }
    void order_ice_cream(ice_cream_flavours flavours) { ... }

    int main() {
        pizza_toppings toppings = pizza_toppings::tomato | pizza_toppings::cheese; // a decent start
        toppings |= pizza_toppings::salami | pizza_toppings::olives;    // even better
        order_pizza(toppings);                              // order main course
        order_pizza(toppings & ~pizza_toppings::salami);    // order a vegetarian pizza
        order_ice_cream(ice_cream_flavours::vanilla);       // order desert

        // error: negative mask is not a pizza topping
        order_pizza(~pizza_toppings::olives);

        // error: mixing different enumerations
        toppings |= ice_cream_flavours::strawberry;

        // error: called with wrong enumeration
        order_ice_cream(toppings);
    }



    // another example

    enum class flags_t {
        a = 0x1,
        b = 0x2,
        c = 0x4,
    };

    enum class flags2_t {
        e = boost::flags::nth_bit(0), // == 0x1
        f = boost::flags::nth_bit(1), // == 0x2
        g = boost::flags::nth_bit(2), // == 0x4
    };

    template<>
    struct boost::flags::enable<flags_t> : std::true_type {};

    template<>
    struct boost::flags::enable<flags2_t> : std::true_type {};

    void foo() {
        auto ab = flags_t::a | flags_t::b;  // type: flags_t
        auto bc = flags_t::b | flags_t::c;  // type: flags_t
        auto ab_and_bc = ab & bc;           // type: flags_t
        auto not_a = ~flags_t::a;           // type: boost::flags::complement<flags_t>
        auto not_not_a = ~not_a;            // type: flags_t

        auto not_a_and_ab = not_a & ab;     // type: flags_t
        auto not_a_or_ab = not_a | ab;      // type: boost::flags::complement<flags_t>

        // auto ae = flags_t::a | flags2_t::e;  // compilation error (different enumerations)


        // test with boolean result
        // using pseudo operator BF_AND
        // BF_AND(x,y) -> bool
        // BF_AND(x,y) ::= (x != 0) && (y != 0) 
        if (ab_and_bc BF_AND not_a) {
            // ...
        }
    }
*/


namespace boost {
    namespace flags {
        namespace impl {

#ifdef __cpp_lib_is_scoped_enum
            template<typename E>
            using is_scoped_enum = std::is_scoped_enum<E>;
#else
            template<typename E>
            struct is_scoped_enum : std::bool_constant < requires
            {
                requires std::is_enum_v<E>;
                requires !std::is_convertible_v<E, std::underlying_type_t<E>>;
            } >
            {};
#endif
        }

        // non-intrusive opt-in to operations of boost::flags
        // specialize
        // `template<> struct boost::flags::enable<my_enum> : std::true_type {};`
        // to enable operations for scoped or unscoped enums
        template<typename E>
        struct enable : std::false_type {};


        struct option_disable_complement {};

        // indicates invalid/incompatible types for operation
        struct error_tag {};

        // explicitly disable error_tag
        template<>
        struct enable<error_tag> : std::false_type {};




        // class-template to indicate complemented flags
        template<typename E>
        struct complement {
            E value;
        };



        // get enum-type from E
// strip off `complement`-templates
        template<typename E>
        struct enum_type {
            using type = E;
        };

        template<typename E>
        struct enum_type<complement<E>> :enum_type<E> {};

        template<typename E>
        using enum_type_t = typename enum_type<E>::type;


        // test if E is enabled: either a flags-enum or a negation (detects double-negations)
        template<typename E>
        struct is_enabled;

        template<typename T>
        concept IsComplementDisabled =
            std::is_base_of_v<option_disable_complement, enable<enum_type_t<T>>>;

        // test if E is a flags-enum:
        // detects double-negation
        // returns false_type for non-enabled enums (SFINAE friendly)
        template<typename E>
        struct is_flags;

        // test for complement:
        // detects double-negation
        // returns false_type for non-enabled enums (SFINAE friendly)
        template<typename E>
        struct is_complement;


        template<typename E>
        struct is_enabled :enable<enum_type_t<E>> {};


        // test if E is enabled
        template<typename E>
        struct is_flags : is_enabled<E> {};

        template<typename E>
        struct is_flags<complement<E>> :
            std::conjunction<
            std::negation<is_flags<E>>,
            is_enabled<E>                       // ensure it's enabled
            > {};


        // test for complement (detects double-negation)
        template<typename E>
        struct is_complement :
            std::conjunction<
            std::negation<is_flags<E>>,
            is_enabled<E>                       // ensure it's enabled
            > {};


        template<typename E>
        concept IsFlags = is_flags<E>::value;

        template<typename E>
        concept IsComplement = is_complement<E>::value;

        template<typename E>
        concept IsEnabled = is_enabled<E>::value;


        template<typename T1, typename T2>
        concept IsCompatible = std::is_same<enum_type_t<T1>, enum_type_t<T2>>::value;

        template<typename T1, typename T2>
        concept IsCompatibleFlags = IsCompatible<T1, T2> &&
            IsFlags<T1> && IsFlags<T2>;

        template<typename T1, typename T2>
        concept IsCompatibleComplement = IsCompatible<T1, T2>&&
            IsComplement<T1>&& IsComplement<T2>;

        template<typename T1, typename T2>
        concept IsCompatibleFlagsOrComplement = IsCompatible<T1, T2>&&
            ((IsFlags<T1>&& IsFlags<T2>) || (IsComplement<T1>&& IsComplement<T2>));


        namespace impl {

            // NULL constant tag
            struct null_tag {};

            // for pseudo operator BF_AND
            struct pseudo_and_op_tag {};

            template<typename T>
            struct pseudo_and_op_intermediate_t {
                T value;
            };

            template<typename T>
            struct is_pseudo_and_op_intermediate : 
                std::false_type {};

            template<typename E>
            struct is_pseudo_and_op_intermediate<pseudo_and_op_intermediate_t<E>> : 
                std::true_type {};

            template<typename E>
            struct is_pseudo_and_op_type :
                    std::disjunction<
                    std::is_same<std::remove_cvref_t<E>, pseudo_and_op_tag>,
                    is_pseudo_and_op_intermediate<std::remove_cvref_t<E>>
                    > {};

            template<typename T>
            static constexpr bool is_pseudo_and_op_type_v = is_pseudo_and_op_type<T>::value;

            // calculate whether result Op(T1) is in the original or the complemented boolean algebra
            // always returns the canonical form (either the enum or its negation, never a double negated enum)
            template<typename T1, template<typename> class UnOp>
            struct unary_operation_result {
                using E1 = enum_type_t<T1>;

                using type = typename std::conditional<
                    enable<E1>::value,        // check if undelying enum is enabled
                    typename std::conditional_t<
                        IsComplementDisabled<T1>,
                        T1,
                        typename std::conditional_t<
                            UnOp<is_complement<T1>>::value,
                            complement<E1>,
                            E1
                        >
                    >,
                    error_tag
                >::type;
            };



            // calculate whether result Op(T1, T2) is in the original or the complement boolean algebra
            // always returns the canonical form (either the enum or its negation, never a double negated enum)
            template<typename T1, typename T2, template<typename...> class BinOp>
            struct binary_operation_result {
                using E1 = enum_type_t<T1>;
                using E2 = enum_type_t<T2>;

                // static-asserting on "if E1 or E2 is enabled then both are equal" in here would result in poor
                // error diagnostic. The error would point here, and not to the call site. 
                // We use the compatibility_check template instead (see below).
                //static_assert(
                //    (!(enable<E1>::value || enable<E2>::value) || std::is_same_v<E1, E2>)
                //    );

                using type = typename std::conditional<
                    std::is_same<E1, E2>::value&& enable<E1>::value,        // check undelying enums are the same and enabled
                    typename std::conditional<
                    BinOp<is_complement<T1>, is_complement<T2>>::value,
                    complement<E1>,
                    E1
                    >::type,
                    error_tag
                >::type;
            };


            // checking: if E1 or E2 is enabled then both are equal
            // enforces a hard compilation error
            template<typename T1, typename T2>
            struct compatibility_check {
                using E1 = enum_type_t<T1>;
                using E2 = enum_type_t<T2>;
                static_assert(
                    (!(enable<E1>::value || enable<E2>::value) 
                        || std::is_same_v<E1, E2>
                        || is_pseudo_and_op_type_v<E1>
                        || is_pseudo_and_op_type_v<E2>
                        )
                    );
                using type = void;
            };

            template<typename T1, typename T2>
            using compatibility_check_t = compatibility_check<T1, T2>::type;


            // the standard doesn't provide a not_equal trait, so lets use our own
            template<typename B1, typename B2>
            struct not_equal : std::bool_constant<bool(B1::value) != bool(B2::value)> {};


            template<typename T>
            constexpr auto get_underlying(T value) {
                using underlying = typename boost::underlying_type<T>::type;
                return static_cast<underlying>(value);
            }

            template<typename T>
            constexpr auto get_underlying(complement<T> value) {
                return get_underlying(value.value);
            }


            // normalize `complement`s to zero or one

            // test for outer `double complement`
            template<typename E>
            struct is_double_outer_complement : std::false_type {};
                
            template<typename E>
            struct is_double_outer_complement<complement<complement<E>>> :
                is_enabled<E>                       // ensure it's enabled
            {};


            template<typename E>
            concept IsDoubleOuterComplement = is_double_outer_complement<std::remove_cvref_t<E>>::value;


            template<typename T>
            constexpr auto&& get_normalized(T&& value) {
                return std::forward<T>(value);
            }

            template<typename T>
            requires IsDoubleOuterComplement<T>
            constexpr decltype(auto) get_normalized(T&& value) {
                return get_normalized(std::forward<T>(value).value.value);
            }

        } // namespace impl


        // concept checking if arguments is enabled
        template<typename T1, template<typename> class UnOp>
        concept UnaryOperationEnabled = is_enabled<typename impl::unary_operation_result<T1, UnOp>::type>::value;

        // concept checking both arguments are compatible and enabled
        template<typename T1, typename T2, template<typename...> class BinOp>
        concept BinaryOperationEnabled = is_enabled<typename impl::binary_operation_result<T1, T2, BinOp>::type>::value;

        // concept checking both arguments are compatible, enabled and the result is assignable to T1
        template<typename T1, typename T2, template<typename...> class BinOp>
        concept BinaryAssignmentEnabled = BinaryOperationEnabled<T1, T2, BinOp>&&
            std::is_same<T1, typename impl::binary_operation_result<T1, T2, BinOp>::type>::value;

        // concept checking both arguments are compatible, enabled and the result is not a complement
        template<typename T1, typename T2, template<typename...> class BinOp>
        concept LogicalOperationEnabled = BinaryOperationEnabled<T1, T2, BinOp>&&
            is_flags<typename impl::binary_operation_result<T1, T2, BinOp>::type>::value;

        template<typename T1, typename T2, typename Check = impl::compatibility_check_t<T1,T2>>
            requires BinaryOperationEnabled<T1, T2, std::conjunction>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr auto
            operator&(T1 lhs, T2 rhs) {
            using result_t = typename impl::binary_operation_result<T1, T2, std::conjunction>::type;

            return result_t{
                static_cast<enum_type_t<T1>>(impl::get_underlying(lhs) & impl::get_underlying(rhs))
            };
        }

        template<typename T1, typename T2, typename Check = impl::compatibility_check_t<T1, T2>>
            requires BinaryOperationEnabled<T1, T2, std::disjunction>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr auto
            operator|(T1 lhs, T2 rhs) {
            using result_t = typename impl::binary_operation_result<T1, T2, std::disjunction>::type;

            return result_t{
                static_cast<enum_type_t<T1>>(impl::get_underlying(lhs) | impl::get_underlying(rhs))
            };
        }

        template<typename T1, typename T2, typename Check = impl::compatibility_check_t<T1, T2>>
            requires BinaryOperationEnabled<T1, T2, impl::not_equal>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr auto
            operator^(T1 lhs, T2 rhs) {
            using result_t = typename impl::binary_operation_result<T1, T2, impl::not_equal>::type;

            return result_t{
                static_cast<enum_type_t<T1>>(impl::get_underlying(lhs) ^ impl::get_underlying(rhs))
            };
        }


        template<typename T> 
            requires UnaryOperationEnabled<T, std::negation>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr auto
            operator~(T value) {
            using result_t = typename impl::unary_operation_result<T, std::negation>::type;

            return result_t{
                static_cast<enum_type_t<T>>(~impl::get_underlying(value))
            };
        }


        template<typename T1, typename T2, typename Check = impl::compatibility_check_t<T1, T2>>
            requires BinaryAssignmentEnabled<T1, T2, std::conjunction>
        constexpr T1&
            operator&=(T1& lhs, T2 rhs) {
            lhs = lhs & rhs;
            return lhs;
        }

        template<typename T1, typename T2, typename Check = impl::compatibility_check_t<T1, T2>>
            requires BinaryAssignmentEnabled<T1, T2, std::disjunction>
        constexpr T1&
            operator|=(T1& lhs, T2 rhs) {
            lhs = lhs | rhs;
            return lhs;
        }

        template<typename T1, typename T2, typename Check = impl::compatibility_check_t<T1, T2>>
            requires BinaryAssignmentEnabled<T1, T2, impl::not_equal>
        constexpr T1&
            operator^=(T1& lhs, T2 rhs) {
            lhs = lhs ^ rhs;
            return lhs;
        }


        template<typename T>
            requires IsFlags<T>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            operator!(T e) {
            return !impl::get_underlying(e);
        }

        // test for == 0 / != 0
        template<typename T>
            requires IsFlags<T>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            operator==(T value, nullptr_t) {
            return impl::get_underlying(value) == 0;
        }

#if __cplusplus < 202002
        // no rewritten candidates

        template<typename T>
            requires IsFlags<T>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            operator==(nullptr_t, T value) {
            return impl::get_underlying(value) == 0;
        }

        template<typename T>
            requires IsFlags<T>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            operator!=(T value, nullptr_t) {
            return !(impl::get_underlying(value) == 0);
        }

        template<typename T>
            requires IsFlags<T>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            operator!=(nullptr_t, T value) {
            return !(impl::get_underlying(value) == 0);
        }
#endif


        // test for == 0 / != 0
        template<typename T>
            requires IsFlags<T>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            operator==(T value, impl::null_tag) {
            return impl::get_underlying(value) == 0;
        }

#if __cplusplus < 202002
        // no rewritten candidates

        template<typename T>
            requires IsFlags<T>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            operator==(impl::null_tag, T value) {
            return impl::get_underlying(value) == 0;
        }

        template<typename T>
            requires IsFlags<T>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            operator!=(T value, impl::null_tag) {
            return !(impl::get_underlying(value) == 0);
        }

        template<typename T>
            requires IsFlags<T>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            operator!=(impl::null_tag, T value) {
            return !(impl::get_underlying(value) == 0);
        }
#endif




        // conversion to / from underlying type
        template<typename T>
            requires IsEnabled<T>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr auto
            get_underlying(T value) {
            return impl::get_underlying(value);
        }

        template<typename T>
            requires IsEnabled<T>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr auto
            from_underlying(typename boost::underlying_type<enum_type_t<T>>::type value) {
            if constexpr (IsComplement<T>) {
                return complement{ static_cast<enum_type_t<T>>(value) };
            }
            else {
                return static_cast<enum_type_t<T>>(value);
            }
        }


        // delete operator == (and also !=) for comparison of incompatible types
        // only deleted for non-class enums
        template<typename T1, typename T2>
            requires ((IsEnabled<T1> && !impl::is_scoped_enum<enum_type_t<T1>>::value)
                      ||
                      (IsEnabled<T2> && !impl::is_scoped_enum<enum_type_t<T2>>::value))
                      && (!IsCompatibleFlagsOrComplement<T1, T2>)
        BOOST_ATTRIBUTE_NODISCARD
            constexpr bool operator == (T1, T2) = delete;

        // delete operator == (and also !=) for comparison of incompatible types
        template<typename T1, typename T2>
            requires IsCompatibleFlagsOrComplement<T1, T2>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr bool operator == (T1 e1, T2 e2) {
            return get_underlying(e1) == get_underlying(e2);
        }

        // disabling logical operators
        // 
        template<typename T1, typename T2>
            requires (IsEnabled<T1> || IsEnabled<T2>) // && (!IsCompatibleFlags<T1, T2>)
        BOOST_ATTRIBUTE_NODISCARD
            constexpr bool operator && (T1, T2) = delete;

        template<typename T1, typename T2>
            requires (IsEnabled<T1> || IsEnabled<T2>) // && (!IsCompatibleFlags<T1, T2>)
        BOOST_ATTRIBUTE_NODISCARD
            constexpr bool operator || (T1, T2) = delete;


        // disabling relational operators
        // 
        template<typename T1, typename T2>
            requires (IsEnabled<T1> || IsEnabled<T2>) && (!IsCompatibleFlagsOrComplement<T1, T2>)
        BOOST_ATTRIBUTE_NODISCARD
            constexpr std::partial_ordering operator <=> (T1, T2) = delete;

        namespace impl {
            template<typename T1, typename T2>
            BOOST_ATTRIBUTE_NODISCARD
                std::partial_ordering normalized_contained_induced_compare(T1 l, T2 r) {
                return l == r
                    ? std::partial_ordering::equivalent
                    : (l & r) == l
                    ? std::partial_ordering::less
                    : (l & r) == r
                    ? std::partial_ordering::greater
                    : std::partial_ordering::unordered
                    ;
            }

            template<typename T1, typename T2>
            BOOST_ATTRIBUTE_NODISCARD
                std::partial_ordering contained_induced_compare(T1 l, T2 r) {
                return normalized_contained_induced_compare(
                    get_normalized(l),
                    get_normalized(r)
                );
            }
        }

        // end of core part
        //
        ///////////////////////////////////////////////////////////////////////////////////////
        

        // test if any bit is set
        template<typename T>
            requires IsFlags<T>
            BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            any(T e) {
            return impl::get_underlying(e) != 0;
        }

        // test if no bit is set
        template<typename T>
            requires IsFlags<T>
            BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            none(T e) {
            return !e;
        }


        // test if test_subset is completly contained in superset
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
            BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            contains(T1 superset, T2 test_subset) {
            return (superset & test_subset) == test_subset;
        }


        // test if test_subset is completly contained in superset
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
            BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            intersect(T1 lhs, T2 rhs) {
            return (impl::get_underlying(lhs) & impl::get_underlying(rhs)) != 0;
        }


        // test if test_subset is completly contained in superset
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
            BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            disjoint(T1 lhs, T2 rhs) {
            return (impl::get_underlying(lhs) & impl::get_underlying(rhs)) == 0;
        }


        // returns an empty instance of T
        template<typename T>
            requires IsFlags<T>
            BOOST_ATTRIBUTE_NODISCARD
            constexpr enum_type_t<T>
            make_null(T) {
            return static_cast<enum_type_t<T>>(0);
        }


        // depending on set
        // returns e or an empty instance of T
        template<typename T>
            requires IsFlags<T>
            BOOST_ATTRIBUTE_NODISCARD
            constexpr enum_type_t<T>
            make_if(T e, bool set) {
            return static_cast<enum_type_t<T>>(set ? impl::get_underlying(e) : 0);
        }

        // return a copy of value with all
        // bits of modification set resp. cleared
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
            BOOST_ATTRIBUTE_NODISCARD
            constexpr enum_type_t<T1>
            modify(T1 value, T2 modification, bool set) {
            return set ? (value | modification) : (value & ~modification);
        }

        // sets resp. clears the bits of modification
        // in value in-place
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2> 
            constexpr T1&
            modify_inplace(T1& value, T2 modification, bool set) {
            value = set ? (value | modification) : (value & ~modification);
            return value;
        }



        template<typename T>
            requires IsEnabled<T>
        BOOST_ATTRIBUTE_NODISCARD
            constexpr impl::pseudo_and_op_intermediate_t<T>
            operator&(T lhs, impl::pseudo_and_op_tag) {
            return { lhs };
        }

        template<typename T1, typename T2, typename Check = impl::compatibility_check_t<T1, T2>>
            requires LogicalOperationEnabled<T1, T2, std::conjunction>
            BOOST_ATTRIBUTE_NODISCARD
            constexpr bool
            operator&(impl::pseudo_and_op_intermediate_t<T1> lhs, T2 rhs) {
            return (impl::get_underlying(lhs.value) & impl::get_underlying(rhs)) != 0;
        }


        // returns a value with the n-th (zero-indexed) bit set
        BOOST_ATTRIBUTE_NODISCARD
            inline constexpr auto nth_bit(unsigned int n) { return 1 << n; }
    }
}


using boost::flags::operator |;
using boost::flags::operator &;
using boost::flags::operator ^;
using boost::flags::operator ~;
using boost::flags::operator |=;
using boost::flags::operator &=;
using boost::flags::operator ^=;
using boost::flags::operator !;
using boost::flags::operator ==;
#if __cplusplus < 202002
using boost::flags::operator !=;
#endif

using boost::flags::operator <=>;

using boost::flags::operator &&;
using boost::flags::operator ||;



#define BOOST_FLAGS_REL_OPS_DELETE(E)                                   \
/* needed to match better than built-in relational operators */            \
std::partial_ordering operator <=> (E l, E r) = delete;                 \
                                                                               \
/* needed to match all other E, complement<E> arguments */            \
template<typename T1, typename T2>                                             \
    requires (std::is_same_v<E, boost::flags::enum_type_t<T1>> &&                            \
              std::is_same_v<E, boost::flags::enum_type_t<T2>> )           \
std::partial_ordering operator <=> (T1 l, T2 r) = delete;                 \


#define BOOST_FLAGS_REL_OPS_PARTIAL_ORDER(E)                                   \
/* needed to match better than built-in relational operators */            \
std::partial_ordering operator <=> (E l, E r) {                                \
    return boost::flags::impl::normalized_contained_induced_compare(l, r); \
}                                                                              \
                                                                               \
/* needed to match all other E, complement<E> arguments */            \
template<typename T1, typename T2>                                             \
    requires (std::is_same_v<E, boost::flags::enum_type_t<T1>> &&                            \
              std::is_same_v<E, boost::flags::enum_type_t<T2>> &&                            \
                boost::flags::IsCompatibleFlagsOrComplement<T1, T2>)           \
std::partial_ordering operator <=> (T1 l, T2 r) {                              \
    return boost::flags::impl::contained_induced_compare(l, r);            \
}                                                                              \



#define BOOST_FLAGS_PSEUDO_AND_OPERATOR & boost::flags::impl::pseudo_and_op_tag{} &

#define BF_AND  BOOST_FLAGS_PSEUDO_AND_OPERATOR

#define BOOST_FLAGS_NULL boost::flags::impl::null_tag{}
#define BF_NULL  BOOST_FLAGS_NULL


#endif  // BOOST_FLAGS_HPP_INCLUDED
