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

#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>


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
    #include <boost/flags.hpp>

    enum class pizza_toppings : unsigned int {
        tomato      = boost::flags::nth_bit(0), // == 0x01
        cheese      = boost::flags::nth_bit(1), // == 0x02
        salami      = boost::flags::nth_bit(2), // == 0x04
        olives      = boost::flags::nth_bit(3), // == 0x08
    };
    // enable Boost.Flags for pizza_toppings
    static constexpr bool boost_flags_enable(pizza_toppings) { return true; }

    enum class ice_cream_flavours : unsigned int {
        vanilla     = boost::flags::nth_bit(0), // == 0x01
        chocolate   = boost::flags::nth_bit(1), // == 0x02
        strawberry  = boost::flags::nth_bit(2), // == 0x04
    };
    // enable Boost.Flags for ice_cream_flavours
    static constexpr bool boost_flags_enable(ice_cream_flavours) { return true; }

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

    enum class flags_t : unsigned int {
        a = 0x1,
        b = 0x2,
        c = 0x4,
    };

    enum class flags2_t : unsigned int {
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
        // using pseudo operator BOOST_FLAG_AND
        // BOOST_FLAG_AND(x,y) -> bool
        // BOOST_FLAG_AND(x,y) ::= (x != 0) && (y != 0)
        if (ab_and_bc BOOST_FLAG_AND not_a) {
            // ...
        }
    }
*/




// detecting g++
#if defined(__GNUC__) && (!defined(__clang__) || defined(__MINGW32__))
# define BOOST_FLAGS_IS_GCC_COMPILER 1
#else // defined(__GNUC__) && !defined(__clang__)
# define BOOST_FLAGS_IS_GCC_COMPILER 0
#endif // defined(__GNUC__) && !defined(__clang__)


// check if the spaceship has alreay landed
#if !defined(BOOST_FLAGS_HAS_THREE_WAY_COMPARISON)
// g++ does not allow overwriting rel. operators with spaceship for enums 
// cf. https://cplusplus.github.io/CWG/issues/2673.html
// cf. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105200
// hopefully g++ 14 will have fixed it
# if (BOOST_FLAGS_IS_GCC_COMPILER && __GNUC__ < 14) || !defined(__cpp_impl_three_way_comparison)
#  define BOOST_FLAGS_HAS_THREE_WAY_COMPARISON 0
# else // (BOOST_FLAGS_IS_GCC_COMPILER && __GNUC__ < 14) || !defined(__cpp_impl_three_way_comparison)
#  define BOOST_FLAGS_HAS_THREE_WAY_COMPARISON 1
# endif // (BOOST_FLAGS_IS_GCC_COMPILER && __GNUC__ < 14) || !defined(__cpp_impl_three_way_comparison)
#endif // !defined(BOOST_FLAGS_HAS_THREE_WAY_COMPARISON)


// check for library support of three-way-comparison (flags will use std::partial_ordering)
#if !defined(BOOST_FLAGS_HAS_PARTIAL_ORDERING)
# if defined(__has_include)
#  if __has_include(<compare>) && defined(__cpp_lib_three_way_comparison) && (__cpp_lib_three_way_comparison >= 201907L)
#   define BOOST_FLAGS_HAS_PARTIAL_ORDERING 1
#  else // __has_include(<compare>) && defined(__cpp_lib_three_way_comparison) && (__cpp_lib_three_way_comparison >= 201907L)
#   define BOOST_FLAGS_HAS_PARTIAL_ORDERING 0
#  endif // __has_include(<compare>) && defined(__cpp_lib_three_way_comparison) && (__cpp_lib_three_way_comparison >= 201907L)
# else // defined(__has_include)
#  define BOOST_FLAGS_HAS_PARTIAL_ORDERING 0
# endif // defined(__has_include)
#endif // !defined(BOOST_FLAGS_HAS_PARTIAL_ORDERING)

#if (BOOST_FLAGS_HAS_THREE_WAY_COMPARISON) && !(BOOST_FLAGS_HAS_PARTIAL_ORDERING)
// this should usually not happen as spaceship requires std::partial_ordering
# undef BOOST_FLAGS_HAS_PARTIAL_ORDERING
# define BOOST_FLAGS_HAS_PARTIAL_ORDERING 1
#endif // (BOOST_FLAGS_HAS_THREE_WAY_COMPARISON) && !(BOOST_FLAGS_HAS_PARTIAL_ORDERING)

//
// [[nodiscard]] attribute
//

#if !defined(BOOST_FLAGS_ATTRIBUTE_NODISCARD)
# if defined(__has_attribute) && defined(__SUNPRO_CC) && (__SUNPRO_CC > 0x5130)
# if __has_attribute(nodiscard)
#  define BOOST_FLAGS_ATTRIBUTE_NODISCARD [[nodiscard]]
# endif
# if __has_attribute(no_unique_address)
#  define BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS [[no_unique_address]]
# endif
# elif defined(__has_cpp_attribute)
// clang-6 accepts [[nodiscard]] with -std=c++14, but warns about it -pedantic
# if __has_cpp_attribute(nodiscard) && !(defined(__clang__) && (__cplusplus < 201703L)) && !(defined(__GNUC__) && (__cplusplus < 201100))
#  define BOOST_FLAGS_ATTRIBUTE_NODISCARD [[nodiscard]]
# endif
# endif
# ifndef BOOST_FLAGS_ATTRIBUTE_NODISCARD
#  define BOOST_FLAGS_ATTRIBUTE_NODISCARD
# endif
#endif // !defined(BOOST_FLAGS_ATTRIBUTE_NODISCARD)


#if !defined(BOOST_FLAGS_ATTRIBUTE_NODISCARD_CTOR)
# if (defined(__clang_major__) && (__clang_major__ < 10)) || (defined(__GNUC__) && (__GNUC__ < 10))
#  define BOOST_FLAGS_ATTRIBUTE_NODISCARD_CTOR
# else // (defined(__clang_major__) && (__clang_major__ < 10)) || (defined(__GNUC__) && (__GNUC__ < 10))
#  define BOOST_FLAGS_ATTRIBUTE_NODISCARD_CTOR BOOST_FLAGS_ATTRIBUTE_NODISCARD
# endif // (defined(__clang_major__) && (__clang_major__ < 10)) || (defined(__GNUC__) && (__GNUC__ < 10))
#endif // !defined(BOOST_FLAGS_ATTRIBUTE_NODISCARD_CTOR)


// adapted from boost/asio/detail/config.hpp
// Support concepts on compilers known to allow them.
#if !defined(BOOST_FLAGS_HAS_CONCEPTS)
# if !defined(BOOST_FLAGS_DISABLE_CONCEPTS)
#  if defined(__cpp_concepts)
#   define BOOST_FLAGS_HAS_CONCEPTS 1
#   if (__cpp_concepts >= 201707)
#    define BOOST_FLAGS_CONCEPT concept
#   else // (__cpp_concepts >= 201707)
#    define BOOST_FLAGS_CONCEPT concept bool
#   endif // (__cpp_concepts >= 201707)
#  else // defined(__cpp_concepts)
#   define BOOST_FLAGS_HAS_CONCEPTS 0
#  endif // defined(__cpp_concepts)
# else // !defined(BOOST_FLAGS_DISABLE_CONCEPTS)
#  define BOOST_FLAGS_HAS_CONCEPTS 0
# endif // !defined(BOOST_FLAGS_DISABLE_CONCEPTS)
#endif // !defined(BOOST_FLAGS_HAS_CONCEPTS)


// check if std::is_scoped_enum is available
#if !defined(BOOST_FLAGS_HAS_IS_SCOPED_ENUM)
# if defined(__cpp_lib_is_scoped_enum)
#  define BOOST_FLAGS_HAS_IS_SCOPED_ENUM 1
# else // defined(__cpp_lib_is_scoped_enum)
#  define BOOST_FLAGS_HAS_IS_SCOPED_ENUM 0
# endif // defined(__cpp_lib_is_scoped_enum)
#endif // !defined(BOOST_FLAGS_HAS_IS_SCOPED_ENUM)


// check for std::conjunction, std::disjunction and std::negation
#if !defined(BOOST_FLAGS_HAS_LOGICAL_TRAITS)
# if defined(__cpp_lib_logical_traits)
#  define BOOST_FLAGS_HAS_LOGICAL_TRAITS 1
# else //  defined(__cpp_lib_logical_traits)
#  define BOOST_FLAGS_HAS_LOGICAL_TRAITS 0
# endif //  defined(__cpp_lib_logical_traits)
#endif // !defined(BOOST_FLAGS_HAS_LOGICAL_TRAITS)

// first check if user requested explicit definition
#if defined(BOOST_FLAGS_DEFINE_PARTIAL_ORDERING_OBJECTS) && (BOOST_FLAGS_DEFINE_PARTIAL_ORDERING_OBJECTS)
#if !defined(BOOST_FLAGS_WEAK_SYMBOL)
#  define BOOST_FLAGS_WEAK_SYMBOL 
# endif // !defined(BOOST_FLAGS_WEAK_SYMBOL)
#endif // defined(BOOST_FLAGS_DEFINE_PARTIAL_ORDERING_OBJECTS) && (BOOST_FLAGS_DEFINE_PARTIAL_ORDERING_OBJECTS)



