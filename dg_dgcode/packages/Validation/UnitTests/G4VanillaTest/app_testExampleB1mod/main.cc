#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4VanillaTest/B1DetectorConstruction.hh"
#include "G4VanillaTest/B1ActionInitialization.hh"
#include "QBBC.hh"
#include "Randomize.hh"

int main()
{
  // construct the default run manager
  //auto runManager = G4RunManagerFactory::CreateRunManager();
  G4RunManager* runManager = new G4RunManager;

  // Detector construction
  runManager->SetUserInitialization(new B1DetectorConstruction());

  // Physics list
  G4VModularPhysicsList* physicsList = new QBBC;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  runManager->SetUserInitialization(new B1ActionInitialization());

  // initialize G4 kernel
  runManager->Initialize();

  // get the pointer to the UI manager and set verbosities
  G4UImanager* UI = G4UImanager::GetUIpointer();
  UI->ApplyCommand("/run/verbose 0");
  UI->ApplyCommand("/event/verbose 0");
  UI->ApplyCommand("/tracking/verbose 0");

  // start a run - differs from original Geant4 Basic/B1
  int numberOfEvent = 1;
  runManager->BeamOn(numberOfEvent);

  // job termination
  delete runManager;
  return 0;
}
