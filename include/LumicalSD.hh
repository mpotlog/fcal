#ifndef LumicalSD_h
#define LumicalSD_h 1
 
#include "G4VSensitiveDetector.hh"
#include "LumicalHit.hh"


class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
 
class LumicalSD : public G4VSensitiveDetector
{
   public:
    LumicalSD(const G4String& name);
    virtual ~LumicalSD();
    
    virtual void Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history ); 
 
   // G4int plan;
    
   private:
      LumicalHitsCollection* fHitsCollection;
      G4int fLumiCalHCID;
 
};
#endif