// check, if inline varibles are supported (MSVC requires at least v142)
#if !defined(BOOST_FLAGS_HAS_INLINE_VARIABLES)
# if defined(__cpp_inline_variables) && (__cpp_inline_variables >= 201606L) && (!defined(_MSC_VER) || _MSC_VER >= 1920)
#  define BOOST_FLAGS_HAS_INLINE_VARIABLES 1
# else //  defined(__cpp_inline_variables) && (__cpp_inline_variables >= 201606L)
#  define BOOST_FLAGS_HAS_INLINE_VARIABLES 0
# endif //  defined(__cpp_inline_variables) && (__cpp_inline_variables >= 201606L)
#endif // !defined(BOOST_FLAGS_HAS_INLINE_VARIABLES)


// workaround for MSVC v140 (VS 2015): constexpr function templates not recoginzed correctly
// thus, cannot be used to construct std::integral_constants
#if !defined(BOOST_FLAGS_NO_CONSTEXPR_FUNCTION_TEMPLATES)
# if defined(_MSC_VER) && _MSC_VER < 1910
#  define BOOST_FLAGS_NO_CONSTEXPR_FUNCTION_TEMPLATES 1
# else // defined(_MSC_VER) && _MSC_VER < 1910
#  define BOOST_FLAGS_NO_CONSTEXPR_FUNCTION_TEMPLATES 0
# endif //  defined(_MSC_VER) && _MSC_VER < 1910
#endif // !defined(BOOST_FLAGS_NO_CONSTEXPR_FUNCTION_TEMPLATES)


// attribute for defining weak symbols 
// only needed when std::partial_ordering is not present and inline variables are not supported
#if !defined(BOOST_FLAGS_WEAK_SYMBOL) && (BOOST_FLAGS_HAS_INLINE_VARIABLES)
# define BOOST_FLAGS_WEAK_SYMBOL inline
#endif // !defined(BOOST_FLAGS_WEAK_SYMBOL) && (BOOST_FLAGS_HAS_INLINE_VARIABLES)

// explicit enable/disable definition of emulated partial_ordering objects
// again, only needed when std::partial_ordering is not present and inline variables are not supported 
// at least gcc on mingw has problems with weak symbols
#if !defined(BOOST_FLAGS_DEFINE_PARTIAL_ORDERING_OBJECTS)
# define BOOST_FLAGS_DEFINE_PARTIAL_ORDERING_OBJECTS 1
#endif // !defined(BOOST_FLAGS_DEFINE_PARTIAL_ORDERING_OBJECTS)


// trying to set up weak-symbols appropriate
#if !defined(BOOST_FLAGS_WEAK_SYMBOL)

// only required for definition of partial_ordering
# if BOOST_FLAGS_HAS_PARTIAL_ORDERING

// BOOST_FLAGS_WEAK_SYMBOL is not used

# else // BOOST_FLAGS_HAS_PARTIAL_ORDERING

// adapted from boost/dll/alias.hpp
#  if defined(_MSC_VER) // MSVC, Clang-cl, and ICC on Windows
#   define BOOST_FLAGS_WEAK_SYMBOL __declspec(selectany)
#  else // defined(_MSC_VER)
#   if BOOST_FLAGS_IS_GCC_COMPILER
#    if !defined(__MINGW32__)
        // There are some problems with mixing `__dllexport__` and `weak` using MinGW
        // See https://sourceware.org/bugzilla/show_bug.cgi?id=17480
#     define BOOST_FLAGS_WEAK_SYMBOL __attribute__((weak))
#    else // !defined(__MINGW32__)
#     define BOOST_FLAGS_WEAK_SYMBOL
#    endif // !defined(__MINGW32__)
#   else // BOOST_FLAGS_IS_GCC_COMPILER
#    if defined(__clang__)
#     define BOOST_FLAGS_WEAK_SYMBOL __attribute__((weak))
#    else // defined(__clang__)
#     define BOOST_FLAGS_WEAK_SYMBOL
#    endif // defined(__clang__)
#   endif // BOOST_FLAGS_IS_GCC_COMPILER
#  endif // defined(_MSC_VER)
# endif // BOOST_FLAGS_HAS_PARTIAL_ORDERING

#endif // !defined(BOOST_FLAGS_WEAK_SYMBOL)


// operator rewritten candidates
#if !defined(BOOST_FLAGS_HAS_REWRITTEN_CANDIDATES)

# if __cplusplus < 202002L

#  define BOOST_FLAGS_HAS_REWRITTEN_CANDIDATES 0

# else // __cplusplus < 202002L

#  define BOOST_FLAGS_HAS_REWRITTEN_CANDIDATES 1

# endif // __cplusplus < 202002L

#endif // !defined(BOOST_FLAGS_HAS_REWRITTEN_CANDIDATES)


#if BOOST_FLAGS_HAS_PARTIAL_ORDERING
#include <compare>
#endif




namespace boost {
    namespace flags {

        // non-intrusive opt-in to operations of boost::flags
        // overload `boost_flags_enable` for scoped or unscoped enums with
        // constexpr inline bool boost_flags_enable(my_enum) { return true; }
        // `boost_flags_enable` will be found by ADL
        // (alt. specialize `template<> struct boost::flags::enable<my_enum> : std::true_type {};`)
        // to enable operations for scoped or unscoped enums

#if BOOST_FLAGS_NO_CONSTEXPR_FUNCTION_TEMPLATES
        // workaround for msvc v140 (constexpr function templates not recognized correctly)
        // used good old variadic arguments
        constexpr inline bool boost_flags_enable(...) { return false; }
        //constexpr inline bool boost_flags_disable_complement(...) { return false; }
        //constexpr inline bool boost_flags_logical_and(...) { return false; }
#else // BOOST_FLAGS_NO_CONSTEXPR_FUNCTION_TEMPLATES
        template<typename E>
        constexpr inline bool boost_flags_enable(E) { return false; }

        //template<typename E>
        //constexpr inline bool boost_flags_disable_complement(E) { return false; }

        //template<typename E>
        //constexpr inline bool boost_flags_logical_and(E) { return false; }
#endif // BOOST_FLAGS_NO_CONSTEXPR_FUNCTION_TEMPLATES


        // options flags
        enum class options : uint8_t {
            enable              = 0x1,
            disable_complement  = 0x2,
            logical_and         = 0x4,
        };


        // derive `enable` from this type to disable the `complement` template for the enabled enumeration
        struct disable_complement {};

        // derive `enable` from this type to enable support for logical and
        struct logical_and {};



        namespace impl {
            // error tag indicating invalid/incompatible types for operation
            struct error_tag {};

            // empty struct
            template<typename Tag>
            struct empty {};


            // the `bool` versions for the options-detectors
            // the overloads for `options` are define below and will be picked up by ADL
            constexpr inline bool is_option_enable(bool v) { return v; }
            constexpr inline bool is_option_disable_complement(bool) { return false; }
            constexpr inline bool is_option_logical_and(bool) { return false; }


            // type to calculate the enabling (using concepts/SFINAE)
            // enable helper

#if BOOST_FLAGS_HAS_CONCEPTS
            template<typename E>
#else // BOOST_FLAGS_HAS_CONCEPTS
            template<typename E, typename = void>   // required for SFINAE        
#endif // BOOST_FLAGS_HAS_CONCEPTS
            struct enable_helper : std::false_type {};


#if BOOST_FLAGS_HAS_CONCEPTS
            template<typename E>
                requires std::is_enum_v<E>
            struct enable_helper<E>
#else // BOOST_FLAGS_HAS_CONCEPTS
            template<typename E>
            struct enable_helper<E, typename std::enable_if<std::is_enum<E>::value>::type>
#endif // BOOST_FLAGS_HAS_CONCEPTS
                : std::integral_constant<bool, is_option_enable(boost_flags_enable(E{})) >
                , std::conditional < is_option_disable_complement(boost_flags_enable(E{})), disable_complement, impl::empty<disable_complement> > ::type
                , std::conditional < is_option_logical_and(boost_flags_enable(E{})), logical_and, impl::empty<logical_and> > ::type
            {};

        }


        // main enabling template
        // enumeration `E` will use operators and functions from this library
        // if `enable<E>::value == true` (e.g. inherits from `std::true_type`)
        template<typename E>
        struct enable : impl::enable_helper<E> {};


        // explicitly disable error_tag
        template<>
        struct enable<impl::error_tag> : std::false_type {};

        // enable options enumeration
        template<>
        struct enable<options> : std::true_type {};



        namespace impl {

#if BOOST_FLAGS_HAS_IS_SCOPED_ENUM
            template<typename E>
            using is_scoped_enum = std::is_scoped_enum<E>;
#else
# if BOOST_FLAGS_HAS_CONCEPTS
            template<typename E>
            struct is_scoped_enum : std::bool_constant < requires
            {
                requires std::is_enum_v<E>;
                requires !std::is_convertible_v<E, std::underlying_type_t<E>>;
            } >
            {};
# else // BOOST_FLAGS_HAS_CONCEPTS

            // need to jump through some hoops to work around SFINAE unfriendly old std::underlying_type

            template <typename T, bool B = std::is_enum<T>::value>
            struct is_scoped_enum : std::false_type {};

            template <typename T>
            struct is_scoped_enum<T, true> : std::integral_constant<bool,
                !std::is_convertible<T, typename std::underlying_type<T>::type>::value
            > {};

# endif // BOOST_FLAGS_HAS_CONCEPTS
#endif

#if BOOST_FLAGS_HAS_LOGICAL_TRAITS

            template<class... Args>
            using conjunction = std::conjunction<Args...>;

            template<class... Args>
            using disjunction = std::disjunction<Args...>;

            template<class B>
            using negation = std::negation<B>;

#else // BOOST_FLAGS_HAS_LOGICAL_TRAITS

