#ifndef PhysicsVector_hh
#define PhysicsVector_hh

#include <cmath>
#include <vector>

class PhysicsVector  {
  public:
  PhysicsVector(double* en, double* val, unsigned n);
  ~PhysicsVector();
  double interpolate(double en);
  void getEnergyValuePair(unsigned idx, double &en, double &val) const;
  unsigned getEnUpperBound(double en);
  unsigned getSize() const {return m_size;};
  double energy(unsigned idx) const;
  double value(unsigned idx) const;
  std::vector<double>& getEnergy() {return m_energy;}
  void scaleVector(double forEn, double forVal);
  void save(const char* filename);
  private:
  std::vector<double> m_energy;
  std::vector<double> m_value;
  unsigned m_size;
};



#endif
