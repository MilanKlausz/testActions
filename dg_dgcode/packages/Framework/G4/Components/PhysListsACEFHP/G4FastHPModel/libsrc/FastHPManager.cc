#include "G4FastHPModel/FastHPManager.hh"
#include "G4PhysicsVector.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include <stdexcept>

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicsTable.hh"
#include "G4VCrossSectionDataSet.hh"
#include "G4CrossSectionDataStore.hh"
#include "G4Neutron.hh"
#include "FHPDoppler/PhysicsVector.hh"
#include "FHPDoppler/DopplerEffect.hh"

#include "G4FastHPModel/FastElasticData.hh"
#include "G4FastHPModel/FastCaptureData.hh"
#include "G4FastHPModel/FastInelasticData.hh"
#include <iostream>


FastHPManager* FastHPManager::m_instance = 0;

FastHPManager::FastHPManager()
{
  m_ela_xs = new std::map<const G4Element* , DopplerEffect* >;
  m_cap_xs = new std::map<const G4Element* , DopplerEffect* >;
  m_inl_xs = new std::map<const G4Element* , DopplerEffect* >;
  m_ela_initialized=false;
  m_cap_initialized=false;
  m_inl_initialized=false;
}


FastHPManager::~FastHPManager()
{
}



void FastHPManager::initElastic(G4PhysicsTable* tab)
{
  printf("Running FastHPManager::initElastic\n");

  if(m_ela_initialized)
    throw std::runtime_error("FastHPManager::initElastic: elastic data have been already initialized\n");

  m_ela_table=tab;
  if(!m_ela_table)
    throw std::runtime_error("FastHPManager::initElastic m_ela_table is a null pointer\n");


  const G4MaterialTable *matTable = G4Material::GetMaterialTable();
  //printf("Size of the table %zu\n", m_ela_table->size());

  if(matTable->size()==0)
    throw std::runtime_error("FastHPManager::initElastic GetMaterialTable is empty\n");

  for(size_t i=0; i < matTable->size();i++)
  {
    G4Material *oneMat = matTable->at(i);
    //G4double aT = oneMat->GetTemperature();
    const G4ElementVector* eleVec = oneMat->GetElementVector();

    for(size_t ele_i=0; ele_i < eleVec->size();ele_i++)
    {
      const G4Element* anE = eleVec->at(ele_i);
      G4int ele_index = anE->GetIndex();

      //      ///////Get isotope////////////
      //      G4cout << (G4Element* ) anE << G4endl;
      //      auto isotopeVec = anE->GetIsotopeVector();
      //      G4double *abu = anE->GetRelativeAbundanceVector();
      //      for(unsigned isoID =0;isoID<isotopeVec->size();isoID++ )  {
      //        G4Isotope *aIso = isotopeVec->at(isoID);
      //        G4NeutronHPIsoData isoData;
      //        isoData.Init(aIso->GetN(),aIso->GetZ(),abu[i]);
      //        G4NeutronHPVector *ela_xs = isoData.MakeElasticData();
      //        if(ela_xs)  {
      //          printf("made isotope data for %u %u\n", aIso->GetN(),aIso->GetZ() );
      //          printf("%e %e\n",  ela_xs->GetX(0),  ela_xs->GetY(0) );
      //
      //        }
      //        else
      //          printf("failed to make isotope data for %u %u\n", aIso->GetN(),aIso->GetZ() );
      //      }
      //      ///////////////////////////////////////////

      //elastic
      printf("FastHPManager::initElastic: creating entry for element %s\n", anE->GetName().c_str());

      G4PhysicsVector* elaVec = (*m_ela_table)(ele_index);
      double m_mass_ratio=  anE->GetAtomicMassAmu() / 1.008644904;
      this->CreateElasticCrossEntry(anE, m_mass_ratio, elaVec) ;
    }

  }
  //printf("FastHPManager::initElastic: there are %zu elements in total",m_ela_xs->size());

  m_ela_initialized=true;
}

void FastHPManager::initCapture(G4PhysicsTable* tab)
{
  printf("Running FastHPManager::initCapture\n");

  if(m_cap_initialized)
    throw std::runtime_error("FastHPManager::initCapture: elastic data have been already initialized\n");

  m_cap_table=tab;
  if(!m_cap_table)
    throw std::runtime_error("FastHPManager::initCapture m_cap_table is a null pointer\n");

  const G4MaterialTable *matTable = G4Material::GetMaterialTable();

  if(matTable->size()==0)
    throw std::runtime_error("FastHPManager::initCapture GetMaterialTable is empty\n");

  for(size_t i=0; i < matTable->size();i++)
  {
    G4Material *oneMat = matTable->at(i);
    const G4ElementVector* eleVec = oneMat->GetElementVector();

    for(size_t ele_i=0; ele_i < eleVec->size();ele_i++)
    {
      const G4Element* anE = eleVec->at(ele_i);
      G4int ele_index = anE->GetIndex();

      printf("FastHPManager::initCapture: creating entry for element %s\n", anE->GetName().c_str());

      G4PhysicsVector* elaVec = (*m_cap_table)(ele_index);
      double m_mass_ratio=  anE->GetAtomicMassAmu() / 1.008644904;
      this->CreateCaptureCrossEntry(anE, m_mass_ratio, elaVec) ;
    }

  }
  //printf("FastHPManager::initCapture: there are %zu elements in total",m_cap_xs->size());
  m_cap_initialized=true;
}