            template<class...> struct conjunction : std::true_type {};
            template<class B1> struct conjunction<B1> : B1 {};
            template<class B1, class... Bn>
            struct conjunction<B1, Bn...>
                : std::conditional<bool(B1::value), conjunction<Bn...>, B1>::type {};

            template<class...> struct disjunction : std::false_type {};
            template<class B1> struct disjunction<B1> : B1 {};
            template<class B1, class... Bn>
            struct disjunction<B1, Bn...>
                : std::conditional<bool(B1::value), B1, disjunction<Bn...>>::type {};

            template<class B>
            struct negation : std::integral_constant<bool, !bool(B::value)> { };

#endif // BOOST_FLAGS_HAS_LOGICAL_TRAITS

        } // namespace impl


        template<typename E, bool B = std::is_enum<E>::value>
        struct complement;

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

        // class-template to indicate complemented flags
        // version for nested complements
        template<typename E, bool B /*= std::is_enum<E>::value*/>
        struct complement {
            using enumeration_type = typename enum_type_t<E>;
            using underlying_type = typename std::underlying_type<enumeration_type>::type;
            using value_type = underlying_type;

            BOOST_FLAGS_ATTRIBUTE_NODISCARD_CTOR
                constexpr operator underlying_type() const { return get_underlying(); }

            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr underlying_type get_underlying() const { return value.get_underlying(); }

            E value;
        };

        // class-template to indicate complemented flags
        // version for complement of enumeration (not nested)
        template<typename E>
        struct complement<E,true> {
            using enumeration_type = typename enum_type_t<E>;
            using underlying_type = typename std::underlying_type<enumeration_type>::type;
            using value_type = underlying_type;

            complement() = default;

            BOOST_FLAGS_ATTRIBUTE_NODISCARD_CTOR
                constexpr complement(value_type v) :
                value{ v }
            {}

            BOOST_FLAGS_ATTRIBUTE_NODISCARD_CTOR
                constexpr complement(E v) :
                value{ static_cast<underlying_type>(v) }
            {}

            BOOST_FLAGS_ATTRIBUTE_NODISCARD_CTOR
                constexpr operator underlying_type() const { return get_underlying(); }

            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr underlying_type get_underlying() const { return value; }

            value_type value;
        };
        //// class-template to indicate complemented flags
        //template<typename E>
        //struct complement {
        //    using enumeration_type = typename enum_type_t<E>;
        //    using underlying_type = typename std::underlying_type<enumeration_type>::type;

        //    complement() = default;
        //    
        //    BOOST_FLAGS_ATTRIBUTE_NODISCARD_CTOR
        //        constexpr complement(underlying_type v) :
        //        value{ v }
        //    {}

        //    BOOST_FLAGS_ATTRIBUTE_NODISCARD_CTOR
        //        constexpr complement(E v) :
        //        value{ static_cast<underlying_type>(v) }
        //    {}

        //    BOOST_FLAGS_ATTRIBUTE_NODISCARD_CTOR
        //        constexpr operator underlying_type() const { return value; }

        //    // using the underlying_type (instead of enumeration_type or E) ensures that for
        //    // unscoped enumerations with unspecified underlying type, we
        //    // don't get into trouble with the "hypothetical value type"
        //    // cf. https://eel.is/c++draft/dcl.enum#8
        //    underlying_type value;
        //};



        // test if E is enabled: either a flags-enum or a negation (detects double-negations)
        template<typename E>
        struct is_enabled;

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
        concept IsComplementDisabled =
            std::is_base_of_v<disable_complement, enable<enum_type_t<T>>>;
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
        struct IsComplementDisabled : std::integral_constant<bool,
            std::is_base_of<disable_complement, enable<typename enum_type<T>::type>>::value
        > {};
#endif // BOOST_FLAGS_HAS_CONCEPTS

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
        concept HasLogicalAnd =
            std::is_base_of_v<logical_and, enable<enum_type_t<T>>>;
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
        struct HasLogicalAnd : std::integral_constant<bool,
            std::is_base_of<logical_and, enable<typename enum_type<T>::type>>::value
        > {};
#endif // BOOST_FLAGS_HAS_CONCEPTS

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
            impl::conjunction<
            impl::negation<is_flags<E>>,
            is_enabled<E>                       // ensure it's enabled
            > {};


        // test for complement (detects double-negation)
        template<typename E>
        struct is_complement :
            impl::conjunction<
            impl::negation<is_flags<E>>,
            is_enabled<E>                       // ensure it's enabled
            > {};


#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename E>
        concept IsFlags = is_flags<E>::value;

        template<typename E>
        concept IsComplement = is_complement<E>::value;

        template<typename E>
        concept IsEnabled = is_enabled<E>::value;


        template<typename T1, typename T2>
        concept IsCompatible = std::is_same<enum_type_t<T1>, enum_type_t<T2>>::value;

        template<typename T1, typename T2>
        concept IsCompatibleFlags = IsCompatible<T1, T2>&&
            IsFlags<T1>&& IsFlags<T2>;

        template<typename T1, typename T2>
        concept IsCompatibleComplement = IsCompatible<T1, T2>&&
            IsComplement<T1>&& IsComplement<T2>;

        template<typename T1, typename T2>
        concept IsCompatibleFlagsOrComplement = IsCompatible<T1, T2> &&
            ((IsFlags<T1>&& IsFlags<T2>) || (IsComplement<T1> && IsComplement<T2>));

#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename E>
        struct IsFlags : std::integral_constant<bool,
            is_flags<E>::value
        > {};

        template<typename E>
        struct IsComplement : std::integral_constant<bool,
            is_complement<E>::value
        > {};

        template<typename E>
        struct IsEnabled : std::integral_constant<bool,
            is_enabled<E>::value
        > {};


        template<typename T1, typename T2>
        struct IsCompatible : std::integral_constant<bool,
            std::is_same<enum_type_t<T1>, enum_type_t<T2>>::value
        > {};

        template<typename T1, typename T2>
        struct IsCompatibleFlags : std::integral_constant<bool,
            IsCompatible<T1, T2>::value&&
            IsFlags<T1>::value&& IsFlags<T2>::value
        > {};

        template<typename T1, typename T2>
        struct IsCompatibleComplement : std::integral_constant<bool,
            IsCompatible<T1, T2>::value&&
            IsComplement<T1>::value&& IsComplement<T2>::value
        > {};

        template<typename T1, typename T2>
        struct IsCompatibleFlagsOrComplement : std::integral_constant<bool,
            IsCompatible<T1, T2>::value &&
            ((IsFlags<T1>::value&& IsFlags<T2>::value) || (IsComplement<T1>::value && IsComplement<T2>::value))
        > {};

#endif // BOOST_FLAGS_HAS_CONCEPTS

        namespace impl {

            // NULL constant tag
            struct null_tag {};

            // for pseudo operator BOOST_FLAG_AND
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
                impl::disjunction<
                std::is_same<typename std::remove_cv<typename std::remove_reference<E>::type>::type, pseudo_and_op_tag>,
                is_pseudo_and_op_intermediate<typename std::remove_cv<typename std::remove_reference<E>::type>::type>
                > {};

            // calculate whether result Op(T1) is in the original or the complemented boolean algebra
            // always returns the canonical form (either the enum or its negation, never a double negated enum)
            template<typename T1, template<typename> class UnOp>
            struct unary_operation_result {
                using E1 = enum_type_t<T1>;

                using type = typename std::conditional<
                    enable<E1>::value,        // check if undelying enum is enabled
                    typename std::conditional<
#if BOOST_FLAGS_HAS_CONCEPTS
                    IsComplementDisabled<T1>,
#else // BOOST_FLAGS_HAS_CONCEPTS
                    IsComplementDisabled<T1>::value,
#endif // BOOST_FLAGS_HAS_CONCEPTS
                    T1,
                    typename std::conditional<
                    UnOp<is_complement<T1>>::value,
                    complement<E1>,
                    E1
                    >::type
                    >::type,
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


            // checking: if T1 or T2 are enabled then both are equal or one of them is_pseudo_and_op_type
#if BOOST_FLAGS_HAS_CONCEPTS
            template<typename T1, typename T2>
            concept ArgumentsCompatible =
                !(enable<enum_type_t<T1>>::value || enable<enum_type_t<T2>>::value)
                || std::is_same<enum_type_t<T1>, enum_type_t<T2>>::value
                || is_pseudo_and_op_type<T1>::value
                || is_pseudo_and_op_type<T2>::value
                ;

            template<typename T>
            concept ImplicitIntegralConvertible =
                std::is_convertible<T, int>::value
                ;

            template<typename T1, typename T2>
            concept BothImplicitIntegralConvertible =
                ImplicitIntegralConvertible<T1> && ImplicitIntegralConvertible<T2>
                ;

#else // BOOST_FLAGS_HAS_CONCEPTS
            template<typename T1, typename T2>
            struct ArgumentsCompatible : std::integral_constant<bool,
                !(enable<enum_type_t<T1>>::value || enable<enum_type_t<T2>>::value)
                || std::is_same<enum_type_t<T1>, enum_type_t<T2>>::value
                || is_pseudo_and_op_type<T1>::value
                || is_pseudo_and_op_type<T2>::value
            > {};

            template<typename T>
            struct ImplicitIntegralConvertible : std::integral_constant<bool,
                std::is_convertible<T, int>::value
            > {};

            template<typename T1, typename T2>
            struct BothImplicitIntegralConvertible : std::integral_constant<bool,
                ImplicitIntegralConvertible<T1>::value && ImplicitIntegralConvertible<T2>::value
            > {};

#endif // BOOST_FLAGS_HAS_CONCEPTS

