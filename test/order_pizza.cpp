
// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#define TEST_NAMESPACE order_pizza
#include "include_test.hpp"

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>

#include "include_test_post.hpp"


#if defined(TEST_FLAGS_LINKING)
namespace TEST_NAMESPACE {
#endif // defined(TEST_FLAGS_LINKING)

enum class pizza_toppings : unsigned int {
    tomato       = boost::flags::nth_bit(0), // == 0x01
    cheese       = boost::flags::nth_bit(1), // == 0x02
    salami       = boost::flags::nth_bit(2), // == 0x04
    olives       = boost::flags::nth_bit(3), // == 0x08
    garlic       = boost::flags::nth_bit(4), // == 0x10

    all_toppings = tomato | cheese | salami | olives | garlic,
};

// enable Boost.Flags for pizza_toppings
BOOST_FLAGS_ENABLE(pizza_toppings)


enum class ice_cream_flavours : unsigned int {
    vanilla      = boost::flags::nth_bit(0), // == 0x01
    chocolate    = boost::flags::nth_bit(1), // == 0x02
    strawberry   = boost::flags::nth_bit(2), // == 0x04
};

// enable Boost.Flags for ice_cream_flavours
BOOST_FLAGS_ENABLE(ice_cream_flavours)


void order_pizza(pizza_toppings /*toppings*/) { 
    // order selected pizza
 }

void order_ice_cream(ice_cream_flavours /*flavours*/) { 
    // order selected dessert
 }

int main() {
    report_config();
    pizza_toppings toppings = pizza_toppings::tomato | pizza_toppings::cheese; // a decent start
    toppings |= pizza_toppings::salami | pizza_toppings::olives;    // even better
    order_pizza(toppings);                              // order main course
    order_pizza(toppings & ~pizza_toppings::salami);    // order a vegetarian pizza
    order_ice_cream(ice_cream_flavours::vanilla);       // order desert

#if defined(TEST_COMPILE_FAIL_ORDER_WITH_COMPLEMENT)
    // Guest: "Pizza without olives!"
    // Waiter: "Ok, no olives. But what else to put on it?"
    // error: negative mask is not a pizza topping
    order_pizza(~pizza_toppings::olives);
#endif // defined(TEST_COMPILE_FAIL_ORDER_WITH_COMPLEMENT)

    // Guest: "Pizza with all toppings but olives!"
    // Waiter: "Ok, got it!"
    // Waiter takes note: Pizza with tomato, cheese, salami, garlic.
    order_pizza(pizza_toppings::all_toppings & ~pizza_toppings::olives);

#if defined(TEST_COMPILE_FAIL_MIX_INCOMPATIBLE)
    // error: mixing different enumerations
    toppings |= ice_cream_flavours::strawberry;
#endif // defined(TEST_COMPILE_FAIL_MIX_INCOMPATIBLE)

#if defined(TEST_COMPILE_FAIL_ORDER_WRONG_DESSERT)
    // error: called with wrong enumeration
    order_ice_cream(toppings);
#endif // defined(TEST_COMPILE_FAIL_ORDER_WRONG_DESSERT)
    BOOST_TEST(ice_cream_flavours::vanilla == ice_cream_flavours::vanilla);

    return boost::report_errors();
}

#if defined(TEST_FLAGS_LINKING)
} // namespace TEST_NAMESPACE
#endif // defined(TEST_FLAGS_LINKING)
