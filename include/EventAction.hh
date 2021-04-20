#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "G4Event.hh"

#include <iostream>
#include <fstream>

#include <vector>
using namespace std;

class EventAction : public G4UserEventAction
{
 public:
    EventAction();
    virtual  ~EventAction();


    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
    void ClearVectors();

    G4int fLumiCalHCID; //ID for EM hits collection

    std::vector<G4int> fCalPlan;
    std::vector<G4int> fCalZone;
    std::vector<G4int> fCalPad;
    std::vector<G4double> fCalEne;

    std::vector<G4double> fCalX;
    std::vector<G4double> fCalY;
    std::vector<G4double> fCalZ;
    std::vector<G4double> fCalPx;
    std::vector<G4double> fCalPy;
    std::vector<G4double> fCalPz;


 };
#endif
