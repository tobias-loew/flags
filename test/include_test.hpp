// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#if !defined(TEST_NO_CXX14_DECLTYPE_AUTO)
# if !defined(__cpp_decltype_auto) || (__cpp_decltype_auto < 201304)
#  define TEST_NO_CXX14_DECLTYPE_AUTO
# endif
#endif


#ifndef TEST_NAMESPACE
#error TEST_NAMESPACE not defined!
#endif

#if defined(TEST_FLAGS_LINKING)
# define TEST_FLAGS_LINKING_PREAMBLE }
# define TEST_FLAGS_LINKING_EPILOGUE namespace TEST_NAMESPACE {
# define TEST_FLAGS_LINKING_NAMESPACE TEST_NAMESPACE::
#else // TEST_FLAGS_LINKING
# define TEST_FLAGS_LINKING_PREAMBLE
# define TEST_FLAGS_LINKING_EPILOGUE
# define TEST_FLAGS_LINKING_NAMESPACE
#endif // TEST_FLAGS_LINKING


#if defined(TEST_FLAGS_LINKING)
# if defined(TEST_FLAGS_LINKING_GCC_MINGW) && !defined(BOOST_FLAGS_DEFINE_PARTIAL_ORDERING_OBJECTS)
#  define BOOST_FLAGS_DEFINE_PARTIAL_ORDERING_OBJECTS 0
# endif
#endif
