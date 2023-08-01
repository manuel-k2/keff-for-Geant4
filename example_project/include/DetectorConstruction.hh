#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class DetectorMessenger;

class G4Material;
class G4VPhysicalVolume;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:
    virtual void CreateMaterials();
    virtual G4VPhysicalVolume* Construct();
    virtual G4VPhysicalVolume* GetPhysVol() {return physMOX;};

  private:
    std::vector<G4Material*> materialVector;
    G4Material*  world_mat;
    G4Material*  MOX;
    G4VPhysicalVolume* physMOX;
};

#endif