            // the standard doesn't provide a not_equal trait, so lets use our own
            template<typename B1, typename B2>
            struct not_equal : std::integral_constant<bool, bool(B1::value) != bool(B2::value)> {};


#if BOOST_FLAGS_HAS_CONCEPTS
            template<typename T>
                requires std::is_enum<T>::value
#else // BOOST_FLAGS_HAS_CONCEPTS
            template<typename T,
                typename std::enable_if<std::is_enum<T>::value, int*>::type = nullptr>
#endif // BOOST_FLAGS_HAS_CONCEPTS
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr auto get_underlying_impl(T value) noexcept -> typename std::underlying_type<T>::type {
                using underlying = typename std::underlying_type<T>::type;
                return static_cast<underlying>(value);
            }

            template<typename T>
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr auto get_underlying_impl(complement<T> value) noexcept -> typename std::underlying_type<enum_type_t<T>>::type {
                return value.get_underlying();
            }


            // normalize `complement`s to zero or one

            // test for outer `double complement`
            template<typename E>
            struct is_double_outer_complement : std::false_type {};

            template<typename E>
            struct is_double_outer_complement<complement<complement<E>>> :
                is_enabled<E>                       // ensure it's enabled
            {};


#if BOOST_FLAGS_HAS_CONCEPTS
            template<typename E>
            concept IsDoubleOuterComplement =
                is_double_outer_complement<std::remove_cvref_t<E>>::value;
#else // BOOST_FLAGS_HAS_CONCEPTS
            template<typename E>
            struct IsDoubleOuterComplement : std::integral_constant<bool,
                is_double_outer_complement<typename std::remove_cv<typename std::remove_reference<E>::type>::type>::value
            > {};
#endif // BOOST_FLAGS_HAS_CONCEPTS


            template<typename T>
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr T&& get_normalized(T&& value) noexcept {
                return std::forward<T>(value);
            }

#if BOOST_FLAGS_HAS_CONCEPTS
            template<typename T>
                requires IsDoubleOuterComplement<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
            template<typename T, typename std::enable_if<IsDoubleOuterComplement<T>::value, int*>::type = nullptr>
#endif // BOOST_FLAGS_HAS_CONCEPTS
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr
                auto
                get_normalized(T&& value) noexcept -> decltype(get_normalized(std::forward<T>(value).value.value)) {
                return get_normalized(std::forward<T>(value).value.value);
            }

        } // namespace impl


#if BOOST_FLAGS_HAS_CONCEPTS
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

        // concept checking both arguments are compatible, enabled and the result is not a complement
        // and logical_and is enabled
        template<typename T1, typename T2>
        concept LogicalAndEnabled = LogicalOperationEnabled<T1, T2, impl::conjunction>&& HasLogicalAnd<T1>;

#else // BOOST_FLAGS_HAS_CONCEPTS
        // concept checking if arguments is enabled
        template<typename T1, template<typename> class UnOp>
        struct UnaryOperationEnabled : std::integral_constant<bool,
            is_enabled<typename impl::unary_operation_result<T1, UnOp>::type>::value
        > {};

        // concept checking both arguments are compatible and enabled
        template<typename T1, typename T2, template<typename...> class BinOp>
        struct BinaryOperationEnabled : std::integral_constant<bool,
            is_enabled<typename impl::binary_operation_result<T1, T2, BinOp>::type>::value
        > {};

        // concept checking both arguments are compatible, enabled and the result is assignable to T1
        template<typename T1, typename T2, template<typename...> class BinOp>
        struct BinaryAssignmentEnabled : std::integral_constant<bool,
            BinaryOperationEnabled<T1, T2, BinOp>::value&&
            std::is_same<T1, typename impl::binary_operation_result<T1, T2, BinOp>::type>::value
        > {};

        // concept checking both arguments are compatible, enabled and the result is not a complement
        template<typename T1, typename T2, template<typename...> class BinOp>
        struct LogicalOperationEnabled : std::integral_constant<bool,
            BinaryOperationEnabled<T1, T2, BinOp>::value&&
            is_flags<typename impl::binary_operation_result<T1, T2, BinOp>::type>::value
        > {};

        // concept checking both arguments are compatible, enabled and the result is not a complement
        // and logical_and is enabled
        template<typename T1, typename T2>
        struct  LogicalAndEnabled : std::integral_constant<bool,
            LogicalOperationEnabled<T1, T2, impl::conjunction>::value&& HasLogicalAnd<T1>::value
        > {};

#endif // BOOST_FLAGS_HAS_CONCEPTS

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires BinaryOperationEnabled<T1, T2, impl::conjunction>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<BinaryOperationEnabled<T1, T2, impl::conjunction>::value, int*>::type = nullptr>
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr auto
            operator&(T1 lhs, T2 rhs) noexcept -> typename impl::binary_operation_result<T1, T2, impl::conjunction>::type {
            using result_t = typename impl::binary_operation_result<T1, T2, impl::conjunction>::type;

            using underlying_type = decltype(impl::get_underlying_impl(lhs));
            return result_t{
                static_cast<enum_type_t<T1>>(static_cast<underlying_type>(impl::get_underlying_impl(lhs) & impl::get_underlying_impl(rhs)))
            };
        }

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires(!impl::ArgumentsCompatible<T1, T2> && impl::BothImplicitIntegralConvertible<T1, T2>)
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<!impl::ArgumentsCompatible<T1, T2>::value && impl::BothImplicitIntegralConvertible<T1, T2>::value, int*>::type = nullptr>
#endif // BOOST_FLAGS_HAS_CONCEPTS
        constexpr unsigned int
            operator&(T1 lhs, T2 rhs) noexcept = delete;


#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires BinaryOperationEnabled<T1, T2, impl::disjunction>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<BinaryOperationEnabled<T1, T2, impl::disjunction>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr auto
            operator|(T1 lhs, T2 rhs) noexcept -> typename impl::binary_operation_result<T1, T2, impl::disjunction>::type {
            using result_t = typename impl::binary_operation_result<T1, T2, impl::disjunction>::type;

            using underlying_type = decltype(impl::get_underlying_impl(lhs));
            return result_t{
                static_cast<enum_type_t<T1>>(static_cast<underlying_type>(impl::get_underlying_impl(lhs) | impl::get_underlying_impl(rhs)))
            };
        }

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires(!impl::ArgumentsCompatible<T1, T2> && impl::BothImplicitIntegralConvertible<T1, T2>)
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<!impl::ArgumentsCompatible<T1, T2>::value && impl::BothImplicitIntegralConvertible<T1, T2>::value, int*>::type = nullptr>
#endif // BOOST_FLAGS_HAS_CONCEPTS
        constexpr unsigned int
            operator|(T1 lhs, T2 rhs) noexcept = delete;


#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires BinaryOperationEnabled<T1, T2, impl::not_equal>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<BinaryOperationEnabled<T1, T2, impl::not_equal>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr auto
            operator^(T1 lhs, T2 rhs) noexcept -> typename impl::binary_operation_result<T1, T2, impl::not_equal>::type {
            using result_t = typename impl::binary_operation_result<T1, T2, impl::not_equal>::type;

            using underlying_type = decltype(impl::get_underlying_impl(lhs));
            return result_t{
                static_cast<enum_type_t<T1>>(static_cast<underlying_type>(impl::get_underlying_impl(lhs) ^ impl::get_underlying_impl(rhs)))
            };
        }

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires(!impl::ArgumentsCompatible<T1, T2> && impl::BothImplicitIntegralConvertible<T1, T2>)
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<!impl::ArgumentsCompatible<T1, T2>::value && impl::BothImplicitIntegralConvertible<T1, T2>::value, int*>::type = nullptr>
#endif // BOOST_FLAGS_HAS_CONCEPTS
        constexpr unsigned int
            operator^(T1 lhs, T2 rhs) noexcept = delete;


#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires UnaryOperationEnabled<T, impl::negation>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<UnaryOperationEnabled<T, impl::negation>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr auto
            operator~(T value) noexcept -> typename impl::unary_operation_result<T, impl::negation>::type {
            using result_t = typename impl::unary_operation_result<T, impl::negation>::type;

            using underlying_type = decltype(impl::get_underlying_impl(value));

            // Ensure we do not leave the valid value range: would be UB for constant expressions!
            // https://eel.is/c++draft/expr.static.cast#10
            // https://eel.is/c++draft/dcl.enum#7
            // https://eel.is/c++draft/dcl.enum#8
            // 
            // Still, for unscoped enums with unspecified underlying type, the problem is not solvable:
            // The hypothetical integer value type is in general not the same as the deduced underlying type!
            //
            // E.g. clang reports warnings here, when used with unscoped enums with unspecified underlying type in constant expressions:
            // error: integer value 4294967294 is outside the valid range of values [0, 15] for this enumeration type [-Wenum-constexpr-conversion]

            // check that we don't get into trouble with strange complement implementations  
            static_assert(
                !(
#if BOOST_FLAGS_HAS_CONCEPTS
                    IsComplementDisabled<T>                    // complement is not used
#else // BOOST_FLAGS_HAS_CONCEPTS
                    IsComplementDisabled<T>::value             // complement is not used
#endif // BOOST_FLAGS_HAS_CONCEPTS
                    && std::is_signed<underlying_type>::value   // underlying type is signed
                    && __cplusplus < 202002L                    // before C++20
                 ),
                "For C++ standard before C++20 and disabled complement, Boost.Flags requires an unsigned underlying type.");

            return result_t{
                static_cast<enum_type_t<T>>(static_cast<underlying_type>(~impl::get_underlying_impl(value)))
            };
        }

