#ifndef LumicalHit_h
#define LumicalHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4Types.hh"

class LumicalHit : public G4VHit
{
   public:
      LumicalHit(G4int pl, G4int zn, G4int pd);
      virtual ~LumicalHit();


      inline void *operator new(size_t);
      inline void  operator delete(void* aHit);

      G4int fPlan, fZone, fPad;
      G4double fX, fY, fZ, fPx, fPy, fPz;
      G4double fEdep;        //< Energy deposit in the sensitive volume (in one hit/cell/pad)
};

/* define the hit collection type we used in the SD header */
//typedef G4THitsCollection<LumicalHit> LumicalHitsCollection;

using LumicalHitsCollection = G4THitsCollection<LumicalHit>;

extern G4ThreadLocal G4Allocator<LumicalHit>* LumicalHitAllocator;

inline void* LumicalHit::operator new(size_t)
{
  if (! LumicalHitAllocator)
        LumicalHitAllocator = new G4Allocator<LumicalHit>;
  return (void*)LumicalHitAllocator->MallocSingle();
}

inline void LumicalHit::operator delete(void* aHit)
{
  LumicalHitAllocator->FreeSingle((LumicalHit*) aHit);
}


#endif
