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
    plan1(pl), zone(zn), pad(pd), totene(0.), fEdep(0.)
  // fTrackLength(0.)
{}
    
LumicalHit::~LumicalHit()
{}
  

LumicalHit::LumicalHit(const LumicalHit &right)
 : G4VHit() 
{
  fEdep        = right.fEdep;
  //fTrackLength = right.fTrackLength;
}


const LumicalHit& LumicalHit::operator=(const LumicalHit& right)
{
  fEdep        = right.fEdep;
  //fTrackLength = right.fTrackLength;

  return *this;
}

G4bool LumicalHit::operator==(const LumicalHit& right) const
{
  return ( this == &right ) ? true : false;
}


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
