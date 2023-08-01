#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "NeutronKeffective.hh"

#include "G4RunManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SPSPosDistribution.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4Box.hh"
#include "Randomize.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <numeric>

#include "my_globals.h"

//----------------------------------------------------------------------------//
PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det, NeutronKeffective* kEffective)
: fKeffective(kEffective), fDetector(det)
  
{
  SetSource("KeffSource");
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{ }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if (fType == "keff")
    fKeffective->KeffEvent(anEvent, batchSize);
}

void PrimaryGeneratorAction::PrintParameters()
{
  if (fType)
    G4cout << "\nSource type: " << fType << G4endl;
}

void PrimaryGeneratorAction::SetSource(G4String sourceChoice)
{
  if (sourceChoice == "KeffSource") {fType = "keff";}
  else
  {
    G4cout << "\n--> warning from PrimaryGeneratorAction::SetSource : "
          << sourceChoice << " not found."
          << "\nChoose from valid options: KeffSource" << G4endl;
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

G4String PrimaryGeneratorAction::GetSource()
{
G4String sourceType;
if (fType == "keff")
  sourceType = "KeffSource";
return sourceType;
}
