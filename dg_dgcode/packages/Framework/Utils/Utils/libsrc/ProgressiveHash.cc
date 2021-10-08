#include "Utils/ProgressiveHash.hh"
#include "Core/Assert.hh"
#include "PMurHash.hh"

ProgressiveHash::~ProgressiveHash()
{
  dg_static_assert(sizeof(uint32_t)==sizeof(MH_UINT32));
}

void ProgressiveHash::addData(const char* data, unsigned len)
{
  m_data_length += len;
  PMurHash32_Process(&m_hash_state, &m_hash_carry, data, len);
}

uint32_t ProgressiveHash::getHash() const
{
  return PMurHash32_Result(m_hash_state,m_hash_carry, m_data_length);
}
