// Copyright 2024 Tobias Loew.
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#ifndef TEST_NAMESPACE
#error TEST_NAMESPACE not defined!
#endif

// 
// There exists a very annoying error in g++ with verion < 7:
// 
// Explicit specialization in a namespace enclosing the specialized template
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480
//
// The following macros are used to keep the "enable"-specialization compact
// 

#if defined(__GNUC__) && !defined(__clang__) && (__GNUC__ < 10)

# ifdef TEST_FLAGS_LINKING
#  define TEST_GNU_ERROR_WORKAROUND_PREAMBLE }
#  define TEST_GNU_ERROR_WORKAROUND_EPILOGUE namespace TEST_NAMESPACE {
#  define TEST_GNU_ERROR_WORKAROUND_NAMESPACE TEST_NAMESPACE::
# else // TEST_FLAGS_LINKING
#  define TEST_GNU_ERROR_WORKAROUND_PREAMBLE 
#  define TEST_GNU_ERROR_WORKAROUND_EPILOGUE
#  define   
# endif // TEST_FLAGS_LINKING

#else

# define TEST_GNU_ERROR_WORKAROUND_PREAMBLE
# define TEST_GNU_ERROR_WORKAROUND_EPILOGUE
# define TEST_GNU_ERROR_WORKAROUND_NAMESPACE

#endif
