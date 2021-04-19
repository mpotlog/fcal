#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
//#include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include "G4PhysListFactory.hh"
#include "G4SystemOfUnits.hh"


#ifdef G4VIS_USE
  #include "G4VisExecutive.hh"
#endif


int main(int argc, char** argv) 
{
  G4cout << "Aplicatie pornita..."  << G4endl;   

  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }
    
  // Run manager
  G4RunManager * runManager = new G4RunManager;

  // Mandatory initialization classes
  runManager->SetUserInitialization(new DetectorConstruction);
  //runManager->SetUserInitialization(new PhysicsList);
  
   G4PhysListFactory factory;
   G4VModularPhysicsList *physicsList = factory.GetReferencePhysList("FTFP_BERT");
   //G4VModularPhysicsList *physicsList = factory.GetReferencePhysList("QGSP_BIC_EMY"); 
   physicsList->SetDefaultCutValue(0.001*mm);
   runManager->SetUserInitialization(physicsList);
  
  // Optional user action classes
   runManager->SetUserInitialization(new ActionInitialization());

  // Initialize G4 kernel
  runManager->Initialize();
  
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
    // interactive mode
    UImanager->ApplyCommand("/control/execute vrml1.mac");
    ui->SessionStart();
    delete ui;
  }
  
  // Delete the managers
  #ifdef G4VIS_USE
    delete visManager;
  #endif
  delete runManager;

  G4cout << "Aplicatie incheiata cu succes."  << G4endl;  
  
  return 0;
}

