#include "EventAction.hh"
#include "LumicalHit.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"

#include "G4Run.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "g4root.hh"

#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <vector>
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction() : G4UserEventAction(), 
fLumiCalHCID(-1) 
{ 
   G4RunManager::GetRunManager()->SetPrintProgress(1);
} 

 
EventAction::~EventAction()
{}


void EventAction::BeginOfEventAction(const G4Event* event) 
{ 
     if (fLumiCalHCID==-1) {
        G4SDManager* sdManager = G4SDManager::GetSDMpointer();
        fLumiCalHCID = sdManager->GetCollectionID("sdLumical/LumicalCollect");  
    }
}
 
void EventAction::EndOfEventAction(const G4Event* event) 
{ 
    G4HCofThisEvent* hce = event->GetHCofThisEvent(); 
    if (!hce) return; 
  
    // Get hits collections 
    LumicalHitsCollection* lumicalHC = static_cast<LumicalHitsCollection*>(hce->GetHC(fLumiCalHCID));
    if ( !lumicalHC ) return; 
   
   
    // --------------------- Fill tree  -------------------------------  
    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    auto hitsNb = 0;
    G4double Edepo[15] = {0};
    
    
    for (unsigned long i = 0; i < lumicalHC->GetSize(); ++i){
        auto hit = static_cast<LumicalHit*>(lumicalHC->GetHit(i));
        if(hit->totene <= 0.) continue;
        hitsNb++;
        fCalPlan.push_back(hit->plan1);
        fCalZone.push_back(hit->zone);
        fCalPad.push_back(hit->pad);
        fCalTotEne.push_back(hit->totene);
        
        fCalX.push_back(hit->x);
        fCalY.push_back(hit->y);
        fCalZ.push_back(hit->z);
        fCalPx.push_back(hit->px);
        fCalPy.push_back(hit->py);
        fCalPz.push_back(hit->pz);
       
        for (G4int k=0;k<15;k++) {
           if (hit->plan1 == k) { Edepo[k] += hit->GetEdep(); }
        }
    }
    analysisManager->FillNtupleIColumn(0, hitsNb);
     
    for (G4int j=0;j<15;j++) {
        analysisManager->FillNtupleDColumn(j+11, Edepo[j]);
    }
        
    analysisManager->AddNtupleRow();
    ClearVectors();
 }

 
 void EventAction::ClearVectors(){
    fCalPlan.clear();
    fCalZone.clear();
    fCalPad.clear();
    fCalTotEne.clear();
    
    fCalX.clear();
    fCalY.clear();
    fCalZ.clear();
    fCalPx.clear();
    fCalPy.clear();
    fCalPz.clear();
}
