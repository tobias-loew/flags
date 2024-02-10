// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>

enum class pizza_toppings {
    tomato       = boost::flags::nth_bit(0), // == 0x01
    cheese       = boost::flags::nth_bit(1), // == 0x02
    salami       = boost::flags::nth_bit(2), // == 0x04
    olives       = boost::flags::nth_bit(3), // == 0x08

    all_toppings = tomato | cheese | salami | olives,
};
// enable Boost.Flags for pizza_toppings
template<> struct boost::flags::enable<pizza_toppings> : std::true_type {};

enum class ice_cream_flavours {
    vanilla      = boost::flags::nth_bit(0), // == 0x01
    chocolate    = boost::flags::nth_bit(1), // == 0x02
    strawberry   = boost::flags::nth_bit(2), // == 0x04
};
// enable Boost.Flags for ice_cream_flavours
template<> struct boost::flags::enable<ice_cream_flavours> : std::true_type {};

void order_pizza(pizza_toppings /*toppings*/) { 
    // order selected pizza
 }

void order_ice_cream(ice_cream_flavours /*flavours*/) { 
    // order selected dessert
 }

int main() {
    pizza_toppings toppings = pizza_toppings::tomato | pizza_toppings::cheese; // a decent start
    toppings |= pizza_toppings::salami | pizza_toppings::olives;    // even better
    order_pizza(toppings);                              // order main course
    order_pizza(toppings & ~pizza_toppings::salami);    // order a vegetarian pizza
    order_ice_cream(ice_cream_flavours::vanilla);       // order desert

#ifdef TEST_COMPILE_FAIL_ORDER_WITH_COMPLEMENT
    // Guest: "Pizza without olives!"
    // Waiter: "Ok, no olives. But what else to put on it?"
    // error: negative mask is not a pizza topping
    order_pizza(~pizza_toppings::olives);
#endif

    // Guest: "Pizza with all toppings but olives!"
    // Waiter: "Ok, got it!"
    order_pizza(pizza_toppings::all_toppings & ~pizza_toppings::olives);

#ifdef TEST_COMPILE_FAIL_MIX_INCOMPATIBLE
    // error: mixing different enumerations
    toppings |= ice_cream_flavours::strawberry;
#endif

#ifdef TEST_COMPILE_FAIL_ORDER_WRONG_DESSERT
    // error: called with wrong enumeration
    order_ice_cream(toppings);
#endif
    BOOST_TEST(ice_cream_flavours::vanilla == ice_cream_flavours::vanilla);

    return boost::report_errors();
}