        //
        // Note to the assignment operators:
        // Even though it's tempting to delegate to the built-in assignment operators, writing
        // 
        // E& operator&=(E& lhs, E rhs) { 
        //      return static_cast<E&>(
        //              static_cast<underlying_t<E>&>(lhs) &= static_cast<underlying_t<E>>(rhs)
        //              ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        //      );
        //  }
        // 
        //  is illegal! There is simply no way to legally get a reference to the underlying value.
        //  (cf. https://en.cppreference.com/w/cpp/language/static_cast) 
        //

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires BinaryAssignmentEnabled<T1, T2, impl::conjunction>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<BinaryAssignmentEnabled<T1, T2, impl::conjunction>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        constexpr T1&
            operator&=(T1& lhs, T2 rhs) noexcept {
            // comma operator used to only have a return statement in the function (required for C++11)
            return (lhs = lhs & rhs), lhs;
        }


#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires BinaryAssignmentEnabled<T1, T2, impl::disjunction>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<BinaryAssignmentEnabled<T1, T2, impl::disjunction>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        constexpr T1&
            operator|=(T1& lhs, T2 rhs) noexcept {
            // comma operator used to only have a return statement in the function (required for C++11)
            return (lhs = lhs | rhs), lhs;
        }


#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires BinaryAssignmentEnabled<T1, T2, impl::not_equal>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<BinaryAssignmentEnabled<T1, T2, impl::not_equal>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        constexpr T1&
            operator^=(T1& lhs, T2 rhs) noexcept {
            // comma operator used to only have a return statement in the function (required for C++11)
            return (lhs = lhs ^ rhs), lhs;
        }


#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsFlags<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsFlags<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            operator!(T e) noexcept {
            return !impl::get_underlying_impl(e);
        }

        // test for == 0 / != 0
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsFlags<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsFlags<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            operator==(T value, std::nullptr_t) noexcept {
            return impl::get_underlying_impl(value) == 0;
        }

#if !(BOOST_FLAGS_HAS_REWRITTEN_CANDIDATES)
        // no rewritten candidates

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsFlags<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsFlags<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            operator==(std::nullptr_t, T value) noexcept {
            return impl::get_underlying_impl(value) == 0;
        }

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsFlags<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsFlags<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            operator!=(T value, std::nullptr_t) noexcept {
            return !(impl::get_underlying_impl(value) == 0);
        }

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsFlags<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsFlags<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            operator!=(std::nullptr_t, T value) noexcept {
            return !(impl::get_underlying_impl(value) == 0);
        }
#endif // !(BOOST_FLAGS_HAS_REWRITTEN_CANDIDATES)


        // test for == 0 / != 0
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsFlags<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsFlags<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            operator==(T value, impl::null_tag) noexcept {
            return impl::get_underlying_impl(value) == 0;
        }

#if !(BOOST_FLAGS_HAS_REWRITTEN_CANDIDATES)
        // no rewritten candidates

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsFlags<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsFlags<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            operator==(impl::null_tag, T value) noexcept {
            return impl::get_underlying_impl(value) == 0;
        }

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsFlags<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsFlags<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            operator!=(T value, impl::null_tag) noexcept {
            return !(impl::get_underlying_impl(value) == 0);
        }

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsFlags<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsFlags<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            operator!=(impl::null_tag, T value) noexcept {
            return !(impl::get_underlying_impl(value) == 0);
        }
#endif // !(BOOST_FLAGS_HAS_REWRITTEN_CANDIDATES)




        // conversion to / from underlying type
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsEnabled<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsEnabled<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr auto
            get_underlying(T value) noexcept -> decltype(impl::get_underlying_impl(value)) {
            return impl::get_underlying_impl(value);
        }

#if BOOST_FLAGS_HAS_CONCEPTS

        template<typename T>
            requires IsEnabled<T>
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr auto
            from_underlying(typename std::underlying_type_t<enum_type_t<T>> value) noexcept {
            if constexpr (IsComplement<T>) {
                return complement{ static_cast<enum_type_t<T>>(value) };
            }
            else {
                return static_cast<enum_type_t<T>>(value);
            }
        }

#else // BOOST_FLAGS_HAS_CONCEPTS

        namespace impl {
            template<typename T,
                typename std::enable_if<!IsComplement<T>::value, int*>::type = nullptr >
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr auto
                from_underlying_impl(typename std::underlying_type<enum_type_t<T>>::type value) noexcept -> enum_type_t<T> {
                return static_cast<enum_type_t<T>>(value);
            }

            template<typename T,
                typename std::enable_if<IsComplement<T>::value, int*>::type = nullptr >
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr auto
                from_underlying_impl(typename std::underlying_type<enum_type_t<T>>::type value) noexcept -> complement<T> {
                return complement<T>{ static_cast<enum_type_t<T>>(value) };
            }
        }

        template<typename T,
            typename std::enable_if<IsEnabled<T>::value, int*>::type = nullptr >
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr auto
            from_underlying(typename std::underlying_type<enum_type_t<T>>::type value) noexcept -> decltype(impl::from_underlying_impl(value)) {
            return impl::from_underlying_impl(value);
        }

#endif // BOOST_FLAGS_HAS_CONCEPTS

        // delete operator== (and also !=) for comparison of incompatible types
        // only deleted for non-class enums
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires ((IsEnabled<T1> && !impl::is_scoped_enum<enum_type_t<T1>>::value)
        ||
            (IsEnabled<T2> && !impl::is_scoped_enum<enum_type_t<T2>>::value))
            && (!IsCompatibleFlagsOrComplement<T1, T2>)
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<((IsEnabled<T1>::value && !impl::is_scoped_enum<enum_type_t<T1>>::value)
                ||
                (IsEnabled<T2>::value && !impl::is_scoped_enum<enum_type_t<T2>>::value))
            && (!IsCompatibleFlagsOrComplement<T1, T2>::value), int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        constexpr bool operator== (T1, T2) = delete;

        // delete operator== (and also !=) for comparison of incompatible types
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires IsCompatibleFlagsOrComplement<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<IsCompatibleFlagsOrComplement<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool operator== (T1 e1, T2 e2) noexcept {
            return get_underlying(e1) == get_underlying(e2);
        }



                // logical operators
                // 
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires LogicalAndEnabled<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<LogicalAndEnabled<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        constexpr bool operator&& (T1 lhs, T2 rhs) {
            return !!(lhs & rhs);
        }


#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires ((IsEnabled<T1> || IsEnabled<T2>) && (!LogicalAndEnabled<T1, T2>) && impl::BothImplicitIntegralConvertible<T1,T2>)
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<(IsEnabled<T1>::value || IsEnabled<T2>::value) && (!LogicalAndEnabled<T1, T2>::value && impl::BothImplicitIntegralConvertible<T1, T2>::value), int*>::type = nullptr > // && (!IsCompatibleFlags<T1, T2>)
#endif // BOOST_FLAGS_HAS_CONCEPTS
        constexpr bool operator&& (T1, T2) = delete;



        // if at least one argument is enabled and both are integral-convertible
        // disable logical or (to avoid builtin operator ||)
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires ((IsEnabled<T1> || IsEnabled<T2>) && impl::BothImplicitIntegralConvertible<T1, T2>)
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<(IsEnabled<T1>::value || IsEnabled<T2>::value) && impl::BothImplicitIntegralConvertible<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        constexpr bool operator|| (T1, T2) = delete;

#if !(BOOST_FLAGS_HAS_PARTIAL_ORDERING)

        namespace impl {
            // implementation of partial order
            using compare_underlying_t = signed char;

            enum class compare_equal_enum : compare_underlying_t {
                equal = 0,
                equivalent = equal
            };

            enum class compare_ordered_enum : compare_underlying_t {
                less = -1,
                greater = 1
            };

            enum class compare_incomparable : compare_underlying_t {
                unordered = -2
                //                unordered = -128
            };

            struct zero_t
            {
                constexpr zero_t(zero_t*) noexcept { }
            };

            struct partial_ordering {

                BOOST_FLAGS_ATTRIBUTE_NODISCARD friend constexpr bool operator==(const partial_ordering lhs, zero_t) noexcept {
                    return lhs.value == 0;
                }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD friend constexpr bool operator==(partial_ordering lhs, partial_ordering rhs) noexcept {
                    return lhs.value == rhs.value;
                }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD friend constexpr bool operator<(const partial_ordering lhs, zero_t) noexcept {
                    return lhs.value == static_cast<compare_underlying_t>(compare_ordered_enum::less);
                }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD friend constexpr bool operator>(const partial_ordering lhs, zero_t) noexcept {
                    return lhs.value > 0;
                }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD friend constexpr bool operator<=(const partial_ordering lhs, zero_t) noexcept {
                    return (lhs < 0) || (lhs == 0);
                }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD friend constexpr bool operator>=(const partial_ordering lhs, zero_t) noexcept {
                    return lhs.value >= 0;
                }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD friend constexpr bool operator<(zero_t, const partial_ordering rhs) noexcept {
                    return rhs > 0;
                }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD friend constexpr bool operator>(zero_t, const partial_ordering rhs) noexcept {
                    return rhs < 0;
                }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD friend constexpr bool operator<=(zero_t, const partial_ordering rhs) noexcept {
                    return rhs >= 0;
                }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD friend constexpr bool operator>=(zero_t, const partial_ordering rhs) noexcept {
                    return rhs <= 0;
                }


