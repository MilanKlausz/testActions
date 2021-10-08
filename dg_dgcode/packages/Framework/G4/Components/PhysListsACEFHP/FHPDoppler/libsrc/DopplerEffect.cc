#include "FHPDoppler/DopplerEffect.hh"
#include "FHPDoppler/PhysicsVector.hh"
#include <cmath>
#include <map>
#include <algorithm>
#include <stdio.h>
#include <limits>
#include <stdexcept>

DopplerEffect::DopplerEffect(std::string name, double massRatio, double temp, double* en, double* val, unsigned n  )
:m_mass_ratio(massRatio), m_temp(temp),m_name(name)
{
  m_coldxs = new PhysicsVector(en, val, n);

//  char buff[100];
//  snprintf(buff, sizeof(buff), "%f", massRatio);
//  std::string buffAsStdStr = buff;
//  m_coldxs->save(buffAsStdStr.c_str());

  k_Boltzmann = 8.6173324e-11 ; //in MeV/K
  m_neutronMass = 939.56536 /2.99792458e+8 /2.99792458e+8 ; // in [MeV /( m^2/s^2) ];;

  buf1 = new double[5];
  buf2 = new double[5];
}

DopplerEffect::~DopplerEffect()
{
  delete buf1;
  delete buf2;
}

PhysicsVector* DopplerEffect::broadenThickening(double aT, double tolerance)
{
  if(tolerance<0 or tolerance>1.)
    throw std::runtime_error("DopplerEffect::broadenThickening: wrong tolerance value");
  int dataSize=m_coldxs->getSize();

  Sigma1FunctionVector sfVec;
  sfVec.reserve(dataSize-1);

  double ther_en = k_Boltzmann * aT;

  //initial linear functions
  double ceo = ther_en/m_mass_ratio;
  for(unsigned i=0; i< m_coldxs->getSize()-1; i++)
  {
    Sigma1Function sf;
    double en1,en2,s1,s2;
    m_coldxs->getEnergyValuePair(i,en1,s1);
    m_coldxs->getEnergyValuePair(i+1,en2,s2);

    if(en2-en1==0.)  {
      sf.Ak = 0.;
      sf.Bk = 0.;
      sf.Ck = 0.;
    }
    else  {
      sf.Ak = (en2 * s1 - en1 * s2)/(en2 - en1);
      sf.Bk = (s2-s1)/(en2 - en1);
      sf.Ck = sf.Bk * ceo;
    }
    sfVec.push_back(sf);
  }

  //initial boundaries
  std::vector<double> x_boundaries;
  x_boundaries.reserve(dataSize);
  for(int i=0; i< dataSize; i++)
  {
    double x = sqrt(m_coldxs->energy(i)/ceo );
    x_boundaries.push_back(x);

  }


  std::map<double, double> xsMap;


  double crossSection;
  double en;
  double broadening_threshold = aT*k_Boltzmann*1.e6;

  for(int i=0; i< dataSize; i++)  {
    //TK dead assignment commented: crossSection=0.;
    en = m_coldxs->energy(i);
    // if en > 1e6*kT, broadening is no need
    if(en > broadening_threshold )  {
      crossSection = m_coldxs->interpolate(en);
      xsMap[en]=crossSection;
    }
    else if(i==0)  {
      crossSection=BroadenOnePoint(en, aT, x_boundaries , sfVec);
      xsMap[en]=crossSection;

      if(m_coldxs->energy(1)/en > 1e3)  {
        en *= 1.03;
        while(en < m_coldxs->energy(1))  {
          crossSection=BroadenOnePoint(en, aT, x_boundaries , sfVec);
          xsMap[en]=crossSection;
          en *= 1.03;
        }
      }
    }
    else if(en < 1e-6) { //less then 1eV
      crossSection=BroadenOnePoint(en, aT, x_boundaries , sfVec);
      xsMap[en]=crossSection;
    }
    else  {
      crossSection=BroadenOnePoint(en, aT, x_boundaries , sfVec);
      xsMap[en]=crossSection;

      //check tolerance
      auto it = xsMap.find(en);
      --it;
      double en_last = it->first;
      double meanEnergy = (en+en_last)/2.;

      double xs_last = it->second;
      double mean_interpolated = (xs_last + crossSection) /2.;
      double mean_calulated = BroadenOnePoint(meanEnergy, aT,  x_boundaries , sfVec);
      xsMap[meanEnergy]=mean_calulated;

      if(fabs(mean_interpolated-mean_calulated) /mean_calulated > tolerance )
      {
        unsigned insertPoints = 5;
        double diff_en = (en-en_last)/insertPoints;
        for(unsigned j=1;j<insertPoints;j++)
        {
          double energy = en_last + j*diff_en;
          double aXs = BroadenOnePoint(energy, aT, x_boundaries , sfVec);
          xsMap[energy]=aXs;
        }
      }
    }
  }

  std::vector<double> energyGrid;
  energyGrid.reserve(xsMap.size());
  std::vector<double> broadenXS ;
  broadenXS.reserve(xsMap.size());

  for (std::map<double, double>::iterator it = xsMap.begin(); it!=xsMap.end();++it)  {
    if(energyGrid.size() < 3 || it == xsMap.end() )  {
      energyGrid.push_back(it->first);
      broadenXS.push_back(it->second);
    }
    else  {
      double en1 = energyGrid.at(energyGrid.size()-2);
      double s1 = broadenXS.at(broadenXS.size()-2);
      double en2 = energyGrid.back();
      double s2 = broadenXS.back();

      double en_p = it->first;
      double s = it->second;


      double interS = s1 + (en_p-en1)/(en2-en1)*(s2-s1);
      if(s==0)  {
        energyGrid.push_back(en_p);
        broadenXS.push_back(s);
      }
      else if(std::abs(interS-s) /s > tolerance )  {
        energyGrid.push_back(en_p);
        broadenXS.push_back(s);
      }
    }
  }

  PhysicsVector *hotData = new PhysicsVector(energyGrid.data(),
      broadenXS.data(),
      broadenXS.size());
  return hotData;

}


