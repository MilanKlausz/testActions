#include "RandUtils/Rand.hh"
#include "DSFMT/dSFMT.h"
//#include "Core/Types.hh"
#include <stdexcept>

RandUtils::Rand::Rand(uint64_t seed)
  : m_state(0)
{
  int err=posix_memalign((void**)&m_state,64,sizeof(dsfmt_t));
  if(err!=0)
    throw std::runtime_error("RandUtils::Rand Failed to get aligned memory");
  dsfmt_init_by_array(m_state, reinterpret_cast<uint32_t*>(&seed), 2);
  for( int i=0; i < 20000; ++i ) shoot();      // Warm up just a bit
}

RandUtils::Rand::~Rand()
{
  free(m_state);
}
