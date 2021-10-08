#ifndef Core_Assert_hh
#define Core_Assert_hh

// Provides the following static (i.e. compile time) assert:
//
// dg_static_assert(sizeof(double)==8);
//
// And also the usual runtime assert from the <cassert> header:
//
// assert(x!=0)

//NB: BOOST_STATIC_ASSERT_MSG does not work on SLC6 so do not use it!

#include <dgboost/static_assert.hpp>
#include <cassert>
#define dg_static_assert(expr) BOOST_STATIC_ASSERT(expr)
#define dg_assert(expr) assert(expr)

//Future note: With a more modern boost than in SLC6 we could fancy stuff like:
//#define dg_static_assert_1(expr) BOOST_STATIC_ASSERT(expr)
//#define dg_static_assert_2(expr,msg) BOOST_STATIC_ASSERT(expr,msg)
//#define dg_static_assert(...) BOOST_PP_OVERLOAD(static_assert_,__VA_ARGS__)(__VA_ARGS__)
//And thus support overloading like:
// static_assert(sizeof(double)==8,"unexpected double size");
// static_assert(sizeof(double)==8);

#endif