                compare_underlying_t value;

                static const partial_ordering equivalent;
                static const partial_ordering less;
                static const partial_ordering greater;
                static const partial_ordering unordered;
            };

#if (BOOST_FLAGS_DEFINE_PARTIAL_ORDERING_OBJECTS)
            BOOST_FLAGS_WEAK_SYMBOL const partial_ordering partial_ordering::equivalent{ static_cast<compare_underlying_t>(compare_equal_enum::equivalent) };
            BOOST_FLAGS_WEAK_SYMBOL const partial_ordering partial_ordering::less{ static_cast<compare_underlying_t>(compare_ordered_enum::less) };
            BOOST_FLAGS_WEAK_SYMBOL const partial_ordering partial_ordering::greater{ static_cast<compare_underlying_t>(compare_ordered_enum::greater) };
            BOOST_FLAGS_WEAK_SYMBOL const partial_ordering partial_ordering::unordered{ static_cast<compare_underlying_t>(compare_incomparable::unordered) };
#endif // (BOOST_FLAGS_DEFINE_PARTIAL_ORDERING_OBJECTS)
        }

        // alias to partial_ordering
        using partial_ordering = impl::partial_ordering;

        namespace impl {
            template<typename T1, typename T2>
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr partial_ordering normalized_subset_induced_compare(T1 l, T2 r) noexcept {
                return l == r
                    ? partial_ordering::equivalent
                    : (l & r) == l
                    ? partial_ordering::less
                    : (l & r) == r
                    ? partial_ordering::greater
                    : partial_ordering::unordered
                    ;
            }

            template<typename T1, typename T2>
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr partial_ordering subset_induced_compare(T1 l, T2 r) noexcept {
                return normalized_subset_induced_compare(
                    get_normalized(l),
                    get_normalized(r)
                );
            }
        }

        struct partial_order_t {
#if BOOST_FLAGS_HAS_CONCEPTS
            template<typename T1, typename T2>
                requires IsCompatibleFlagsOrComplement<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
            template<typename T1, typename T2,
                typename std::enable_if<IsCompatibleFlagsOrComplement<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr auto operator()(T1 e1, T2 e2) const noexcept -> decltype(impl::subset_induced_compare(e1, e2)) {
                return impl::subset_induced_compare(e1, e2);
            }

            using is_transparent = int;
        };
        static constexpr partial_order_t partial_order{};

#else // !(BOOST_FLAGS_HAS_PARTIAL_ORDERING)

        // alias to partial_ordering
        using partial_ordering = std::partial_ordering;

        // disabling relational operators
        // 
        template<typename T1, typename T2>
            requires (IsEnabled<T1> || IsEnabled<T2>) && (!IsCompatibleFlagsOrComplement<T1, T2>)
        constexpr partial_ordering operator<=> (T1, T2) = delete;

        namespace impl {
            template<typename T1, typename T2>
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr partial_ordering normalized_subset_induced_compare(T1 l, T2 r) noexcept {
                return l == r
                    ? partial_ordering::equivalent
                    : (l & r) == l
                    ? partial_ordering::less
                    : (l & r) == r
                    ? partial_ordering::greater
                    : partial_ordering::unordered
                    ;
            }

            template<typename T1, typename T2>
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr partial_ordering subset_induced_compare(T1 l, T2 r) noexcept {
                return normalized_subset_induced_compare(
                    get_normalized(l),
                    get_normalized(r)
                );
            }
        }

        struct partial_order_t {
            template<typename T1, typename T2>
                requires IsCompatibleFlagsOrComplement<T1, T2>
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr auto operator()(T1 e1, T2 e2) const noexcept {
                return impl::subset_induced_compare(e1, e2);
            }

            using is_transparent = int;
        };
        static constexpr partial_order_t partial_order{};
#endif // !(BOOST_FLAGS_HAS_PARTIAL_ORDERING)



        // explicit total order for using enabled flags as (part of) keys in ordered associative containers

        struct total_order_t {
#if BOOST_FLAGS_HAS_CONCEPTS
            template<typename T1, typename T2>
                requires IsCompatibleFlagsOrComplement<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
            template<typename T1, typename T2,
                typename std::enable_if<IsCompatibleFlagsOrComplement<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr auto operator()(T1 e1, T2 e2) const noexcept -> decltype(get_underlying(e1) < get_underlying(e2)) {
                return get_underlying(e1) < get_underlying(e2);
            }

            using is_transparent = int;
        };
        static constexpr total_order_t total_order{};

        // end of core part
        //
        ///////////////////////////////////////////////////////////////////////////////////////


        // test if any bit is set
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsFlags<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsFlags<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            any(T e) noexcept {
            return impl::get_underlying_impl(e) != 0;
        }

        // test if no bit is set
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsFlags<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsFlags<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            none(T e) noexcept {
            return !e;
        }


        // test if subset is contained in superset
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<IsCompatibleFlags<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            subseteq(T1 subset, T2 superset) noexcept {
            return (subset & superset) == subset;
        }

        // test if subset is a proper subset of superset
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<IsCompatibleFlags<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            subset(T1 subset, T2 superset) noexcept {
            return subseteq(subset, superset) && (subset != superset);
        }


        // test if lhs and rhs have non-empty intersection (have at least one common flag)
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<IsCompatibleFlags<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            intersect(T1 lhs, T2 rhs) noexcept {
            return (impl::get_underlying_impl(lhs) & impl::get_underlying_impl(rhs)) != 0;
        }


        // test if lhs and rhs are disjoint (have no common flag)
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<IsCompatibleFlags<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            disjoint(T1 lhs, T2 rhs) noexcept {
            return (impl::get_underlying_impl(lhs) & impl::get_underlying_impl(rhs)) == 0;
        }


        // returns an empty instance of T
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsFlags<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsFlags<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr enum_type_t<T>
            make_null(T) noexcept {
            return static_cast<enum_type_t<T>>(0);
        }


        // depending on set
        // returns e or an empty instance of T
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsFlags<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsFlags<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr enum_type_t<T>
            make_if(T e, bool set) noexcept {
            return static_cast<enum_type_t<T>>(set ? impl::get_underlying_impl(e) : 0);
        }

        // return a copy of value with all
        // bits of modification set resp. cleared
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<IsCompatibleFlags<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr enum_type_t<T1>
            modify(T1 value, T2 modification, bool set) noexcept {
            return set ? (value | modification) : (value & ~modification);
        }

        // sets resp. clears the bits of modification
        // in value in-place
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<IsCompatibleFlags<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        constexpr T1&
            modify_inplace(T1& value, T2 modification, bool set) noexcept {
            // comma operator used to only have a return statement in the function (required for C++11)
            return (value = set ? (value | modification) : (value & ~modification)), value;
        }

        // return a copy of value with all
        // bits of modification set resp. cleared
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<IsCompatibleFlags<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr enum_type_t<T1>
            add_if(T1 value, T2 modification, bool add) noexcept {
            return add ? (value | modification) : value;
        }

        // sets resp. clears the bits of modification
        // in value in-place
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<IsCompatibleFlags<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        constexpr T1&
            add_if_inplace(T1& value, T2 modification, bool add) noexcept {
            // comma operator used to only have a return statement in the function (required for C++11)
            return (value = add ? (value | modification) : value), value;
        }

        // return a copy of value with all
        // bits of modification set resp. cleared
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<IsCompatibleFlags<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr enum_type_t<T1>
            remove_if(T1 value, T2 modification, bool remove) noexcept {
            return remove ? value & ~modification : value;
        }

        // sets resp. clears the bits of modification
        // in value in-place
#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires IsCompatibleFlags<T1, T2>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<IsCompatibleFlags<T1, T2>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        constexpr T1&
            remove_if_inplace(T1& value, T2 modification, bool remove) noexcept {
            // comma operator used to only have a return statement in the function (required for C++11)
            return (value = remove ? (value & ~modification) : value), value;
        }



#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T>
            requires IsEnabled<T>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T,
            typename std::enable_if<IsEnabled<T>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr impl::pseudo_and_op_intermediate_t<T>
            operator&(T lhs, impl::pseudo_and_op_tag) noexcept {
            return { lhs };
        }

#if BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2>
            requires LogicalOperationEnabled<T1, T2, impl::conjunction>
#else // BOOST_FLAGS_HAS_CONCEPTS
        template<typename T1, typename T2,
            typename std::enable_if<LogicalOperationEnabled<T1, T2, impl::conjunction>::value, int*>::type = nullptr >
#endif // BOOST_FLAGS_HAS_CONCEPTS
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr bool
            operator&(impl::pseudo_and_op_intermediate_t<T1> lhs, T2 rhs) noexcept {
            return (impl::get_underlying_impl(lhs.value) & impl::get_underlying_impl(rhs)) != 0;
        }


        namespace impl {
            template <typename T, bool B = std::is_enum<T>::value>
            struct underlying_or_identity {
                using type = T;
            };

            template <typename T>
            struct underlying_or_identity<T, true> {
                using type = typename std::underlying_type<T>::type;
            };
        }

        // returns a value with the n-th (zero-indexed) bit set
        template<typename T = int>
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            inline constexpr auto nth_bit(unsigned int n) noexcept -> typename impl::underlying_or_identity<T>::type {
            return static_cast<typename impl::underlying_or_identity<T>::type>(1) << n;
        }

        template<typename T>
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            inline constexpr auto next_bit(T n) noexcept -> T {
            return n << 1;
        }



        // bits-generator
        template<typename E>
        class flag_generator {
            E begin_{};
            E end_{};

