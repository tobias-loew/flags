
// Copyright 2024, 2025 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <iostream>
#include <boost/flags.hpp>

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

// order selected pizza
void order_pizza(pizza_toppings toppings) {
    std::cout << "Pizza ordered with\n";
    if (boost::flags::any(toppings & pizza_toppings::tomato)) {
        std::cout << "- tomato\n";
    }
    if (boost::flags::any(toppings & pizza_toppings::cheese)) {
        std::cout << "- cheese\n";
    }
    if (boost::flags::any(toppings & pizza_toppings::salami)) {
        std::cout << "- salami\n";
    }
    if (boost::flags::any(toppings & pizza_toppings::olives)) {
        std::cout << "- olives\n";
    }
    if (boost::flags::any(toppings & pizza_toppings::garlic)) {
        std::cout << "- garlic\n";
    }
    std::cout << "\n";
}

// order selected dessert
void order_ice_cream(ice_cream_flavours flavours) {
    std::cout << "Ice cream ordered with flavours\n";
    if (boost::flags::any(flavours & ice_cream_flavours::vanilla)) {
        std::cout << "- vanilla\n";
    }
    if (boost::flags::any(flavours & ice_cream_flavours::chocolate)) {
        std::cout << "- chocolate\n";
    }
    if (boost::flags::any(flavours & ice_cream_flavours::strawberry)) {
        std::cout << "- strawberry\n";
    }
    std::cout << "\n";
}

int main() {
    pizza_toppings toppings = pizza_toppings::tomato | pizza_toppings::cheese; // a decent start
    toppings |= pizza_toppings::salami | pizza_toppings::olives;    // even better
    order_pizza(toppings);                              // order main course
    order_pizza(toppings & ~pizza_toppings::salami);    // order a vegetarian pizza
    order_ice_cream(ice_cream_flavours::vanilla);       // order desert

    // Guest: "Pizza without olives!"
    // Waiter: "Ok, no olives. But what else to put on it?"
    // error: negative mask is not a pizza topping
 //   order_pizza(~pizza_toppings::olives);

    // Guest: "Pizza with all toppings but olives!"
    // Waiter: "Ok, got it!"
    // Waiter takes note: Pizza with tomato, cheese, salami, garlic.
    order_pizza(pizza_toppings::all_toppings & ~pizza_toppings::olives);

    // error: mixing different enumerations
//    toppings |= ice_cream_flavours::strawberry;

    // error: called with wrong enumeration
//    order_ice_cream(toppings);
}
