#ifndef StepMaxMessenger_h
#define StepMaxMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class StepMax;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class StepMaxMessenger: public G4UImessenger
{
public:

  StepMaxMessenger(StepMax*);
  ~StepMaxMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  StepMax* fStepMax;
  G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
