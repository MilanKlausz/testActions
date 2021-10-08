#ifndef Utils_tempfixbadcxx11_hh
#define Utils_tempfixbadcxx11_hh

//Attempt to fix some parts of incomplete C++11 support on some old platforms
//(like slc6 with gcc 4.4). This header is destined for removal one day.

#include "Core/_tempdetectbadcxx11.hh"

#ifdef NEED_CXX11_WORKAROUNDS

#include <cmath>
#include <memory>
#include <limits>
#include <dgboost/shared_ptr.hpp>

#define final
#define constexpr

#ifdef __GNUC__
#  define nullptr __null
#else
#  define nullptr 0
#endif

namespace std {

  inline bool isnan(double x) { return ::isnan(x); }
  inline bool isinf(double x) { return ::isinf(x); }
  inline bool isfinite(double x) { return ! ( isnan(x) || isinf(x) ); }
  inline bool isnormal(double x)
  {
    return x!=0 && !isnan(x) && !isinf(x)
      && !(std::fabs( x ) < std::numeric_limits<double>::min());
  }

  template <class T>
  struct shared_ptr_hack : dgboost::shared_ptr<T> {
    shared_ptr_hack() : dgboost::shared_ptr<T>() {}
    shared_ptr_hack(T*t) : dgboost::shared_ptr<T>(t) {}
    template <class TOther>
    shared_ptr_hack(const shared_ptr_hack<TOther>& o) : dgboost::shared_ptr<T>() { *this = o; }
    template <class TOther>
    shared_ptr_hack& operator=(const shared_ptr_hack<TOther>& o)
    {
      dgboost::shared_ptr<T> * boost_this = static_cast<dgboost::shared_ptr<T> *>(this);
      const dgboost::shared_ptr<TOther> * boost_o = static_cast<const dgboost::shared_ptr<TOther> *>(&o);
      *boost_this = *boost_o;
      return *this;
    }
  };

  template< class T >
  shared_ptr_hack<T> make_shared_hack( ) { return shared_ptr_hack< T >(new T()); }
  template< class T, class A1 >
  shared_ptr_hack<T> make_shared_hack( A1 const & a1 ) { return shared_ptr_hack< T >(new T(a1)); }
  template< class T, class A1 >
  shared_ptr_hack<T> make_shared_hack( const A1*& a1 ) { return shared_ptr_hack< T >(new T(a1)); }
  template< class T, class A1, class A2 >
  shared_ptr_hack<T> make_shared_hack( A1 const & a1, A2 const & a2 ) { return shared_ptr_hack< T >(new T(a1,a2)); }
  template< class T, class A1, class A2 >
  shared_ptr_hack<T> make_shared_hack( A1 const & a1, const A2*& a2 ) { return shared_ptr_hack< T >(new T(a1,a2)); }

#define make_shared make_shared_hack
#define shared_ptr shared_ptr_hack
#define static_assert(x,y)

}


#endif

#endif
