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
      LumicalHit(const LumicalHit& right);
      virtual ~LumicalHit();
            
      const LumicalHit& operator=(const LumicalHit& right);
      G4bool operator==(const LumicalHit&) const;

      inline void *operator new(size_t);
      inline void  operator delete(void* aHit);
      
      G4int plan1, zone, pad;
      G4double totene;
      G4double x, y, z, px, py, pz;
      
      void SetEdep(G4double de) { fEdep = de; }
      G4double GetEdep() const { return fEdep; }
    
  private:
    G4double fEdep;        //< Energy deposit in the sensitive volume
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