            // flags-iterator
            class iterator {
                using iterator_category = std::forward_iterator_tag;
                using difference_type = int;
                using value_type = E;
                using pointer = E const*;
                using reference = E const&;
                E value_{};
            public:
                BOOST_FLAGS_ATTRIBUTE_NODISCARD_CTOR
                    explicit constexpr iterator(E pos) : value_{ pos } {}

                constexpr value_type operator*() const { return value_; }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD
                    // before C++14 constexpr member functions were implicitly const
#if defined(__cplusplus) && __cplusplus >= 201402L // (C++14)
                    constexpr
#endif // defined(__cplusplus) && __cplusplus >= 201402L // (C++14)
                    pointer operator->() { return &value_; }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD
                    // before C++14 constexpr member functions were implicitly const
#if defined(__cplusplus) && __cplusplus >= 201402L // (C++14)
                    constexpr
#endif // defined(__cplusplus) && __cplusplus >= 201402L // (C++14)
                    iterator& operator++() {
                    value_ = static_cast<E>(get_underlying(value_) << 1);
                    return *this;
                }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD
                    // before C++14 constexpr member functions were implicitly const
#if defined(__cplusplus) && __cplusplus >= 201402L // (C++14)
                    constexpr
#endif // defined(__cplusplus) && __cplusplus >= 201402L // (C++14)
                    iterator operator++(int) {
                    iterator tmp = *this;
                    ++(*this);
                    return tmp;
                }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD
                    friend constexpr bool operator==(iterator const& fir, iterator const& sec) {
                    return fir.value_ == sec.value_;
                }

                BOOST_FLAGS_ATTRIBUTE_NODISCARD
                    friend constexpr bool operator!=(iterator const& fir, iterator const& sec) {
                    return fir.value_ != sec.value_;
                }
            };


        public:
            BOOST_FLAGS_ATTRIBUTE_NODISCARD_CTOR
                constexpr flag_generator(E begin, E end) : begin_{ begin }, end_{ end } {}

            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr iterator begin() const { return iterator{ begin_ }; }
            BOOST_FLAGS_ATTRIBUTE_NODISCARD
                constexpr iterator end() const { return iterator{ end_ }; }
        };

        template<typename E>
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr flag_generator<E> flags_from_to(E first, E last) {
            return flag_generator<E>{first, static_cast<E>(get_underlying(last) << 1)};
        }

        template<typename E>
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr flag_generator<E> flags_to(E last) {
            return flags_from_to(E(1), last);
        }

        template<typename E>
        BOOST_FLAGS_ATTRIBUTE_NODISCARD
            constexpr flag_generator<E> flags_all() {
            return flag_generator<E>{E(1), E{}};
        }

        namespace impl {

            // the `options` versions for the options-detectors
            // the overloads for `options` will be picked up by ADL
            constexpr inline bool is_option_enable(options v) { return (v & options::enable) != 0; }
            constexpr inline bool is_option_disable_complement(options v) { return (v & options::disable_complement) != 0; }
            constexpr inline bool is_option_logical_and(options v) { return (v & options::logical_and) != 0; }

        }
        using impl::is_option_enable;
        using impl::is_option_disable_complement;
        using impl::is_option_logical_and;


    }
}


using boost::flags::operator|;
using boost::flags::operator&;
using boost::flags::operator^;
using boost::flags::operator~;
using boost::flags::operator|=;
using boost::flags::operator&=;
using boost::flags::operator^=;
using boost::flags::operator!;
using boost::flags::operator==;
#if !(BOOST_FLAGS_HAS_REWRITTEN_CANDIDATES)
using boost::flags::operator!=;
#endif // !(BOOST_FLAGS_HAS_REWRITTEN_CANDIDATES)

#if BOOST_FLAGS_HAS_PARTIAL_ORDERING
using boost::flags::operator<=>;
#endif

using boost::flags::operator&&;
using boost::flags::operator||;

using boost::flags::any;
using boost::flags::none;
using boost::flags::subseteq;
using boost::flags::subset;
using boost::flags::intersect;
using boost::flags::disjoint;
using boost::flags::make_null;
using boost::flags::make_if;
using boost::flags::modify;
using boost::flags::modify_inplace;
using boost::flags::add_if;
using boost::flags::add_if_inplace;
using boost::flags::remove_if;
using boost::flags::remove_if_inplace;
using boost::flags::get_underlying;
using boost::flags::from_underlying;



#define BOOST_FLAGS_SPECIALIZE_STD_LESS(E)                                              \
 /* specialize std::less for E and complement<E> */                                     \
 /* at least gcc < version 7 is not conforming with template specialization */          \
 /* put it explicitly into namespace std to make them all happy */                      \
namespace std {                                                                         \
    template<>                                                                          \
    struct less<E> {                                                                    \
        BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                 \
            constexpr bool operator()(E const& lhs, E const& rhs) const noexcept {      \
            return boost::flags::total_order(lhs, rhs);                                 \
        }                                                                               \
    };                                                                                  \
    template<>                                                                          \
    struct less<boost::flags::complement<E>> {                                          \
        BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                 \
            constexpr bool operator()(                                                  \
                boost::flags::complement<E> const& lhs,                                 \
                boost::flags::complement<E> const& rhs                                  \
                ) const noexcept {                                                      \
            return boost::flags::total_order(lhs, rhs);                                 \
        }                                                                               \
    };                                                                                  \
} /* namespace std */                                                               

#if !(BOOST_FLAGS_HAS_THREE_WAY_COMPARISON)

#if BOOST_FLAGS_HAS_CONCEPTS

#define BOOST_FLAGS_REL_OPS_DELETE(E)                                                   \
/* matches better than built-in relational operators */                                 \
bool operator< (E l, E r) = delete;                                                     \
bool operator<= (E l, E r) = delete;                                                    \
bool operator> (E l, E r) = delete;                                                     \
bool operator>= (E l, E r) = delete;                                                    \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2>                                                      \
    requires (std::is_same_v<E, boost::flags::enum_type_t<T1>> &&                       \
    std::is_same_v<E, boost::flags::enum_type_t<T2>>)                                   \
bool operator< (T1 l, T2 r) = delete;                                                   \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2>                                                      \
    requires (std::is_same_v<E, boost::flags::enum_type_t<T1>> &&                       \
    std::is_same_v<E, boost::flags::enum_type_t<T2>>)                                   \
bool operator<= (T1 l, T2 r) = delete;                                                  \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2>                                                      \
    requires (std::is_same_v<E, boost::flags::enum_type_t<T1>> &&                       \
    std::is_same_v<E, boost::flags::enum_type_t<T2>>)                                   \
bool operator> (T1 l, T2 r) = delete;                                                   \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2>                                                      \
    requires (std::is_same_v<E, boost::flags::enum_type_t<T1>> &&                       \
    std::is_same_v<E, boost::flags::enum_type_t<T2>>)                                   \
bool operator>= (T1 l, T2 r) = delete;                                                  \
                                                                                        \


#define BOOST_FLAGS_REL_OPS_PARTIAL_ORDER(E)                                            \
/* matches better than built-in relational operators */                                 \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator< (E l, E r) noexcept {                                          \
    return boost::flags::impl::normalized_subset_induced_compare(l, r) < 0;             \
}                                                                                       \
                                                                                        \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator<= (E l, E r) noexcept {                                         \
    return boost::flags::impl::normalized_subset_induced_compare(l, r) <= 0;            \
}                                                                                       \
                                                                                        \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator> (E l, E r) noexcept {                                          \
    return boost::flags::impl::normalized_subset_induced_compare(l, r) > 0;             \
}                                                                                       \
                                                                                        \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator>= (E l, E r) noexcept {                                         \
    return boost::flags::impl::normalized_subset_induced_compare(l, r) >= 0;            \
}                                                                                       \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2>                                                      \
    requires (std::is_same_v<E, boost::flags::enum_type_t<T1>> &&                       \
    std::is_same_v<E, boost::flags::enum_type_t<T2>> &&                                 \
    boost::flags::IsCompatibleFlagsOrComplement<T1, T2>)                                \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator< (T1 l, T2 r) noexcept {                                        \
    return boost::flags::impl::subset_induced_compare(l, r) < 0;                        \
}                                                                                       \
                                                                                        \
template<typename T1, typename T2>                                                      \
    requires (std::is_same_v<E, boost::flags::enum_type_t<T1>> &&                       \
    std::is_same_v<E, boost::flags::enum_type_t<T2>> &&                                 \
    boost::flags::IsCompatibleFlagsOrComplement<T1, T2>)                                \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator<= (T1 l, T2 r) noexcept {                                       \
    return boost::flags::impl::subset_induced_compare(l, r) <= 0;                       \
}                                                                                       \
                                                                                        \
template<typename T1, typename T2>                                                      \
    requires (std::is_same_v<E, boost::flags::enum_type_t<T1>> &&                       \
    std::is_same_v<E, boost::flags::enum_type_t<T2>> &&                                 \
    boost::flags::IsCompatibleFlagsOrComplement<T1, T2>)                                \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator> (T1 l, T2 r) noexcept {                                        \
    return boost::flags::impl::subset_induced_compare(l, r) > 0;                        \
}                                                                                       \
                                                                                        \
template<typename T1, typename T2>                                                      \
    requires (std::is_same_v<E, boost::flags::enum_type_t<T1>> &&                       \
    std::is_same_v<E, boost::flags::enum_type_t<T2>> &&                                 \
    boost::flags::IsCompatibleFlagsOrComplement<T1, T2>)                                \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator>= (T1 l, T2 r) noexcept {                                       \
    return boost::flags::impl::subset_induced_compare(l, r) >= 0;                       \
}                                                                                       \
                                                                                        \

