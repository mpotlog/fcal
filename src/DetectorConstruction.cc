#include "DetectorConstruction.hh"
#include "LumicalSD.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVDivision.hh"
#include "G4PVReplica.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4AssemblyVolume.hh"
#include "G4NistManager.hh"
#include "G4UIcommand.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "G4Region.hh"
#include "G4String.hh"

#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4AutoDelete.hh"

#include "G4SDManager.hh"

using namespace std;


DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(), 
 fLogicCalPad(nullptr)
{;}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
 //====================================== materials ==============================================
  G4double a;                   // numarul de masa
  G4double z;                   // numarul atomic
  G4double density;             // densitatea

  G4String name, symbol;
  G4int ncomponents,natoms;
  
  G4Element* elH  = new G4Element(name="Hydrogen", symbol="H", z=1., a=1.01*g/mole);    
  G4Element* elC  = new G4Element(name="Carbon",   symbol="C", z=6., a=12.01*g/mole); 
  G4Element* elN  = new G4Element(name="Nitrogen", symbol="N", z=7., a=14.01*g/mole);
  G4Element* elO  = new G4Element(name="Oxygen",   symbol="O", z=8., a=16.00*g/mole);


  //G4Material* Al = new G4Material(name="Aluminum", z=13., a = 26.98*g/mole, density = 2.700*g/cm3);

  G4NistManager* material = G4NistManager::Instance();
  G4Material* Air  = material->FindOrBuildMaterial("G4_AIR");
  G4Material* Ag   = material->FindOrBuildMaterial("G4_Ag");
  G4Material* Al   = material->FindOrBuildMaterial("G4_Al");
  G4Material* C    = material->FindOrBuildMaterial("G4_C");
  G4Material* cu   = material->FindOrBuildMaterial("G4_Cu");
  G4Material* Kapton = material->FindOrBuildMaterial("G4_KAPTON");
  G4Material* ni   = material->FindOrBuildMaterial("G4_Ni"); 
  G4Material* si   = material->FindOrBuildMaterial("G4_Si");
  G4Material* W = material->FindOrBuildMaterial("G4_W"); 
    
  // Absorber   
  G4Material* W_abs93 = new G4Material(name="Abs_93", density = 17.7*g/cm3, ncomponents=3);
  W_abs93->AddMaterial(cu, 1.75*perCent); 
  W_abs93->AddMaterial(ni, 5.25*perCent); 
  W_abs93->AddMaterial(W,  93.*perCent); 
    
  G4Material* W_abs95 = new G4Material(name="Abs_95", density = 18.*g/cm3, ncomponents=3);
  W_abs95->AddMaterial(cu, 2.5*perCent); 
  W_abs95->AddMaterial(ni, 2.5*perCent); 
  W_abs95->AddMaterial(W,  95.*perCent); 
  
  // Epoxy 
  G4Material* Epoxy = new G4Material("Epoxy" , density= 1.3*g/cm3, ncomponents=3);
  Epoxy->AddElement(elH, natoms=44);
  Epoxy->AddElement(elC, natoms=15);
  Epoxy->AddElement(elO, natoms=7);
   
  // Carbon fiber
  G4Material* CarbonFiber = new G4Material(name="CarbonFiber", density = 1.6*g/cm3, ncomponents=2);
  CarbonFiber->AddMaterial(C,     50.*perCent);
  CarbonFiber->AddMaterial(Epoxy, 50.*perCent); 
  
       
  // Epoxy glue - Araldite - partA
  G4Material* AralditeA = new G4Material(name="BisphenolAEpoxyResin", density = 1.20*mg/cm3, ncomponents=3);
  AralditeA->AddElement(elC, natoms=15);
  AralditeA->AddElement(elH, natoms=16);
  AralditeA->AddElement(elO, natoms=2);
    
  // Epoxy glue - Araldite - partB
  G4Material* AralditeB = new G4Material(name="Dimethylaminopropyl", density = 0.883*mg/cm3, ncomponents=3);
  AralditeB->AddElement(elC, natoms=8);
  AralditeB->AddElement(elH, natoms=21);
  AralditeB->AddElement(elN, natoms=3);
   
  G4Material* araldite= new G4Material (name="EpoxyGlue", density= 1.04*g/cm3, ncomponents=2);
  araldite->AddMaterial(AralditeA, 50*perCent);
  araldite->AddMaterial(AralditeB, 50*perCent);
  
  G4Material* conductive_glue= new G4Material (name="ConductiveGlue", density= 3.2*g/cm3, ncomponents=2);
  conductive_glue->AddMaterial(araldite, 70*perCent);
  conductive_glue->AddMaterial(Ag,       30*perCent);
 
  G4Material* Polyvinyltoluene= new G4Material (name="Polyvinyltoluene", density= 1.032*g/cm3, ncomponents=2);
  Polyvinyltoluene->AddElement(elC, natoms=9);
  Polyvinyltoluene->AddElement(elH, natoms=10);
       
  
  //=================================== experimental hall ===============================================
  G4double worldX = 2500.0*mm;
  G4double worldY = 2500.0*mm;
  G4double worldZ = 20500.0*mm;
  G4Box* world = new G4Box("world",worldX,worldY,worldZ);
     worldLog  = new G4LogicalVolume(world,Air,"worldLog",0,0,0);
     worldPhys = new G4PVPlacement(0,G4ThreeVector(),worldLog,"worldPhys",0,false,0);

  //------------------------------ general dimensions -----------------------------
  G4double wSlot = 4.5*mm;
  G4double dSlotfAl  = 58.*mm;         // distanta intre primul slot si folia de aluminiu din interiorul DUT ului
    
  
  G4double translateX  = 17.947*mm;    // translate all detectors on X axis for the beam to hit between first 2 sectors of the sensors
  G4double translateY  = -137.6*mm;    // translate all detectors on Y axis for the beam to hit in the middle of the sensors
  G4double tranTY = 0*mm;              // tranzitie pentru placile de w
  
  
  //=================================== Al foil - Lumical coverage =========================================  
  G4double foilAlX = 50.*mm;
  G4double foilAlY = 70.*mm;     
  G4double wfoilAl = 0.05*mm;
  
  G4Box* Alfoil = new G4Box("Alfoil",foilAlX,foilAlY,wfoilAl);
      AlfoilLog = new G4LogicalVolume(Alfoil,Al,"AlfoilLog");
  
  // distance from Absorber placed in front of sensor to Al foil covering the Lumical    
  G4double dAbsAlfoil = (wSlot + dSlotfAl)*mm;              // = 62.5 mm
     AlfoilPhys = new G4PVPlacement(0, G4ThreeVector(translateX,0,-(dAbsAlfoil + wfoilAl/2)*mm), AlfoilLog, "alfoilPhys", worldLog, false, 0, fCheckOverlaps);
  
  //============================= Telescope - 2 before and 3 after magnet =================================
  // telescope plan dimensions
  G4double telescopeX = 10.6*mm;
  G4double telescopeY = 5.3*mm;
  G4double telescopeZ = 0.05*mm;    

  G4Box* telescope = new G4Box("telescope",telescopeX,telescopeY,telescopeZ);
  
  // distances from telescope planes to the Lumical coverage
  G4double dLumTel5  = 364*mm;      // distance from Lumical to Telescope plan 5
  G4double dTel5Tel4 = 90*mm;       // distance from Telescope plan 5 to Telescope plan 4
  G4double dTel4Tel3 = 89*mm;       // distance from Telescope plan 4 to Telescope plan 3
  G4double dTel3Tel2 = 2167*mm;     // distance from Telescope plan 3 to Telescope plan 2
  G4double dTel2Tel1 = 173*mm;      // distance from Telescope plan 2 to Telescope plan 1
  
  // distance from Absorber placed in front of sensor to Telescope plan 5    
  G4double dAbsTel5 = (dAbsAlfoil + wfoilAl + dLumTel5)*mm; 
  
  // positions of Telescope planes
  G4double ZTelPlan5 = - (dAbsTel5 + telescopeZ/2)*mm; 
  G4double ZTelPlan4 = - (dAbsTel5 + dTel5Tel4 + telescopeZ)*mm; 
  G4double ZTelPlan3 = - (dAbsTel5 + dTel5Tel4 + dTel4Tel3 + (2*telescopeZ) + telescopeZ/2)*mm;     
  G4double ZTelPlan2 = - (dAbsTel5 + dTel5Tel4 + dTel4Tel3 + dTel3Tel2 + (3*telescopeZ) + telescopeZ/2 )*mm;   
  G4double ZTelPlan1 = - (dAbsTel5 + dTel5Tel4 + dTel4Tel3 + dTel3Tel2 + dTel2Tel1 + (4*telescopeZ) + telescopeZ/2)*mm;
   
  G4double ZTelPlans[5] = {ZTelPlan1, ZTelPlan2, ZTelPlan3, ZTelPlan4, ZTelPlan5} ;
     
  for( int b = 0; b < 5; b++ ){    
     teleplanLog[b] = new G4LogicalVolume(telescope, si, "teleplanLog[b]");
       teleplanPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, ZTelPlans[b]*mm), teleplanLog[b], "teleplanPhys", worldLog, false, b, fCheckOverlaps);
  }  
  
  //=================================== Trigger - 2 scintillators ========================================
  // scintillator dimensions
  G4double scintillatorX = 15.*mm;    
  G4double scintillatorY = 15.*mm;
  G4double scintillatorZ = 2.064*mm;           // 14.mm; - 2 dimensions used in previous experiments
  
  G4Box* scintplan = new G4Box("scintplan",scintillatorX,scintillatorY,scintillatorZ);
  
  // distances from scintillators to telescope planes  
  G4double dScint2Tel1 = 30*mm;
  G4double dScint2Scint1 = 40*mm;
    
  G4double Zscint2 = - (-ZTelPlan1 + telescopeZ/2 + dScint2Tel1 + scintillatorZ/2)*mm;
  G4double Zscint1 = - (-ZTelPlan1 + telescopeZ/2 + dScint2Tel1 + dScint2Scint1 + scintillatorZ + scintillatorZ/2)*mm;
        
     scintplanLog = new G4LogicalVolume(scintplan, Polyvinyltoluene," scintplanLog");
    scintplanPhys1 = new G4PVPlacement(0, G4ThreeVector(0.*mm, 0, Zscint1*mm), scintplanLog, "scintplanPhys1", worldLog, false, 1, fCheckOverlaps);
    scintplanPhys2 = new G4PVPlacement(0, G4ThreeVector(0.*mm, 0, Zscint2*mm), scintplanLog, "scintplanPhys2", worldLog, false, 2, fCheckOverlaps);
  
  //========================================= Magnet ======================================================
  /* // magnet dimensions
  G4double mX=130*cm;
  G4double mY=75*cm;
  G4double mZ=1300*mm;
  
  G4double telescopdist= 100*mm;
  G4double tel2mag=500*mm;
  G4double tel3mag=500*mm;
  G4double duttel4= 1000*mm;
  G4double wm       =(- wSlot-48- wfoilAl -duttel4 -(2*telescopdist) - (3*telescopeZ)-tel3mag-mZ/2)*mm;
     
  G4Box* magnet = new G4Box("magnet",mX,mY,mZ/2);
      magnetLog = new G4LogicalVolume(magnet, Air,"magnetLog",0,0,0);
     magnetPhys = new G4PVPlacement(0,G4ThreeVector(0,0,wm*mm),magnetLog,"magnetPhys",worldLog,false,0,fCheckOverlaps);
    */        
            
  //=================================== LumiCal Configuration ========================================
  
  // dimensions of sensors layers  
  G4double wK1      = 0.0127*mm;            // polyimide 1 width
  G4double wAd1     = 0.0254*mm;            // adhesive 1 width
  G4double wCu1     = 0.0356*mm;            // copper 1 width
  G4double wK2      = 0.0254*mm;            // polyimide 2 width 
  G4double wAd2     = 0.0127*mm;            // adhesive 2 width
  G4double wEpoxy1  = 0.01*mm;              // epoxy glue 1 width
  G4double wAl      = 0.020*mm;             // aluminium 1 and 2 width
  G4double wSi      = 0.320*mm;
  G4double wcg      = 0.040*mm;             // conductive glue width
  G4double wCu_HV   = 0.025*mm;             // HV kapton - copper width
  G4double wK_HV    = 0.075*mm;             // HV kapton - polyimide width
  G4double wEpoxy2  = 0.02*mm;              // epoxy glue 2 width
  G4double wCf      = 0.115*mm;             // carbon fiber support width
    
  // positions of sensor layers
  G4double ZK1         = wK1/2*mm; 
  G4double ZAd1        = (wK1 + wAd1/2)*mm;
  G4double ZCu1        = (wK1 + wAd1 + wCu1/2)*mm;
  G4double ZK2         = (wK1 + wAd1 + wCu1 + wK2/2)*mm; 
  G4double ZAd2        = (wK1 + wAd1 + wCu1 + wK2 + wAd2/2)*mm;
  G4double ZK3         = (wK1 + wAd1 + wCu1 + wK2 + wAd2 + wK1/2)*mm;
  G4double ZAralditel1 = (wK1 + wAd1 + wCu1 + wK2 + wAd2 + wK1 + wEpoxy1/2)*mm;
  G4double ZAl1        = (wK1 + wAd1 + wCu1 + wK2 + wAd2 + wK1 + wEpoxy1 + wAl/2)*mm; 
  G4double before_Si   = (wK1 + wAd1 + wCu1 + wK2 + wAd2 + wK1 + wEpoxy1 + wAl)*mm;
  G4double ZSi         = ((0*wSlot) + before_Si + wSi/2)*mm;            // first plate of sensor
  G4double ZAl2        = (before_Si + wSi + wAl/2)*mm;
  G4double Zcg         = (before_Si + wSi + wAl + wcg/2)*mm;
  G4double ZCu_HV      = (before_Si + wSi + wAl + wcg + wCu_HV/2)*mm; 
  G4double ZK_HV       = (before_Si + wSi + wAl + wcg + wCu_HV + wK_HV/2 )*mm;
  G4double ZAralditel2 = (before_Si + wSi + wAl + wcg + wCu_HV + wK_HV + wEpoxy2/2)*mm;
  G4double ZCf         = (before_Si + wSi + wAl + wcg + wCu_HV + wK_HV + wEpoxy2 + wCf/2)*mm;
  G4double wSiSensorAll= (before_Si + wSi + wAl + wcg + wCu_HV + wK_HV + wEpoxy2 + wCf)*mm;      // end of Si sensor with front and back components 
 
  
  //****************************************** Tungsten absorbers *******************************************
  G4double absorberX = 65 *mm;
  G4double absorberY = 65 *mm;   
  
  G4double ZAbsorber[16] = { 3.520*mm, 3.470*mm, 3.542*mm, 3.505*mm, 3.490*mm, 3.584*mm, 3.521*mm, 3.645*mm, 3.470*mm, 3.550*mm, 3.558*mm, 3.543*mm, 3.543*mm, 3.550*mm, 3.550*mm, 3.528*mm };      // each absorber plate has a different thickness
  
  for( int k = 0; k < 16; k++ ){    
     absorberSolid[k] = new G4Box("Absorber",absorberX,absorberY,ZAbsorber[k]/2);
     absorberLog[k] = new G4LogicalVolume(absorberSolid[k], W_abs95,"absorberLog[k]");
             
     G4double wW0 = ((k-1)*wSlot+wSiSensorAll + ZAbsorber[k]/2 )*mm;
    
     absorberPhys[k] = new G4PVPlacement(0, G4ThreeVector(translateX, tranTY, wW0), absorberLog[k], "absorberPhys", worldLog, false, k, fCheckOverlaps);
  }    
  
        
  //****************************************** Silicon sensors *******************************************
  //--------------------------------------- kapton fan-out ---------------------------------
  G4double KRint = 80.*mm;
  G4double KRext = 195.2*mm;
  G4double KstartAngle = (90.*deg - 15.*deg);
  G4double KtotAngle = 30.*deg;

  // polyimide 1
  G4Tubs* stratK1 = new G4Tubs("stratK1", KRint, KRext, wK1/2, KstartAngle, KtotAngle);  
      stratK1_log = new G4LogicalVolume(stratK1, Kapton, "K1log");
  // adhesive 1
  G4Tubs* stratAd1 = new G4Tubs("stratAd1", KRint, KRext, wAd1/2, KstartAngle, KtotAngle);  
      stratAd1_log = new G4LogicalVolume(stratAd1, Epoxy, "Ad1log");
  // copper 1  
  G4Tubs* stratCu1 = new G4Tubs("stratCu1", KRint, KRext, wCu1/2, KstartAngle, KtotAngle);  
      stratCu1_log = new G4LogicalVolume(stratCu1, cu, "Cu1log");
  // polyimide 2  
  G4Tubs* stratK2 = new G4Tubs("stratK2", KRint, KRext, wK2/2, KstartAngle, KtotAngle);  
      stratK2_log = new G4LogicalVolume(stratK2, Kapton, "K2log");
  // adhesive 2
  G4Tubs* stratAd2 = new G4Tubs("stratAd2", KRint, KRext, wAd2/2, KstartAngle, KtotAngle);  
      stratAd2_log = new G4LogicalVolume(stratAd2, Epoxy, "Ad2log");
  // polyimide 3  
  G4Tubs* stratK3 = new G4Tubs("stratK3", KRint, KRext, wK1/2, KstartAngle, KtotAngle);  
      stratK3_log = new G4LogicalVolume(stratK3, Kapton, "K3log");

  //---------------------------------- epoxy glue 1 - Araldite -----------------------------  
  G4Tubs* stratAraldite1 = new G4Tubs("stratAraldite1", KRint, KRext, wEpoxy1/2, KstartAngle, KtotAngle);  
      stratAraldite1_log = new G4LogicalVolume(stratAraldite1, araldite, "Araldite1log");
    
  //--------------------------------------- Silicon sensor ---------------------------------  
  // aluminuim foil in front of Si sensor
  G4Tubs* stratAl1 = new G4Tubs("stratAl1", KRint, KRext, wAl/2, KstartAngle, KtotAngle);  
      stratAl1_log = new G4LogicalVolume(stratAl1, Al, "Al1log");
      
  //silicon layer - to identify pads we use replica of SensorPad and SenzorZone inside Sensor
  G4Tubs *solidSensor = new G4Tubs("solidSensor", KRint, KRext, wSi/2, KstartAngle+15.*deg, KtotAngle/2);
      logicCalSensor = new G4LogicalVolume(solidSensor, si, "logicCalSensor");
  G4Tubs *solidSensorZone = new G4Tubs("solidSensorZone", KRint, KRext, wSi/2, -3.75*deg, 7.5*deg);
      logicCalZone = new G4LogicalVolume(solidSensorZone, si, "logicCalZone");
  G4Tubs *solidSensorPad = new G4Tubs("solidSensorPad", KRint, KRext, wSi/2, -3.75*deg, 7.5*deg);
     fLogicCalPad = new G4LogicalVolume(solidSensorPad, si, "logicCalPad");
 
  new G4PVReplica("SiCalZone", logicCalZone, logicCalSensor, kPhi, 2, 7.5*deg, KstartAngle+15.*deg);
  new G4PVReplica("SiCalPad", fLogicCalPad, logicCalZone, kRho, 64, 1.8*mm, KRint);
 
    
  // aluminuim foil behind Si sensor
  G4Tubs* stratAl2 = new G4Tubs("stratAl1", KRint, KRext, wAl/2, KstartAngle, KtotAngle);  
      stratAl2_log = new G4LogicalVolume(stratAl2, Al, "Al2log");
    
  //--------------------------------------- conductive glue ---------------------------------  
  G4Tubs* stratconductive_glue = new G4Tubs("stratconductive_glue", KRint, KRext, wcg/2, KstartAngle, KtotAngle);  
      stratconductive_glue_log = new G4LogicalVolume(stratconductive_glue, conductive_glue, "conductivegluelog");
    
  //--------------------------------------- HV kapton ---------------------------------------
  // copper
  G4Tubs* stratCu_HV = new G4Tubs("stratCu_HV", KRint, KRext, wCu_HV/2, KstartAngle, KtotAngle);  
     stratCu_HV_log = new G4LogicalVolume(stratCu_HV, cu, "CuHVlog");
  // polyimide  
  G4Tubs* stratK_HV = new G4Tubs("stratK_HV", KRint, KRext, wK_HV/2, KstartAngle, KtotAngle);  
      stratK_HV_log = new G4LogicalVolume(stratK_HV, Kapton, "KHVlog");
    
  //---------------------------------- epoxy glue 2 - Araldite -----------------------------   
  G4Tubs* stratAraldite2 = new G4Tubs("stratAraldite1", KRint, KRext, wEpoxy2/2, KstartAngle, KtotAngle);  
      stratAraldite2_log = new G4LogicalVolume(stratAraldite2, araldite, "Araldite2log");
  
  //------------------------------------ carbon fiber support ------------------------------     
  G4Box* Cfiber = new G4Box("Cfiber", absorberX, absorberY, wCf/2);
      CfiberLog = new G4LogicalVolume(Cfiber, CarbonFiber, "CfiberLog");
        
 
  // start for assembly volume: Kapton fan-out, Al, glue, K_HV, epoxy_glue, Carbon_fiber .... (all but absorber plate)
  G4AssemblyVolume* assamblyKfo = new G4AssemblyVolume();
   
  G4ThreeVector Ta;
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( ZK1 );
  assamblyKfo->AddPlacedVolume( stratK1_log, Ta, 0 );                      // polyimide 1
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( ZAd1 );
  assamblyKfo->AddPlacedVolume( stratAd1_log, Ta, 0 );                     // adhesive 1
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( ZCu1 );
  assamblyKfo->AddPlacedVolume( stratCu1_log, Ta, 0 );                     // copper 1 
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( ZK2 );
  assamblyKfo->AddPlacedVolume( stratK2_log, Ta, 0 );                      // polyimide 2
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( ZAd2 );
  assamblyKfo->AddPlacedVolume( stratAd2_log, Ta, 0 );                     // adhesive 2
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( ZK3 );
  assamblyKfo->AddPlacedVolume( stratK3_log, Ta, 0 );                      // polyimide 3
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( ZAralditel1 );
  assamblyKfo->AddPlacedVolume( stratAraldite1_log, Ta, 0 );               // epoxy glue 1 - Araldite 
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( ZAl1 );
  assamblyKfo->AddPlacedVolume( stratAl1_log, Ta, 0 );                     // aluminuim foil in front of Si sensor
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( ZSi );               
  assamblyKfo->AddPlacedVolume( logicCalSensor, Ta, 0 );                   // Si layer 
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( ZAl2 );
  assamblyKfo->AddPlacedVolume( stratAl2_log, Ta, 0 );                     // aluminuim foil behind Si sensor
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( Zcg );
  assamblyKfo->AddPlacedVolume( stratconductive_glue_log, Ta, 0 );         // conductive glue
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( ZCu_HV );
  assamblyKfo->AddPlacedVolume( stratCu_HV_log, Ta, 0 );                  // copper
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( ZK_HV );
  assamblyKfo->AddPlacedVolume( stratK_HV_log, Ta, 0 );                    // polyimide 
  Ta.setX( translateX*mm ); Ta.setY( translateY*mm ); Ta.setZ( ZAralditel2 );
  assamblyKfo->AddPlacedVolume( stratAraldite2_log, Ta, 0 );               // epoxy glue 2 - Araldite
  Ta.setX( translateX*mm ); Ta.setY( tranTY*mm ); Ta.setZ( ZCf );
  assamblyKfo->AddPlacedVolume( CfiberLog, Ta, 0 );                       // carbon fiber support
  
  
  for( int i = 0; i < 15; i++ ) {
    G4ThreeVector Tm( 0,0,(wSlot*i)*mm);
    assamblyKfo->MakeImprint( worldLog, Tm, 0, 0, pSurfChk );
  }


  //======================================= visualization ========================================      
  // Invisible world volume
  worldLog->SetVisAttributes(G4VisAttributes::Invisible);
  // trigger - 2 scintillators
  scintplanLog->SetVisAttributes(G4Colour::Red());
  // telescope - 2 before and 3 after magnet
  for( int c = 0; c < 5; c++ ) teleplanLog[c]->SetVisAttributes(G4Colour::Green());
  // magnet
  //magnetLog->SetVisAttributes(G4Colour::Red());
  //----- Al foil
  AlfoilLog->SetVisAttributes(G4Colour::Gray());
  //----- tungsten absorber
  for( int l = 0; l < 16; l++ ) absorberLog[l]->SetVisAttributes(G4Colour::Blue());
  //----- Sensor layers
  stratK1_log ->SetVisAttributes(G4Colour::Green());
  stratAd1_log->SetVisAttributes(G4Colour::Magenta());
  stratCu1_log->SetVisAttributes(G4Colour::Blue());
  stratK2_log ->SetVisAttributes(G4Colour::Green());
  stratAd2_log->SetVisAttributes(G4Colour::Magenta());
  stratK3_log ->SetVisAttributes(G4Colour::Green());
  stratAraldite1_log->SetVisAttributes(G4Colour::Grey());  
  stratAl1_log->SetVisAttributes(G4Colour::Cyan());
  logicCalSensor->SetVisAttributes(G4VisAttributes::Invisible);
  logicCalZone->SetVisAttributes(G4VisAttributes::Invisible);
  fLogicCalPad->SetVisAttributes(G4Colour::Red());
  stratAl2_log->SetVisAttributes(G4Colour::Cyan());
  stratconductive_glue_log->SetVisAttributes(G4Colour::Brown());
  stratCu_HV_log->SetVisAttributes(G4Colour::Blue());
  stratK_HV_log->SetVisAttributes(G4Colour::Green());  
  stratAraldite2_log->SetVisAttributes(G4Colour::Grey());
  CfiberLog->SetVisAttributes(G4Colour::Yellow());  
    
    
  return worldPhys;
  
}

void DetectorConstruction::ConstructSDandField()
{
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    
    LumicalSD* SenDetLumical = new LumicalSD("sdLumical");
    sdManager -> AddNewDetector(SenDetLumical);
    fLogicCalPad->SetSensitiveDetector(SenDetLumical);
}
  
