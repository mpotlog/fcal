#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  fParticleGun->SetParticleDefinition(particleTable->FindParticle(particleName="e-"));
  fParticleGun->SetParticleEnergy(5.*GeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
 G4double zur = -4000.0*mm, x = 5.*mm, y = 5.*mm;
 G4double xur = x *(2*G4UniformRand()-1);
 G4double yur = y *(2*G4UniformRand()-1);
 fParticleGun->SetParticlePosition(G4ThreeVector(xur,yur,zur));
 
 fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
 fParticleGun->GeneratePrimaryVertex(anEvent);
   
}