double DopplerEffect::getCrossSection (double aT, double en_in_MeV)
{
  auto phyVec = m_CrossSectionMap.find(aT);
  PhysicsVector *aVect = 0; //FIXME: nullptr;
  if(phyVec==m_CrossSectionMap.end())  {
    aVect = broadenThickening(aT);
    m_CrossSectionMap[aT] = aVect;
  }
  else  {
    aVect=phyVec->second;
  }
  //aVect->save(m_name.c_str());
  return aVect->interpolate(en_in_MeV);
}



PhysicsVector DopplerEffect::broaden(double aT)
{
  int dataSize=m_coldxs->getSize();

  Sigma1FunctionVector sfVec;
  sfVec.reserve(dataSize-1);

  double ther_en = k_Boltzmann * aT;

  //initial linear functions
  double ceo = ther_en/m_mass_ratio;
  for(unsigned i=0; i< m_coldxs->getSize()-1; i++)
  {
    Sigma1Function sf;
    double en1,en2,s1,s2;
    m_coldxs->getEnergyValuePair(i,en1,s1);
    m_coldxs->getEnergyValuePair(i+1,en2,s2);

    sf.Ak = (en2 * s1 - en1 * s2)/(en2 - en1);
    sf.Bk = (s2-s1)/(en2 - en1);
    sf.Ck = sf.Bk * ceo;

    sfVec.push_back(sf);
  }

  //initial boundaries
  std::vector<double> x_boundaries;
  x_boundaries.reserve(dataSize);
  for(int i=0; i< dataSize; i++)
  {
    double x = sqrt(m_coldxs->energy(i)/ceo );
    x_boundaries.push_back(x);
  }

  std::vector<double> energyGrid_toBebroaden(m_coldxs->getEnergy()) ;
  std::vector<double> broadenXS ;
  broadenXS.reserve(energyGrid_toBebroaden.size());


  double crossSection;
  double en;
  double broadening_threshold = aT*k_Boltzmann*1.e6;
  for(int i=0; i< dataSize; i++)
  {
    //TK dead assignment commented:crossSection=0.;
    en = energyGrid_toBebroaden[i];
    // if en > 1e6*kT, broadening is no need
    if(en > broadening_threshold)
    {
      crossSection = m_coldxs->interpolate(en);
      broadenXS.push_back(crossSection);
    }
    else
    {
      crossSection=BroadenOnePoint(en, aT, x_boundaries , sfVec);
      broadenXS.push_back(crossSection);
    }
  }


  return PhysicsVector(energyGrid_toBebroaden.data(),
      broadenXS.data(),
      broadenXS.size());

}

double DopplerEffect::BroadenOnePoint(double en,
    double aT,
    std::vector<double> &x_boundaries,
    Sigma1FunctionVector &sfVec)
{
  //
  double x_k, x_kp1, y;
  unsigned firstBin, lastBin;
  double crossSection=0.;

  y=sqrt(en*m_mass_ratio /(k_Boltzmann*aT));

  FindIntegralRange(en, aT, firstBin, lastBin);

  for(unsigned k=firstBin;k < lastBin; k++)  {
    x_k = x_boundaries[k];
    x_kp1 = x_boundaries[k+1];

    if(x_k == x_kp1)  {
      continue;
    }
    crossSection += HFuncFirstTerm(y, sfVec[k], x_k, x_kp1);

    // calculate second term
    if(m_mass_ratio*en/(k_Boltzmann*aT)<16.)    {
      crossSection -= HFuncSecondTerm(y, sfVec.at(k), x_k,x_kp1);
    }
  }

  if(firstBin==0)  {
    double e1=m_coldxs->energy(0);
    double s1=m_coldxs->value(0);
    double contribution  = BoundaryConditionOneOverV(y, aT, e1, s1);

    crossSection += contribution;
  }
  return crossSection;
}

void DopplerEffect::FindIntegralRange( double En,
    double aT,
    unsigned &firstBin,
    unsigned &lastBin)

