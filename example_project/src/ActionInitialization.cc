#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "NeutronKeffective.hh"


ActionInitialization::ActionInitialization(DetectorConstruction* detector)
 : G4VUserActionInitialization(),
   fDetector(detector)
{ }

ActionInitialization::~ActionInitialization()
{ }

void ActionInitialization::BuildForMaster() const
{
  NeutronKeffective* kEffective = new NeutronKeffective();
  PrimaryGeneratorAction* thePrimGenAction = new PrimaryGeneratorAction(fDetector, kEffective);
  RunAction* theRunAction = new RunAction(fDetector, thePrimGenAction, kEffective);
  SetUserAction(theRunAction);
}

void ActionInitialization::Build() const
{
  NeutronKeffective* kEffective = new NeutronKeffective();
  PrimaryGeneratorAction* thePrimGenAction = new PrimaryGeneratorAction(fDetector, kEffective);
  SetUserAction(thePrimGenAction);
  RunAction* theRunAction = new RunAction(fDetector, thePrimGenAction, kEffective);
  SetUserAction(theRunAction);
  SetUserAction(new SteppingAction(theRunAction, kEffective));
}
