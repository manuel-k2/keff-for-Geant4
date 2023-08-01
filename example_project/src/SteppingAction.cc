#include "SteppingAction.hh"

#include "RunAction.hh"
#include "NeutronKeffective.hh"

#include <G4TransportationManager.hh>
#include <G4VProcess.hh>
#include <G4Step.hh>
#include <G4Track.hh>
#include <G4TrackVector.hh>
#include <G4IonTable.hh>
#include <G4Neutron.hh>
#include <G4Alpha.hh>
#include <G4Gamma.hh>
#include <G4LogicalVolume.hh>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

SteppingAction::SteppingAction(RunAction* runAction, NeutronKeffective* kEffective)
: G4UserSteppingAction(),
  fRunAction(runAction), fKeffective(kEffective)
{ }

SteppingAction::~SteppingAction()
{ }

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->SetPushVerbosity(0);

  // Search for secondary neutron tracks resulting from fission and (n,Xn) reactions (kill all other tracks)

  fKeffective->AnalyseParticles(step);

  // Count neutrons leaving the world

  G4StepStatus status = step->GetPostStepPoint()->GetStepStatus();
  if (status != fWorldBoundary) return;
  //if (status != fGeomBoundary) return;

  G4Track* track = step->GetTrack();
  const G4ParticleDefinition* particle = track->GetParticleDefinition();
  if (particle == G4Neutron::Definition())
  {
    fRunAction->AddLostNeutron();
  }

  track->SetTrackStatus(fStopAndKill);
}
