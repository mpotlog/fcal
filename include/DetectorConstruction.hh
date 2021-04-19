#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;

#include "G4VUserDetectorConstruction.hh"
#include "G4AssemblyVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4Tubs.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
     

  private:
      
    G4Box* absorberSolid[16];
      
    // Logical volumes
    G4LogicalVolume* worldLog;
    G4LogicalVolume* AlfoilLog;
    G4LogicalVolume* teleplanLog[5];
    G4LogicalVolume* scintplanLog;
    G4LogicalVolume* absorberLog[16];
    
    G4LogicalVolume* stratK1_log;
    G4LogicalVolume* stratAd1_log;
    G4LogicalVolume* stratCu1_log;
    G4LogicalVolume* stratK2_log;
    G4LogicalVolume* stratAd2_log;
    G4LogicalVolume* stratAraldite1_log;
    G4LogicalVolume* stratAl1_log;
    G4LogicalVolume* logicCalSensor;   
    G4LogicalVolume* logicCalZone;   
    G4LogicalVolume* fLogicCalPad;   
    G4LogicalVolume* stratAl2_log;
    G4LogicalVolume* stratCu_HV_log;
    G4LogicalVolume* stratconductive_glue_log;
    G4LogicalVolume* stratK_HV_log;
    G4LogicalVolume* stratAraldite2_log;
    G4LogicalVolume* CfiberLog;
    G4LogicalVolume* stratK3_log;
   
    G4LogicalVolume* magnetLog;
   
    
    // Physical volumes
    G4VPhysicalVolume* worldPhys;
    G4VPhysicalVolume* AlfoilPhys;
    G4VPhysicalVolume* teleplanPhys;
    G4VPhysicalVolume* scintplanPhys1;
    G4VPhysicalVolume* scintplanPhys2;
    G4VPhysicalVolume* absorberPhys[16];
 
    G4VPhysicalVolume* stratK1Phys;
    G4VPhysicalVolume* stratAd1Phys;
    G4VPhysicalVolume* stratCu1Phys;
    G4VPhysicalVolume* stratK2Phys;
    G4VPhysicalVolume* stratAd2Phys;
    G4VPhysicalVolume* stratK3Phys;
    G4VPhysicalVolume* stratAraldite1Phys;
    G4VPhysicalVolume* stratAl1Phys;
    G4VPhysicalVolume* stratAl2Phys;
    G4VPhysicalVolume* stratconductive_gluePhys;
    G4VPhysicalVolume* stratCu_HVPhys;
    G4VPhysicalVolume* stratK_HVPhys;
    G4VPhysicalVolume* stratAraldite2Phys;
    G4VPhysicalVolume* CfiberPhys;
      
    G4VPhysicalVolume* magnetPhys;
  
       
    G4bool fCheckOverlaps=true; 			// option to activate checking of volumes overlaps
    G4bool pSurfChk=true;
    
};

#endif