{
  double Vn = sqrt(2.*En/m_neutronMass);
  double gamma = sqrt(m_mass_ratio*m_neutronMass/2./k_Boltzmann/aT);
  double Vcut = 4./gamma;
  double Vmax = Vn+Vcut;
  double Emax = 0.5*m_neutronMass*Vmax*Vmax;
  lastBin = m_coldxs->getEnUpperBound(Emax);

  if(Vcut < Vn)  {
    double Vmin = Vcut - Vn;
    double Emin = 0.5*m_neutronMass*Vmin*Vmin;
    firstBin = m_coldxs->getEnUpperBound(Emin);
    if(firstBin!=0)
      firstBin--;
  }
  else
  {
    firstBin=0;
  }
}

void DopplerEffect::F_func_a_2_inf(double a, double *factors)
{
  double Erfca = erfc(a);
  double a2=a*a;
  double a3=a2*a;
  double expa2 = exp(-a2);

  double sqrt_1_over_pi= sqrt(1./M_PI);
  factors[0] = Erfca;
  factors[1] = sqrt_1_over_pi*(expa2);
  factors[2] = .5*factors[0] + a*factors[1];
  factors[3] = (1+a*a)*factors[1];
  factors[4] = 1.5*factors[2]+a3*factors[1];
}


void DopplerEffect::F_func_0_2_a(double a, double *factors)
{
  double Erfa = erf(a);
  double expa2 = exp(-a*a);
  double sqrt_1_over_pi= sqrt(1./M_PI);
  double a2=a*a;
  double a3=a2*a;

  factors[0] = Erfa;
  factors[1] = sqrt_1_over_pi*(1.-expa2);
  factors[2] = 0.5*Erfa - a*expa2*sqrt_1_over_pi;
  factors[3] = sqrt_1_over_pi*(1.-(1.+a2)*expa2);
  factors[4] = 0.75*Erfa - sqrt_1_over_pi*(1.5*a + a3)*expa2;
}


double DopplerEffect::BoundaryConditionOneOverV(double y, double aT, double e1, double sigma1)
{
  double xs_buf=0.;

  double x_kp1 =sqrt(e1*m_mass_ratio /(k_Boltzmann*aT)); // expected to be 1e-5eV

  //first term
  F_func_a_2_inf(0 - y, buf1);
  F_func_a_2_inf(x_kp1 - y, buf2);

  xs_buf += (buf1[0]-buf2[0])*(sigma1*sqrt(e1*m_mass_ratio/(k_Boltzmann*aT))*y);
  xs_buf += (buf1[1]-buf2[1])*(sigma1*sqrt(e1*m_mass_ratio/(k_Boltzmann*aT)));

  //second term
  F_func_a_2_inf(0 + y, buf1);
  F_func_a_2_inf(x_kp1 + y, buf2);

  xs_buf += (buf1[0]-buf2[0])*(sigma1*sqrt(e1*m_mass_ratio/(k_Boltzmann*aT))*y);
  xs_buf -= (buf1[1]-buf2[1])*(sigma1*sqrt(e1*m_mass_ratio/(k_Boltzmann*aT)));

  xs_buf *= 1./(2.*y*y);

  return xs_buf;
}

double DopplerEffect::HFuncFirstTerm(double y, Sigma1Function &sf,double x_k, double x_kp1)
{

  double xs_buf=0.;

  F_func_a_2_inf(x_k - y, buf1);
  F_func_a_2_inf(x_kp1 - y, buf2);


  double y2=y*y;
  double y3=y2*y;
  double y4=y3*y;

  xs_buf += (buf1[0]-buf2[0])*(sf.Ak*y2 + sf.Ck*y4);
  xs_buf += (buf1[1]-buf2[1])*(2.*sf.Ak*y + 4.*sf.Ck*y3);
  xs_buf += (buf1[2]-buf2[2])*(sf.Ak+6.*sf.Ck*y2);
  xs_buf += (buf1[3]-buf2[3])*4.*sf.Ck*y;
  xs_buf += (buf1[4]-buf2[4])*sf.Ck;

  xs_buf *= 1./(2.*y2);

  return xs_buf;

}

double DopplerEffect::HFuncSecondTerm(double y, Sigma1Function &sf, double x_k, double x_kp1)
{

  double xs_buf=0.;

  F_func_a_2_inf(x_k + y, buf1);
  F_func_a_2_inf(x_kp1 + y, buf2);

  double y2=y*y;
  double y3=y2*y;
  double y4=y3*y;

  xs_buf += (buf1[0]-buf2[0])*(sf.Ak*y2 + sf.Ck*y4);
  xs_buf -= (buf1[1]-buf2[1])*(2.*sf.Ak*y + 4.*sf.Ck*y3);
  xs_buf += (buf1[2]-buf2[2])*(sf.Ak+6.*sf.Ck*y2);
  xs_buf -= (buf1[3]-buf2[3])*4.*sf.Ck*y;
  xs_buf += (buf1[4]-buf2[4])*sf.Ck;

  xs_buf *= 1./(2.*y2);

  return xs_buf;
}



