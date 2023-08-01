#ifndef NeutronKeffective_h
#define NeutronKeffective_h 1

#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include <vector>

class G4GeneralParticleSource;

class NeutronKeffective
{
public:
  NeutronKeffective();
  ~NeutronKeffective();

  void KeffEvent(G4Event*, G4int);
  void AnalyseParticles(const G4Step*);
  void SaveSecondaryNeutronStats(const G4Track*, G4ThreeVector);
  std::vector<G4double> GetInitStats();
  std::vector<G4double> GetRandomSecondaryStats();
  void ClearDaughters();
  void CloneDaughters();
  void StoreNofSecondaries();
  void PrintKeff();
  void PrintAverageDist();


private:
  G4int batch;
  G4GeneralParticleSource* fGPS;
  G4int currentGen;
  G4int maxGen;
  G4int nofSec;
  G4ParticleDefinition* particle;
  G4double energy;
  G4ThreeVector position;
  G4ThreeVector direction;
  std::vector<G4double> initStats;
  std::vector<G4double> randomSecStats;
  std::vector<G4double> daughters;
  std::vector<G4double> clone_daughters;
  std::vector<G4double> secondaryNeutronVector;
};

#endif
