#include "LumicalSD.hh"
#include "LumicalHit.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4HCofThisEvent.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4HCtable.hh"
#include "G4VTouchable.hh"
#include "G4SystemOfUnits.hh"

#include "G4VProcess.hh"
#include "G4String.hh"
#include "Randomize.hh"

#include "G4EventManager.hh"
#include "G4RunManager.hh"


LumicalSD::LumicalSD(const G4String& name)
: G4VSensitiveDetector(name),
 fHitsCollection(0),
 fLumiCalHCID(-1)
{
   G4String HCname = "LumicalCollect";
    collectionName.insert(HCname);
}


LumicalSD::~LumicalSD()
{}


// -------------create new Hit Collection at start of the event ---------------
void LumicalSD::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection = new LumicalHitsCollection(SensitiveDetectorName, collectionName[0]);
    if (fLumiCalHCID<0)
    { fLumiCalHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
    hce->AddHitsCollection(fLumiCalHCID,fHitsCollection);
  
    // Hit collection is created at the begining of each event
    for(G4int plan=33; plan<244; plan+=15){
        for(G4int zone=0; zone<4; zone++){
            for(G4int pad=0; pad<64; pad++){
                G4int plan1 = (plan-33)/15;
                fHitsCollection->insert(new LumicalHit(plan1, zone, pad));
            }
        }
    }

}


// --- core of the SD, running whenever a step is inside the assigned volume(s) ------
G4bool LumicalSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    //const G4StepPoint* endPoint = step->GetPostStepPoint(); 
    // check that an real interaction occured (eg. not a transportation)
    //G4StepStatus stepStatus = endPoint->GetStepStatus();
    //G4bool transmit = (stepStatus==fGeomBoundary || stepStatus==fWorldBoundary);
    //if (transmit) return true;

    //-----------create a hit and populate it with the information ----------
        
    G4double edep = (step->GetTotalEnergyDeposit()/MeV) - (step->GetNonIonizingEnergyDeposit()/MeV);
    if (edep <= 0.) return true;
    
        
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4TouchableHistory* touchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
    G4ThreeVector position = step->GetTrack()->GetPosition();    // position in mm
   
    //G4int plan = touchable->GetCopyNumber(3);
    G4int plan = touchable->GetReplicaNumber(2);
    G4int zone = touchable->GetReplicaNumber(1);
    G4int pad  = touchable->GetReplicaNumber(0);
   
    G4int padID = pad + 64 * zone + 256 * ((plan-33)/15);
    LumicalHit *hit = (*fHitsCollection)[padID];

    hit->totene += edep;
    
    //G4ThreeVector position = preStepPoint->GetPosition();
    hit->x = position.getX();
    hit->y = position.getY();
    hit->z = position.getZ();

    G4ThreeVector momentum = preStepPoint->GetMomentum();
    hit->px = momentum.getX();
    hit->py = momentum.getY();
    hit->pz = momentum.getZ();
    
     
    hit->SetEdep(edep); 
    //fHitsCollection -> insert(hit);
      
    return true;
   
}





