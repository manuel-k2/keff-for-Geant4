#include "DetectorConstruction.hh"

#include <G4RunManager.hh>
#include <G4GeometryManager.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SolidStore.hh>
#include <G4NistManager.hh>

#include <G4Box.hh>
#include <G4Cons.hh>
#include <G4Tubs.hh>
#include <G4Orb.hh>
#include <G4Sphere.hh>
#include <G4Trd.hh>

#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4RotationMatrix.hh>
#include <G4Transform3D.hh>
#include <G4SDManager.hh>
#include <G4MultiFunctionalDetector.hh>
#include <G4VPrimitiveScorer.hh>
#include <G4SDParticleFilter.hh>
#include <G4SDChargedFilter.hh>
#include <G4PSDoseDeposit.hh>
#include <G4PSNofStep.hh>
#include <G4PSPopulation.hh>
#include <G4PSNofSecondary.hh>
#include <G4VisAttributes.hh>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>
#include <G4PhysicalConstants.hh>
#include <vector>
#include <sstream>


using namespace std;

DetectorConstruction::DetectorConstruction()
 :G4VUserDetectorConstruction()
{
  CreateMaterials();
}

DetectorConstruction::~DetectorConstruction()
{ }

void DetectorConstruction::CreateMaterials()
{
  // Create materials

  G4NistManager* nist = G4NistManager::Instance();
  world_mat = nist->FindOrBuildMaterial("G4_AIR");
  materialVector.push_back(world_mat);

  // Create fissile material
  G4Isotope* U235 = new G4Isotope("Uraniumium235", 92, 235, 235.043 * g/mole);
  G4Isotope* U238 = new G4Isotope("Uraniumium238", 92, 238, 238.051 * g/mole);

  G4Isotope* Pu238 = new G4Isotope("Plutonium238", 94, 238, 238.050 * g/mole);
  G4Isotope* Pu239 = new G4Isotope("Plutonium239", 94, 239, 239.052 * g/mole);
  G4Isotope* Pu240 = new G4Isotope("Plutonium240", 94, 240, 240.054 * g/mole);
  G4Isotope* Pu241 = new G4Isotope("Plutonium241", 94, 241, 241.057 * g/mole);
  G4Isotope* Pu242 = new G4Isotope("Plutonium242", 94, 242, 242.059 * g/mole);

  G4Isotope* Am241 = new G4Isotope("Americium241", 95, 241, 241.057 * g/mole);

  G4Element* U_235 = new G4Element("U_235", "U235", 1);
  U_235->AddIsotope(U235, 100.0*perCent);
  G4Element* U_238 = new G4Element("U_238", "U238", 1);
  U_238->AddIsotope(U238, 100.0*perCent);

  G4Element* Pu_238 = new G4Element("Pu_238", "Pu238", 1);
  Pu_238->AddIsotope(Pu238, 100.0*perCent);
  G4Element* Pu_239 = new G4Element("Pu_239", "Pu239", 1);
  Pu_239->AddIsotope(Pu239, 100.0*perCent);
  G4Element* Pu_240 = new G4Element("Pu_240", "Pu240", 1);
  Pu_240->AddIsotope(Pu240, 100.0*perCent);
  G4Element* Pu_241 = new G4Element("Pu_241", "Pu241", 1);
  Pu_241->AddIsotope(Pu241, 100.0*perCent);
  G4Element* Pu_242 = new G4Element("Pu_242", "Pu242", 1);
  Pu_242->AddIsotope(Pu242, 100.0*perCent);

  G4Element* Am_241 = new G4Element("Am_241", "Am241", 1);
  Am_241->AddIsotope(Am241, 100.0*perCent);

  G4Element* elH = new G4Element("TS_H_of_Polyethylene","H", 1, 1.007 * g/mole);
  G4Element* elC = new G4Element("Carbon", "C", 6, 12.001 * g/mole);
  G4Element* elO = new G4Element("Oxygen","O", 8, 15.999 * g/mole);

  /*MOX = new G4Material("MOX1", 4.53*g/cm3, 11);
  MOX->AddElement(U_235, 0.08 *perCent);
  MOX->AddElement(U_238, 53.85 *perCent);
  MOX->AddElement(Pu_238, 0.02 *perCent);
  MOX->AddElement(Pu_239, 19.31 *perCent);
  MOX->AddElement(Pu_240, 2.58 *perCent);
  MOX->AddElement(Pu_241, 0.44 *perCent);
  MOX->AddElement(Pu_242, 0.05 *perCent);
  MOX->AddElement(Am_241, 0.09 *perCent);
  MOX->AddElement(elH, 0.89 *perCent);
  MOX->AddElement(elC, 11.73 *perCent);
  MOX->AddElement(elO, 10.96 *perCent);
  materialVector.push_back(MOX);*/

  MOX = new G4Material("MOX1", 4.53*g/cm3, 11, kStateSolid, 293.15*kelvin);
  MOX->AddElement(U_235, 0.08 *perCent);
  MOX->AddElement(U_238, 53.85 *perCent);
  MOX->AddElement(Pu_238, 0.02 *perCent);
  MOX->AddElement(Pu_239, 19.25 *perCent);
  MOX->AddElement(Pu_240, 2.58 *perCent);
  MOX->AddElement(Pu_241, 0.46 *perCent);
  MOX->AddElement(Pu_242, 0.06 *perCent);
  MOX->AddElement(Am_241, 0.11 *perCent);
  MOX->AddElement(elH, 0.90 *perCent);
  MOX->AddElement(elC, 11.73 *perCent);
  MOX->AddElement(elO, 10.96 *perCent);
  materialVector.push_back(MOX);
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  if (!materialVector.size()) CreateMaterials();

  // Clear old geometry, if any

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // Checking of volumes overlaps

  G4bool checkOverlaps = true;

  // Create world

  G4double world_hx = 1.0*m;
  G4double world_hy = 1.0*m;
  G4double world_hz = 3.0*m;

  G4Box* solidWorld
    = new G4Box("World", world_hx, world_hy, world_hz);

  G4LogicalVolume* logicWorld
    = new G4LogicalVolume(solidWorld, world_mat, "World");

  G4VisAttributes* visAttr = new G4VisAttributes();
  visAttr->SetVisibility(false);
  logicWorld->SetVisAttributes(visAttr);

  G4VPhysicalVolume* physWorld
    = new G4PVPlacement(0,                     //no rotation
                        G4ThreeVector(),       //at (0,0,0)
                        logicWorld,            //its logical volume
                        "World",               //its name
                        0,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);

  // Create MOX block

  // Case 1
  G4double mox_hx = 50.9*cm/2;
  G4double mox_hy = 50.83*cm/2;
  G4double mox_hz = 45.24*cm/2;

  // Case 2
  //G4double mox_hx = 50.9*cm/2;
  //G4double mox_hy = 55.913*cm/2;
  //G4double mox_hz = 42.003*cm/2;

  G4Box* solidMOX
    = new G4Box("MOX block", mox_hx, mox_hy, mox_hz);

  G4LogicalVolume* logicMOX
    = new G4LogicalVolume(solidMOX, MOX, "MOX block");

  physMOX
   = new G4PVPlacement(0,
                      G4ThreeVector(),
                      logicMOX,
                      "MOX block",
                      logicWorld,
                      false,
                      0,
                      checkOverlaps);


  // Always return the physical World
  return physWorld;
}
