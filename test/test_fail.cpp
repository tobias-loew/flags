
// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/core/lightweight_test_trait.hpp>

#if defined(TEST_COMPILE_FAIL)


char not_a_function;
int i = not_a_function();

#endif // defined(TEST_COMPILE_FAIL)

int main() {
    
   BOOST_TEST(1+1== 2);

    return boost::report_errors();
}