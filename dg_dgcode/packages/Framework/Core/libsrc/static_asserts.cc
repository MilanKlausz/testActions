#include "Core/Assert.hh"
#include "Core/Types.hh"

void _core_asserts() {
  //make sure we are not on some unusual platform
  dg_static_assert(CHAR_BIT==8);
  dg_static_assert(sizeof(float)==4);
  dg_static_assert(sizeof(double)==8);
  dg_static_assert(__DBL_DIG__==15);
  dg_static_assert(__DBL_MANT_DIG__==53);
  dg_static_assert(__DBL_MAX_10_EXP__==308);
  dg_static_assert(__DBL_MAX_EXP__==1024);
  dg_static_assert(__DBL_MIN_10_EXP__==-307);
  dg_static_assert(__DBL_MIN_EXP__==-1021);
}

