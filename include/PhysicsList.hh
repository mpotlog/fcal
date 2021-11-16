#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include "G4EmConfigurator.hh"

class G4VPhysicsConstructor;
class StepMax;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList: public G4VModularPhysicsList
{
public:

  PhysicsList();
  virtual ~PhysicsList();

  void ConstructParticle();
    
  void SetCuts();
  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);
  void SetCutForProton(G4double);
  void SetCutForMuonMinus(G4double); 
  
  void AddPhysicsList(const G4String& name);
  void ConstructProcess();
    
  void AddStepMax(); 

private:

  void  AddPAIModel(const G4String&);
  void  NewPAIModel(const G4ParticleDefinition*, const G4String& modname, 
                    const G4String& procname);

  G4EmConfigurator* fConfig;

  G4double fCutForGamma;
  G4double fCutForElectron;
  G4double fCutForPositron;
  G4double fCutForProton;
  G4double fCutForMuonMinus;

  G4VPhysicsConstructor*  fEmPhysicsList;
  G4VPhysicsConstructor*  fDecayPhysicsList;
  std::vector<G4VPhysicsConstructor*> fHadronPhys;
  G4String fEmName;
    
  StepMax* fStepMaxProcess;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

