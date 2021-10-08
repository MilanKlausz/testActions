#ifndef PROJECTS_DOPPLER_LIBINC_DOPPLEREFFECT_HH_
#define PROJECTS_DOPPLER_LIBINC_DOPPLEREFFECT_HH_

#include <vector>
#include <map>
#include <string>
class PhysicsVector;
class DopplerEffect {
  struct Sigma1Function {
      double Ak;
      double Bk;
      double Ck;
  } ;

  typedef std::vector<Sigma1Function> Sigma1FunctionVector;



public:
  //temp in K, en in MeV
  DopplerEffect(std::string name, double massRatio, double temp_in_K, double* en_in_MeV, double* val, unsigned n );
  virtual ~DopplerEffect();

  PhysicsVector broaden(double aT);
  PhysicsVector* broadenThickening(double aT, double tolerance=0.001);

  double getCrossSection (double aT, double en_in_MeV);


private:

  double erfc_lookup(double a);
  double exp_a2_lookup(double a);



  void F_func_a_2_inf(double a, double *factors);
  void F_func_0_2_a(double a, double *factors);


  double BroadenOnePoint(double en, double aT, std::vector<double> &x_boundaries, Sigma1FunctionVector &sfVec);

  void FindIntegralRange(double En, double aT, unsigned &begin, unsigned &end);



  double BoundaryConditionOneOverV(double y, double aT, double e1, double sigma1);

  double HFuncFirstTerm(double y, Sigma1Function &sf, double x_k, double x_kp1);
  double HFuncSecondTerm(double y, Sigma1Function &sf, double x_k, double x_kp1);


  double m_mass_ratio;
  double m_neutronMass;
  PhysicsVector *m_coldxs;
  double k_Boltzmann;
  double m_temp;

  double *erfcArray;
  double *expArray_a2;

  double *buf1;
  double *buf2;

  std::string m_name;
  std::map<double, PhysicsVector* > m_CrossSectionMap;




};

#endif