#else // BOOST_FLAGS_HAS_CONCEPTS

#define BOOST_FLAGS_REL_OPS_DELETE(E)                                                   \
/* matches better than built-in relational operators */                                 \
bool operator< (E l, E r) = delete;                                                     \
bool operator<= (E l, E r) = delete;                                                    \
bool operator> (E l, E r) = delete;                                                     \
bool operator>= (E l, E r) = delete;                                                    \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2,                                                      \
    typename std::enable_if<std::is_same<E, boost::flags::enum_type_t<T1>>::value &&    \
    std::is_same<E, boost::flags::enum_type_t<T2>>::value, int*>::type = nullptr>       \
bool operator< (T1 l, T2 r) = delete;                                                   \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2,                                                      \
    typename std::enable_if<std::is_same<E, boost::flags::enum_type_t<T1>>::value &&    \
    std::is_same<E, boost::flags::enum_type_t<T2>>::value, int*>::type = nullptr>       \
bool operator<= (T1 l, T2 r) = delete;                                                  \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2,                                                      \
    typename std::enable_if<std::is_same<E, boost::flags::enum_type_t<T1>>::value &&    \
    std::is_same<E, boost::flags::enum_type_t<T2>>::value, int*>::type = nullptr>       \
bool operator> (T1 l, T2 r) = delete;                                                   \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2,                                                      \
    typename std::enable_if<std::is_same<E, boost::flags::enum_type_t<T1>>::value &&    \
    std::is_same<E, boost::flags::enum_type_t<T2>>::value, int*>::type = nullptr>       \
bool operator>= (T1 l, T2 r) = delete;                                                  \
                                                                                        \


// disable for VS 2015 (and earlier) due to ambiguity-bug
// error C2593 : 'operator <' is ambiguous
// note: could be 'bool operator <(relops_partial_order_enum,relops_partial_order_enum) noexcept'
// note: or 'bool operator <(relops_delete_enum,relops_delete_enum)'
// note: or 'bool operator <<relops_partial_order_enum,relops_partial_order_enum,0x0>(T1,T2) noexcept'
#if defined(_MSC_VER) && _MSC_VER <= 1900

// sorry, not supported
#define BOOST_FLAGS_REL_OPS_PARTIAL_ORDER(E) static_assert(false, "not supported for MSVC v140 or earlier");

#else // defined(_MSC_VER) && _MSC_VER <= 1900

#define BOOST_FLAGS_REL_OPS_PARTIAL_ORDER(E)                                            \
/* matches better than built-in relational operators */                                 \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator< (E l, E r) noexcept {                                          \
    return boost::flags::impl::normalized_subset_induced_compare(l, r) < 0;             \
}                                                                                       \
                                                                                        \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator<= (E l, E r) noexcept {                                         \
    return boost::flags::impl::normalized_subset_induced_compare(l, r) <= 0;            \
}                                                                                       \
                                                                                        \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator> (E l, E r) noexcept {                                          \
    return boost::flags::impl::normalized_subset_induced_compare(l, r) > 0;             \
}                                                                                       \
                                                                                        \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator>= (E l, E r) noexcept {                                         \
    return boost::flags::impl::normalized_subset_induced_compare(l, r) >= 0;            \
}                                                                                       \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2,                                                      \
    typename std::enable_if<std::is_same<E, boost::flags::enum_type_t<T1>>::value &&    \
    std::is_same<E, boost::flags::enum_type_t<T2>>::value &&                            \
    boost::flags::IsCompatibleFlagsOrComplement<T1, T2>::value, int*>::type = nullptr>  \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator< (T1 l, T2 r) noexcept {                                        \
    return boost::flags::impl::subset_induced_compare(l, r) < 0;                        \
}                                                                                       \
                                                                                        \
template<typename T1, typename T2,                                                      \
    typename std::enable_if<std::is_same<E, boost::flags::enum_type_t<T1>>::value &&    \
    std::is_same<E, boost::flags::enum_type_t<T2>>::value &&                            \
    boost::flags::IsCompatibleFlagsOrComplement<T1, T2>::value &&                       \
    !(std::is_same<E, T1>::value && std::is_same<E, T2>::value), int*>::type = nullptr> \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator<= (T1 l, T2 r) noexcept {                                       \
    return boost::flags::impl::subset_induced_compare(l, r) <= 0;                       \
}                                                                                       \
                                                                                        \
template<typename T1, typename T2,                                                      \
    typename std::enable_if<std::is_same<E, boost::flags::enum_type_t<T1>>::value &&    \
    std::is_same<E, boost::flags::enum_type_t<T2>>::value &&                            \
    boost::flags::IsCompatibleFlagsOrComplement<T1, T2>::value &&                       \
    !(std::is_same<E, T1>::value && std::is_same<E, T2>::value), int*>::type = nullptr> \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator> (T1 l, T2 r) noexcept {                                        \
    return boost::flags::impl::subset_induced_compare(l, r) > 0;                        \
}                                                                                       \
                                                                                        \
template<typename T1, typename T2,                                                      \
    typename std::enable_if<std::is_same<E, boost::flags::enum_type_t<T1>>::value &&    \
    std::is_same<E, boost::flags::enum_type_t<T2>>::value &&                            \
    boost::flags::IsCompatibleFlagsOrComplement<T1, T2>::value &&                       \
    !(std::is_same<E, T1>::value && std::is_same<E, T2>::value), int*>::type = nullptr> \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr bool operator>= (T1 l, T2 r) noexcept {                                       \
    return boost::flags::impl::subset_induced_compare(l, r) >= 0;                       \
}                                                                                       \


#endif //  // defined(_MSC_VER) && _MSC_VER <= 1900

#endif // BOOST_FLAGS_HAS_CONCEPTS


#else // !(BOOST_FLAGS_HAS_THREE_WAY_COMPARISON)

#if BOOST_FLAGS_HAS_CONCEPTS

#define BOOST_FLAGS_REL_OPS_DELETE(E)                                                   \
/* matches better than built-in relational operators */                                 \
std::partial_ordering operator<=> (E l, E r) = delete;                                  \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2>                                                      \
    requires (std::is_same_v<E, boost::flags::enum_type_t<T1>> &&                       \
              std::is_same_v<E, boost::flags::enum_type_t<T2>> )                        \
std::partial_ordering operator<=> (T1 l, T2 r) = delete;


#define BOOST_FLAGS_REL_OPS_PARTIAL_ORDER(E)                                            \
/* matches better than built-in relational operators */                                 \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr std::partial_ordering operator<=> (E l, E r) noexcept {                       \
    return boost::flags::impl::normalized_subset_induced_compare(l, r);                 \
}                                                                                       \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2>                                                      \
    requires (std::is_same_v<E, boost::flags::enum_type_t<T1>> &&                       \
              std::is_same_v<E, boost::flags::enum_type_t<T2>> &&                       \
                boost::flags::IsCompatibleFlagsOrComplement<T1, T2>)                    \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr std::partial_ordering operator<=> (T1 l, T2 r) noexcept {                     \
    return boost::flags::impl::subset_induced_compare(l, r);                            \
}

#else // BOOST_FLAGS_HAS_CONCEPTS

#define BOOST_FLAGS_REL_OPS_DELETE(E)                                                   \
/* matches better than built-in relational operators */                                 \
std::partial_ordering operator<=> (E l, E r) = delete;                                  \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2,                                                      \
    typename std::enable_if<std::is_same<E, boost::flags::enum_type_t<T1>>::value &&    \
    std::is_same<E, boost::flags::enum_type_t<T2>>::value, int*>::type = nullptr >      \
std::partial_ordering operator<=> (T1 l, T2 r) = delete;


#define BOOST_FLAGS_REL_OPS_PARTIAL_ORDER(E)                                            \
/* matches better than built-in relational operators */                                 \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr auto operator<=> (E l, E r) noexcept                                          \
    -> decltype(boost::flags::impl::normalized_subset_induced_compare(l, r)) {          \
    return boost::flags::impl::normalized_subset_induced_compare(l, r);                 \
}                                                                                       \
                                                                                        \
/* matches all other E, complement<E> arguments */                                      \
template<typename T1, typename T2,                                                      \
    typename std::enable_if<std::is_same<E, boost::flags::enum_type_t<T1>>::value &&    \
    std::is_same<E, boost::flags::enum_type_t<T2>>::value &&                            \
    boost::flags::IsCompatibleFlagsOrComplement<T1, T2>::value, int*>::type = nullptr > \
BOOST_FLAGS_ATTRIBUTE_NODISCARD                                                         \
constexpr auto operator<=> (T1 l, T2 r) noexcept                                        \
    -> decltype(boost::flags::impl::subset_induced_compare(l, r)) {                     \
    return boost::flags::impl::subset_induced_compare(l, r);                            \
}

#endif // BOOST_FLAGS_HAS_CONCEPTS



#endif // !(BOOST_FLAGS_HAS_THREE_WAY_COMPARISON)


#define BOOST_FLAGS_PSEUDO_AND_OPERATOR & boost::flags::impl::pseudo_and_op_tag{} &

#define BOOST_FLAGS_AND  BOOST_FLAGS_PSEUDO_AND_OPERATOR

#define BOOST_FLAGS_NULL boost::flags::impl::null_tag{}


#endif  // BOOST_FLAGS_HPP_INCLUDED

