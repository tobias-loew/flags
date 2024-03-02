// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#define BOOST_FLAGS_DEFINE_PARTIAL_ORDERING_OBJECTS 1

#define TEST_NAMESPACE 
#include "include_test.hpp"

#include <boost/core/lightweight_test_trait.hpp>
#include <boost/flags.hpp>

namespace test_basics {
    int main();
}

namespace test_disable_complement {
    int main();
}

namespace test_rel_ops {
    int main();
}

namespace test_utilities {
    int main();
}

namespace order_pizza {
    int main();
}


int main() {
    test_basics::main();
    test_disable_complement::main();
    test_rel_ops::main();
    test_utilities::main();
    order_pizza::main();

    return boost::report_errors();
}

