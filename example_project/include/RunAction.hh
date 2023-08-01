#ifndef RunAction_h
#define RunAction_h 1

#include <G4UserRunAction.hh>
#include <G4Run.hh>
#include <G4TrackVector.hh>
#include <G4ParticleDefinition.hh>
#include <G4Accumulable.hh>
#include <globals.hh>

class DetectorConstruction;
class PrimaryGeneratorAction;
class NeutronKeffective;

class RunAction : public G4UserRunAction
{
public:
  RunAction(DetectorConstruction*,PrimaryGeneratorAction*, NeutronKeffective*);
  ~RunAction();

public:
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

  void AddLostNeutron()             {fLostNeutrons += 1;};
  void AddCounter1()                {fCounter1 +=1;}
  void AddCounter2()                {fCounter2 +=1;}

  void ClearFiles(G4bool);
  void PrintParameters();

private:
  DetectorConstruction*   fDetector;
  PrimaryGeneratorAction* fPrimGen;
  NeutronKeffective*      fKeffective;
  G4bool                  fClearIt;
  G4String                source;

private:
  G4Accumulable<G4double>    fLostNeutrons;
  G4Accumulable<G4double>    fCounter1;
  G4Accumulable<G4double>    fCounter2;
};

#endif
