#ifndef FastHPInstall_h
#define FastHPInstall_h 1

#include "G4VPhysicsConstructor.hh"
void registerFastHPModel();

class FHPInstaller: public G4VPhysicsConstructor
{
public:
  FHPInstaller():G4VPhysicsConstructor()  {};
  virtual ~FHPInstaller() {};
  virtual void ConstructProcess()  {
    registerFastHPModel();
  };
  virtual void ConstructParticle()  {}; // this method is purely virtual
};

#include "QGSP_BERT.hh"
class ESS_QGSP_BERT_FHP : public QGSP_BERT
{
public:
  ESS_QGSP_BERT_FHP() : QGSP_BERT()  {
    RegisterPhysics(new FHPInstaller());
    G4EmExtraPhysics*  emp = new G4EmExtraPhysics ();
    G4String on = "on";
    emp->MuonNuclear(on);
    RegisterPhysics(emp);
  }
};

#include "QGSP_BIC.hh"
class ESS_QGSP_BIC_FHP : public QGSP_BIC
{
public:
  ESS_QGSP_BIC_FHP() : QGSP_BIC()  {
    RegisterPhysics(new FHPInstaller());
    G4EmExtraPhysics*  emp = new G4EmExtraPhysics ();
    G4String on = "on";
    emp->MuonNuclear(on);
    RegisterPhysics(emp);  }
};

#include "G4EmExtraPhysics.hh"
class ESS_MUON :  public QGSP_BERT
{
public:
  ESS_MUON() : QGSP_BERT()  {
    G4EmExtraPhysics*  emp = new G4EmExtraPhysics ();
    G4String on = "on";
    emp->MuonNuclear(on);
    RegisterPhysics(emp);  }
};


#endif























