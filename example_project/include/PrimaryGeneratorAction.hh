#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "Randomize.hh"
#include "vector"

#include <globals.hh>

class DetectorConstruction;
class NeutronKeffective;

class G4GeneralParticleSource;
class G4DataVector;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*, NeutronKeffective*);
    ~PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* anEvent);
    void CreateIonSource(G4int, G4int);
    void PrintParameters();
    void SetSource(G4String);
    G4String GetSource();

  private:
    DetectorConstruction*  fDetector;
    NeutronKeffective* fKeffective;
    G4String fType;
};

#endif
