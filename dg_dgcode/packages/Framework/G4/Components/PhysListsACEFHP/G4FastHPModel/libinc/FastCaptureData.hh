#ifndef FastCaptureData_h
#define FastCaptureData_h 1


#include "G4VCrossSectionDataSet.hh"
#include "G4DynamicParticle.hh"
#include "G4Element.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicsTable.hh"
#include "G4NeutronHPCaptureData.hh"

#include "FastHPManager.hh"
class FastCaptureData : public G4NeutronHPCaptureData
{
   public:
   
      FastCaptureData();
   
      virtual ~FastCaptureData();

      virtual G4double GetIsoCrossSection( const G4DynamicParticle* dp ,G4int /*Z*/ , G4int /*A*/ ,
          const G4Isotope* /*iso*/  ,
          const G4Element* element ,
          const G4Material* material );

      void GenerateHotData();
   
   private:
      G4double GetCrossSection(const G4DynamicParticle* aP, const G4Element* anE, G4double aT);
   
   protected:
   
      G4PhysicsTable * thePrivateCrossSections;
      FastHPManager *man;

      G4double ke_cache_ess;
      G4double xs_cache_ess;
      const G4Element* element_cache_ess;
      const G4Material* material_cache_ess;

};

#endif
