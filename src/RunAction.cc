#include "RunAction.hh"
#include "EventAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"

#include "Analysis.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(EventAction* eventAction)
 : G4UserRunAction(),
 fEventAction(eventAction)
{
    // Create analysis manager
    // The choice of analysis technology is done via selectin of a namespace in Analysis.hh
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFirstNtupleId(1);
    G4cout << "Using " << analysisManager->GetType() << " analysis manager" << G4endl;

    // Default settings
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFileName("fcal_emstd1GeV_ppm");

  // Creating ntuples
    if ( fEventAction ) {
        analysisManager->CreateNtuple("ResultsTree", "Lumical various data");
        analysisManager->CreateNtupleIColumn("nHits");                              // Id = 0
        analysisManager->CreateNtupleIColumn("Plan", fEventAction->fCalPlan);       // Id = 1
        analysisManager->CreateNtupleIColumn("Sector", fEventAction->fCalSector);   // Id = 2
        analysisManager->CreateNtupleIColumn("Pad",  fEventAction->fCalPad);        // Id = 3
        analysisManager->CreateNtupleDColumn("Energy", fEventAction->fCalEne);      // Id = 4
        analysisManager->CreateNtupleDColumn("pos_x", fEventAction->fCalX);         // Id = 5
        analysisManager->CreateNtupleDColumn("pos_y", fEventAction->fCalY);         // Id = 6
        analysisManager->CreateNtupleDColumn("pos_z", fEventAction->fCalZ);         // Id = 7
        analysisManager->CreateNtupleDColumn("px", fEventAction->fCalPx);           // Id = 8
        analysisManager->CreateNtupleDColumn("py", fEventAction->fCalPy);           // Id = 9
        analysisManager->CreateNtupleDColumn("pz", fEventAction->fCalPz);           // Id = 10
        analysisManager->CreateNtupleIColumn("particleId", fEventAction->fCalpID);  // Id = 11
        //analysisManager->CreateNtupleSColumn("particleId");  // Id = 11
        
        stringstream nameformat;
        G4String edname;
        for(G4int k = 0; k < 16; k++){
            G4String colnames = "edep_";
            G4String colnamer = "X0";
            nameformat << k;
            colnames += nameformat.str() + colnamer;
            nameformat.str("");
            edname = colnames;

            analysisManager->CreateNtupleDColumn(edname);
        }

       analysisManager->FinishNtuple();
    }
}


RunAction::~RunAction()
{
    delete G4AnalysisManager::Instance();
}


void RunAction::BeginOfRunAction(const G4Run* )
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run*)
{
    // save histograms
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}
