#include "G4FastHPModel/FastHPInstall.hh"
#include "G4FastHPModel/FastHPManager.hh"

#include "G4Neutron.hh"
#include "G4ProcessManager.hh"

#include "G4HadronElasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4HadronFissionProcess.hh"

#include "G4NeutronHPFission.hh"
#include "G4NeutronHPFissionData.hh"


#include "G4FastHPModel/FastInelasticData.hh"
#include "G4FastHPModel/FastCaptureData.hh"
#include "G4FastHPModel/FastElasticData.hh"

#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPElastic.hh"

#include "G4HadronicInteraction.hh"
#include "G4LFission.hh"

#include "G4NeutronKiller.hh"

#include "G4Version.hh"
#include "Utils/_tempfixbadcxx11.hh"

#if G4VERSION_NUMBER >= 1030
#include "G4SystemOfUnits.hh"
#include "G4EnergyRangeManager.hh"
#endif

void registerFastHPModel()
{
  G4ProcessManager* pmanager = G4Neutron::Neutron()->GetProcessManager();
  assert(pmanager);
  const G4ProcessVector* pl = pmanager->GetProcessList();
  std::vector<G4HadronElasticProcess*> elaProcVec;
  std::vector<G4HadronCaptureProcess*> capProcVec;
  std::vector<G4NeutronInelasticProcess*> inlProcVec;
  std::vector<G4HadronFissionProcess*> fisProcVec;

  for (G4int i=0;i<pl->size();++i) {
    if (!pmanager->GetProcessActivation(i))
      continue;
    G4HadronElasticProcess* aElasticProc = dynamic_cast<G4HadronElasticProcess*>((*pl)[i]);
    if (aElasticProc) {
      elaProcVec.push_back(aElasticProc);
    }
    G4HadronCaptureProcess* aCaptureProc = dynamic_cast<G4HadronCaptureProcess*>((*pl)[i]);
    if (aCaptureProc) {
      capProcVec.push_back(aCaptureProc);
    }
    G4NeutronInelasticProcess* aInelaProc = dynamic_cast<G4NeutronInelasticProcess*>((*pl)[i]);
    if (aInelaProc) {
      inlProcVec.push_back(aInelaProc);
    }
    G4HadronFissionProcess* aFisProc = dynamic_cast<G4HadronFissionProcess*>((*pl)[i]);
    if (aFisProc) {
      fisProcVec.push_back(aFisProc);
    }

    G4NeutronKiller *nkill = dynamic_cast<G4NeutronKiller*>((*pl)[i]);
    if(nkill)
      pmanager->RemoveProcess(nkill);
  }
  if ( (elaProcVec.size()*capProcVec.size()*inlProcVec.size())!=1)  {
    G4cout <<  elaProcVec.size()  << " "
        << capProcVec.size() << " "
        << inlProcVec.size() << G4endl;
    G4Exception("registerFastHPModel:","Error",FatalException,
        "there should one and only one process object for each process");
  }


  const G4MaterialTable *matTable = G4Material::GetMaterialTable();
  const G4Material* aRandMat = matTable->at(0);

  //all processes are found by now
  G4NeutronInelasticProcess* theNeutronInelProcess = inlProcVec[0];
  G4HadronElasticProcess*   theNeutronElasticProcess   = elaProcVec[0];
  G4HadronCaptureProcess* capProcess = capProcVec[0];
  G4HadronFissionProcess* fissionProcess = nullptr;

  if(fisProcVec.size()==0)  {
    fissionProcess = new G4HadronFissionProcess();
    G4LFission* fissionLModel = new G4LFission();
    fissionLModel->SetMinEnergy(0.*MeV);
    fissionProcess->RegisterMe(fissionLModel);
    pmanager->AddDiscreteProcess(fissionProcess);
  }
  else if(fisProcVec.size()==1)
    fissionProcess = fisProcVec[0];
  else
    G4Exception("registerFastHPModel:","Error",FatalException,
        "More than one process for neutron fission");

  if (!fissionProcess)
    return;//should never happen, but here to silence clang static analyzer



#if G4VERSION_NUMBER >= 1030
  G4DynamicParticle particle (G4Neutron::Neutron(),G4ThreeVector(1,0,0),10*MeV);
  G4HadProjectile proj(particle);
  G4Nucleus nucleus(aRandMat);

  /////////////Elastic/////////////////////////
  for(auto it=theNeutronElasticProcess->GetHadronicInteractionList().begin();it!=theNeutronElasticProcess->GetHadronicInteractionList().end();++it)
  {
    if((*it)->IsApplicable(proj,nucleus))
    {
      if(dynamic_cast<G4NeutronHPInelastic*>(*it))
      {
        // HP model found
        G4Exception("registerFastHPModel:","Error",FatalException,
            "HP inelastic model exists");
      }
      else {
        (*it)->SetMinEnergy(20*MeV);
        theNeutronElasticProcess->AddDataSet(  new FastElasticData() );
        theNeutronElasticProcess->RegisterMe(new G4NeutronHPElastic());
      }
    }
  }

  /////////////Inelastic/////////////////////////
  for(auto it=theNeutronInelProcess->GetHadronicInteractionList().begin();it!=theNeutronInelProcess->GetHadronicInteractionList().end();++it)
  {
    if((*it)->IsApplicable(proj,nucleus))
    {
      if(dynamic_cast<G4NeutronHPInelastic*>(*it))
      {
        // HP model found
        G4Exception("registerFastHPModel:","Error",FatalException,
            "HP inelastic model exists");
      }
      else {
        (*it)->SetMinEnergy(20*MeV);
        FastInelasticData* theHPInelasticData = new FastInelasticData();
        theNeutronInelProcess->AddDataSet( theHPInelasticData );
        G4NeutronHPInelastic* theHPInelasticModel = new G4NeutronHPInelastic();
        theNeutronInelProcess->RegisterMe(theHPInelasticModel);
      }
    }
  }
  /////////////Capture/////////////////////////
  for(auto it=capProcess->GetHadronicInteractionList().begin();it!=capProcess->GetHadronicInteractionList().end();++it)
  {
    if((*it)->IsApplicable(proj,nucleus))
    {
      if(dynamic_cast<G4NeutronHPInelastic*>(*it))
      {
        // HP model found
        G4Exception("registerFastHPModel:","Error",FatalException,
            "HP inelastic model exists");
      }
      else {
        (*it)->SetMinEnergy(20*MeV);
        capProcess->AddDataSet(  new FastCaptureData() );
        capProcess->RegisterMe(new G4NeutronHPCapture());
      }
    }
  }
  /////////////Fission/////////////////////////
  for(auto it=fissionProcess->GetHadronicInteractionList().begin();it!=fissionProcess->GetHadronicInteractionList().end();++it)
  {
    if((*it)->IsApplicable(proj,nucleus))
    {
      if(dynamic_cast<G4NeutronHPInelastic*>(*it))
      {
        // HP model found
        G4Exception("registerFastHPModel:","Error",FatalException,
            "HP inelastic model exists");
      }
      else {
        (*it)->SetMinEnergy(20*MeV);
        fissionProcess->AddDataSet(  new G4NeutronHPFissionData());
        fissionProcess->RegisterMe(new G4NeutronHPFission());
      }
    }
  }



#else
  const G4Element* aRandElement = aRandMat->GetElement(0);

  /////////////Elastic/////////////////////////
  //in versions later than 10.0 G4HadronicInteraction vector can be obtained from G4EnergyRangeManager::GetHadronicInteractionList()
  G4HadronicInteraction * midEnElasticModel = theNeutronElasticProcess->GetManagerPointer()->GetHadronicInteraction(0., aRandMat,aRandElement );
  if(midEnElasticModel)  {
    G4cout << "Set the minimum energy of " << midEnElasticModel->GetModelName() << " to 20MeV" << G4endl;
    midEnElasticModel->SetMinEnergy(20*MeV);
  }
  else
    G4Exception("registerFastHPModel:","Error",FatalException,
        "can't find any elastic model to treat 0.0MeV neutron");
  FastElasticData* theHPElasticData = new FastElasticData();
  theNeutronElasticProcess->AddDataSet( theHPElasticData );
  G4NeutronHPElastic* theNeutronElasticModel = new G4NeutronHPElastic();
  theNeutronElasticProcess->RegisterMe(theNeutronElasticModel);

  /////////////Inelastic/////////////////////////
  G4HadronicInteraction * midEnInelasticModel = theNeutronInelProcess->GetManagerPointer()->GetHadronicInteraction(0., aRandMat,aRandElement );
  if(midEnInelasticModel) {
    G4cout << "Set the minimum energy of " << midEnInelasticModel->GetModelName() << " to 20MeV" << G4endl;
    midEnInelasticModel->SetMinEnergy(20*MeV);
  }
  else
    G4Exception("registerFastHPModel:","Error",FatalException,
        "can't find any inelastic model to treat 0.0MeV neutron");
  FastInelasticData* theHPInelasticData = new FastInelasticData();
  theNeutronInelProcess->AddDataSet( theHPInelasticData );
  G4NeutronHPInelastic* theHPInelasticModel = new G4NeutronHPInelastic();
  theNeutronInelProcess->RegisterMe(theHPInelasticModel);
  ///////////////////////Fission////////////////////////////////////////////
  G4HadronicInteraction * midEnFissionModel = fissionProcess->GetManagerPointer()->GetHadronicInteraction(0., aRandMat,aRandElement );
  if(midEnFissionModel)  {
    G4cout << "Set the minimum energy of " << midEnFissionModel->GetModelName() << " to 20MeV" << G4endl;
    midEnFissionModel->SetMinEnergy(20*MeV);
  }
  else
    G4Exception("registerFastHPModel:","Error",FatalException,
        "can't find any fission model to treat 0.0MeV neutron");
  G4NeutronHPFissionData* theHPFissionData = new G4NeutronHPFissionData();
  fissionProcess->AddDataSet( theHPFissionData );
  G4NeutronHPFission* fissionHPModel = new G4NeutronHPFission();
  fissionProcess->RegisterMe(fissionHPModel);

  /////////////////////Capture//////////////
  G4HadronicInteraction * midEnCaptureModel = capProcess->GetManagerPointer()->GetHadronicInteraction(0., aRandMat,aRandElement );
  if(midEnCaptureModel)  {
    G4cout << "Set the minimum energy of " << midEnCaptureModel->GetModelName() << " to 20MeV" << G4endl;
    midEnCaptureModel->SetMinEnergy(20*MeV);
  }
  else
    G4Exception("registerFastHPModel:","Error",FatalException,
        "can't find any capture model to treat 0.0MeV neutron");
  FastCaptureData* theHPCaptureData = new FastCaptureData();
  capProcess->AddDataSet( theHPCaptureData );
  G4NeutronHPCapture* captureHPModel = new G4NeutronHPCapture();
  capProcess->RegisterMe(captureHPModel);
#endif

}

