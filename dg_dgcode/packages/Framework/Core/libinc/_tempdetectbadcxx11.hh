#ifndef Utils_tempdetectbadcxx11_hh
#define Utils_tempdetectbadcxx11_hh

//Detect incomplete C++11 support on some old platforms (like slc6
//with gcc 4.4), so client code can apply ifdefs as needed. This
//header is destined for removal one day.

#ifdef __clang__
#  if (__clang_major__ * 100 + __clang_minor__ < 304)
#    define NEED_CXX11_WORKAROUNDS
#  endif
#else
#  ifdef __GNUC__
#    if (__GNUC__ * 100 + __GNUC_MINOR__) < 408
#      define NEED_CXX11_WORKAROUNDS
#    endif
#  endif
#endif

#endif
