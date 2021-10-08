#ifndef RandUtils_Rand_hh
#define RandUtils_Rand_hh

#include "Core/Types.hh"
#include "DSFMT/dSFMT.h"

namespace RandUtils {

  class Rand {
  public:
    Rand(uint64_t seed = 123456789);
    ~Rand();

    double shoot();//Uniformly over 0..1
    uint32_t shootUInt32();

  private:
    dsfmt_t * m_state;
  };

}

#include "RandUtils/Rand.icc"

#endif
