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


LumicalHit::LumicalHit(G4int pl, G4int zn, G4int pd)
    :G4VHit(),
    fPlan(pl), fZone(zn), fPad(pd), fEdep(0.){}

LumicalHit::~LumicalHit(){}




/*
int LumicalHit::operator==(const LumicalHit & ) const
{
    return 0;
  //return (fPad==right.fPad);
}


void LumicalHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fPos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

void LumicalHit::Print()
{
    G4cout << "  Pad[" << fPad << "] " << fEdep/MeV << " (MeV)" << G4endl;
}
*/
