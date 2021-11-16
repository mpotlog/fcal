#include "LumicalHit.hh"

#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"

G4ThreadLocal G4Allocator<LumicalHit>* LumicalHitAllocator = 0;


LumicalHit::LumicalHit(G4int pl, G4int st, G4int pd)
    :G4VHit(), fPlan(pl), fSector(st), fPad(pd), fEdep(0.){}

LumicalHit::~LumicalHit(){}