void FastHPManager::initInelastic(G4PhysicsTable* tab)
{
  printf("Running FastHPManager::initInelastic\n");

  if(m_inl_initialized)
    throw std::runtime_error("FastHPManager::initInelastic: inelastic data have been already initialized\n");

  m_inl_table=tab;
  if(!m_inl_table)
    throw std::runtime_error("FastHPManager::initInelastic m_inl_table is a null pointer\n");

  const G4MaterialTable *matTable = G4Material::GetMaterialTable();


  if(matTable->size()==0)
    throw std::runtime_error("FastHPManager::initInelastic GetMaterialTable is empty\n");

  for(size_t i=0; i < matTable->size();i++)
  {
    G4Material *oneMat = matTable->at(i);
    const G4ElementVector* eleVec = oneMat->GetElementVector();

    for(size_t ele_i=0; ele_i < eleVec->size();ele_i++)
    {
      const G4Element* anE = eleVec->at(ele_i);
      G4int ele_index = anE->GetIndex();

      printf("FastHPManager::initInelastic: creating entry for element %s\n", anE->GetName().c_str());

      G4PhysicsVector* elaVec = (*m_inl_table)(ele_index);
      double m_mass_ratio=  anE->GetAtomicMassAmu() / 1.008644904;
      this->CreateInelasticCrossEntry(anE, m_mass_ratio, elaVec) ;
    }

  }
  //printf("FastHPManager::initInelastic: there are %zu elements in total",m_inl_xs->size());
  m_inl_initialized=true;
}


FastHPManager* FastHPManager::GetInstance()
{
  if (m_instance == 0) m_instance = new FastHPManager();
  return m_instance;

}



void FastHPManager::dispose()
{
  if (m_instance != 0)
  {
    delete m_instance;
    m_instance = 0;
  }
}




int FastHPManager::CreateElasticCrossEntry(const G4Element* ele, G4double m_mass_ratio, G4PhysicsVector *vec)
{
  std::map<const G4Element* , DopplerEffect* >::iterator it=m_ela_xs->find(ele);
  if(it==m_ela_xs->end())
  {
    std::vector<double> en;
    en.reserve(vec->GetVectorLength());
    std::vector<double> val;
    val.reserve(vec->GetVectorLength());

    for(unsigned i=0;i<vec->GetVectorLength();i++)  {
      en.push_back(vec->Energy(i));
      val.push_back(vec->operator [](i));
    }
    DopplerEffect* bd = new DopplerEffect("Elastic_"+ele->GetName()+std::to_string((long double)m_mass_ratio),m_mass_ratio, 0., en.data(), val.data(), vec->GetVectorLength());
    m_ela_xs->operator [](ele) = bd;
    return 0;
  }
  else return 1;
}

G4double FastHPManager::GetElasticCrossSection(const G4Element* ele, G4double en, G4double aT)
{
  auto it = m_ela_xs->find(ele);
  if(it==m_ela_xs->end())
    throw std::runtime_error("FastHPManager::GetElasticCrossSection: a new element added at run-time");
  DopplerEffect* bd = m_ela_xs->find(ele)->second;
  return bd->getCrossSection(aT, en);
}


int FastHPManager::CreateCaptureCrossEntry(const G4Element* ele, G4double m_mass_ratio, G4PhysicsVector *vec)
{
  std::map<const G4Element* , DopplerEffect* >::iterator it=m_cap_xs->find(ele);
  if(it==m_cap_xs->end())
  {
    std::vector<double> en;
    en.reserve(vec->GetVectorLength());
    std::vector<double> val;
    val.reserve(vec->GetVectorLength());

    for(unsigned i=0;i<vec->GetVectorLength();i++)  {
      en.push_back(vec->Energy(i));
      val.push_back(vec->operator [](i));
    }
    DopplerEffect* bd = new DopplerEffect("Capture_"+ele->GetName()+std::to_string((long double)m_mass_ratio),m_mass_ratio, 0., en.data(), val.data(), vec->GetVectorLength());
    m_cap_xs->operator [](ele) = bd;
    return 1;
  }
  else return -1;
}

G4double FastHPManager::GetCaptureCrossSection(const G4Element* ele, G4double en, G4double aT)
{
  auto it = m_cap_xs->find(ele);
  if(it==m_cap_xs->end())
    throw std::runtime_error("FastHPManager::GetCaptureCrossSection: a new element added at run-time");
  DopplerEffect* bd = m_cap_xs->find(ele)->second;
  //printf("%s %e %e %e\n", ele->GetName().c_str(), en, en/MeV, bd->getCrossSection(aT, en));
  return bd->getCrossSection(aT, en);
}


int FastHPManager::CreateInelasticCrossEntry(const G4Element* ele, G4double m_mass_ratio, G4PhysicsVector *vec)
{
  std::map<const G4Element* , DopplerEffect* >::iterator it=m_inl_xs->find(ele);
  if(it==m_inl_xs->end())
  {
    std::vector<double> en;
    en.reserve(vec->GetVectorLength());
    std::vector<double> val;
    val.reserve(vec->GetVectorLength());

    for(unsigned i=0;i<vec->GetVectorLength();i++)  {
      en.push_back(vec->Energy(i));
      val.push_back(vec->operator [](i));
    }
    DopplerEffect* bd = new DopplerEffect("Inelastic_"+ele->GetName()+std::to_string((long double)m_mass_ratio),m_mass_ratio, 0., en.data(), val.data(), vec->GetVectorLength());
    m_inl_xs->operator [](ele) = bd;
    return 1;
  }
  else return -1;
}

G4double FastHPManager::GetInelasticCrossSection(const G4Element* ele, G4double en, G4double aT)
{
  auto it = m_inl_xs->find(ele);
  if(it==m_inl_xs->end())
    throw std::runtime_error("FastHPManager::GetCaptureCrossSection: a new element added at run-time");
  DopplerEffect* bd = m_inl_xs->find(ele)->second;
  return bd->getCrossSection(aT, en);
}
