#include "FHPDoppler/PhysicsVector.hh"
#include <algorithm>
#include <functional>
#include <stdexcept>

PhysicsVector::PhysicsVector(double* en, double* val, unsigned n)
:m_size(n)
{
  if(n==0)
    throw std::runtime_error("PhysicsVector::PhysicsVector: vector size is 0");
  m_energy.resize(n);
  std::copy(en, en+n, m_energy.begin());
  m_value.resize(n);
  std::copy(val, val+n, m_value.begin());
}

PhysicsVector::~PhysicsVector()
{

}

double PhysicsVector::interpolate(double en)
{
  if(en<=m_energy[0]) //FIXME show warning message
    return m_value[0];
  else if(en>=m_energy.back()) //FIXME show warning message
    return m_value.back();
  else  {
    unsigned idx = std::upper_bound(m_energy.begin(), m_energy.end(), en) - m_energy.begin();
    double f = (en-m_energy[idx-1])/(m_energy[idx]-m_energy[idx-1]);
    return m_value[idx-1] + f*(m_value[idx] -m_value[idx-1] );
  }
}

void PhysicsVector::getEnergyValuePair(unsigned idx, double &en, double &val) const
{
  if(idx>=m_energy.size())
    throw std::runtime_error("PhysicsVector::getEnergyValuePair: index outside range");
  en=m_energy[idx];
  val=m_value[idx];
}

unsigned PhysicsVector::getEnUpperBound(double en)
{
  return std::upper_bound(m_energy.begin(), m_energy.end(), en) - m_energy.begin();
}

#include <iostream>
#include <fstream>

void PhysicsVector::save(const char* filename)//Fixme should be saved in the NCrystal data format
{
  std::ofstream f;
  f.open (filename);
  f << "\n\n\n";
  for(unsigned i=0;i<m_size;i++)  {
    f << m_energy[i] << " " << m_value[i] << "\n";
  }
  f.close();
}

double PhysicsVector::energy(unsigned idx) const
{
  if(idx>=m_size)
    throw std::runtime_error("PhysicsVector::energy: index outside range");
  return m_energy[idx];
}

double PhysicsVector::value(unsigned idx) const
{
  if(idx>=m_size)
    throw std::runtime_error("PhysicsVector::value: index outside range");
  return m_value[idx];
}


void PhysicsVector::scaleVector(double forEn, double forVal)
{
  for (auto& e: m_energy)
    e *= forEn;
  for (auto& e: m_value)
    e *= forVal;
}
