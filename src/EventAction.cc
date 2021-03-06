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


EventAction::EventAction() : G4UserEventAction(),
fLumiCalHCID(-1)
{
   G4RunManager::GetRunManager()->SetPrintProgress(1);
}


EventAction::~EventAction(){}


void EventAction::BeginOfEventAction(const G4Event*)
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


    // --------------------- Fill Results tree  -------------------------------
    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    G4int hitsNb = 0;
    G4double Edepo[16] = {0.};

    for (unsigned long i = 0; i < lumicalHC->GetSize(); ++i)
    {
        auto hit = static_cast<LumicalHit*>(lumicalHC->GetHit(i));
        if(hit->fEdep <= 0.) continue;
        hitsNb++;
        fCalPlan.push_back(hit->fPlan);
        fCalSector.push_back(hit->fSector);
        fCalPad.push_back(hit->fPad);
        fCalEne.push_back(hit->fEdep);

        fCalX.push_back(hit->fX);
        fCalY.push_back(hit->fY);
        fCalZ.push_back(hit->fZ);
        fCalPx.push_back(hit->fPx);
        fCalPy.push_back(hit->fPy);
        fCalPz.push_back(hit->fPz);
        fCalpID.push_back(hit->fpID);
        //calpID = hit->GetPId();
        
        for (G4int k=0;k<16;k++) {
           if (hit->fPlan == k) { Edepo[k] += hit->fEdep; }
        }
    }
    analysisManager->FillNtupleIColumn(0, hitsNb);
    //analysisManager->FillNtupleIColumn(11, fCalpID);
    
    // --------------------- Fill Edep tree  -------------------------------    
    for (G4int j=0;j<16;j++) {
        analysisManager->FillNtupleDColumn(j+12, Edepo[j]);
    }
    
    analysisManager->AddNtupleRow();
    ClearVectors();
 }


 void EventAction::ClearVectors(){
    fCalPlan.clear();
    fCalSector.clear();
    fCalPad.clear();
    fCalEne.clear();

    fCalX.clear();
    fCalY.clear();
    fCalZ.clear();
    fCalPx.clear();
    fCalPy.clear();
    fCalPz.clear();
    fCalpID.clear();
}
