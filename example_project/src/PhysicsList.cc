#include "PhysicsList.hh"

#include "NeutronHPphysics.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <G4EmStandardPhysics.hh>
#include <G4DecayPhysics.hh>
#include <G4RadioactiveDecayPhysics.hh>
#include <G4EmLivermorePhysics.hh>
#include <G4EmExtraPhysics.hh>
#include <G4HadronPhysicsFTFP_BERT_HP.hh>
#include <G4HadronElasticPhysicsHP.hh>

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "globals.hh"
#include <iomanip>

//----------------------------------------------------------------------------//
PhysicsList::PhysicsList():  G4VModularPhysicsList(), fModPhysList(false)
{
  defaultCutValue = 0.01*mm;
  G4int ver = 1;
  SetVerboseLevel(ver);

  // General Physics
  RegisterPhysics(new NeutronHPphysics("neutronHP"));

}

//----------------------------------------------------------------------------//
PhysicsList::~PhysicsList()
{}

//----------------------------------------------------------------------------//
// Create all particles that can appear in this application.
//----------------------------------------------------------------------------//
void PhysicsList::ConstructParticle()
{
  G4BosonConstructor pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();
}

//----------------------------------------------------------------------------//
void PhysicsList::SetCuts()
{
  // set cut values for gamma and neutron
  SetCutValue(defaultCutValue, "gamma");
  SetCutValue(defaultCutValue, "neutron");

  //  SetCutsWithDefault();
  if (verboseLevel > 0) DumpCutValuesTable();
}
