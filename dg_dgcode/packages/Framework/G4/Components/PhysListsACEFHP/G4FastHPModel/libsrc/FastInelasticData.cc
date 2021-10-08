#include "G4FastHPModel/FastInelasticData.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Neutron.hh"
#include "G4ElementTable.hh"
#include "G4NeutronHPData.hh"
#include "G4ParticleTable.hh"
#include <stdexcept>
/////////////////////////////////////////////////////////////

typedef G4PhysicsTable* hacktype;

template<typename Tag>
struct result {
  /* export it ... */
  typedef typename Tag::type type;
  static type hacktype;
};

template<typename Tag>
typename result<Tag>::type result<Tag>::hacktype;

template<typename Tag, typename Tag::type p>
struct rob : result<Tag> {
  struct filler {
    filler() { result<Tag>::hacktype = p; }
  };
  static filler filler_obj;
};

template<typename Tag, typename Tag::type p>
typename rob<Tag, p>::filler rob<Tag, p>::filler_obj;

struct PrivateXsTable { typedef hacktype(G4NeutronHPInelasticData::*type); };
template struct rob<PrivateXsTable, &G4NeutronHPInelasticData::theCrossSections>;



/////////////////////////////////////////////////////////////////


FastInelasticData::FastInelasticData()
:G4NeutronHPInelasticData()
{
  SetMinKinEnergy( 0*MeV );
  SetMaxKinEnergy( 20*MeV );


  ke_cache_ess =0 ;
  xs_cache_ess = 0.;
  element_cache_ess=0;
  material_cache_ess=0;
  man=FastHPManager::GetInstance();
  BuildPhysicsTable(* G4ParticleTable::GetParticleTable()->FindParticle("neutron"));
  GenerateHotData();

}

FastInelasticData::~FastInelasticData()
{

}


void FastInelasticData::GenerateHotData()
{
  G4PhysicsTable* table = this->*result<PrivateXsTable>::hacktype;
  if(!table)
    throw std::runtime_error("FastInelasticData::GenerateHotData table is not created\n");
  man->initInelastic(table);
  G4cout << "FastInelasticData::GenerateHotData: hot data created" << G4endl;
}



G4double FastInelasticData::GetIsoCrossSection( const G4DynamicParticle* dp ,
    G4int /*Z*/ , G4int /*A*/ ,
    const G4Isotope* /*iso*/  ,
    const G4Element* element ,
    const G4Material* material )
{
  if ( dp->GetKineticEnergy() == ke_cache_ess && element == element_cache_ess &&  material == material_cache_ess ) return xs_cache_ess;

  ke_cache_ess = dp->GetKineticEnergy();
  element_cache_ess = element;
  material_cache_ess = material;
  G4double xs = GetCrossSection( dp , element , material->GetTemperature() );
  xs_cache_ess = xs;
  return xs;
}



#include "G4Nucleus.hh"
#include "G4NucleiProperties.hh"
#include "G4Neutron.hh"
#include "G4Electron.hh"

G4double FastInelasticData::GetCrossSection(const G4DynamicParticle* aP, const G4Element* anE, G4double aT)
{
  G4double eKinetic = aP->GetKineticEnergy();
  G4double xs=man->GetInelasticCrossSection( anE, eKinetic, aT);

  return  xs;

}

