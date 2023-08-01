#include "RunAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "NeutronKeffective.hh"

#include <G4AccumulableManager.hh>

#include <G4Track.hh>
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4Alpha.hh>
#include <G4Neutron.hh>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>

#include "my_globals.h"

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* primGen, NeutronKeffective* kEffective)
: G4UserRunAction(),
  fDetector(det),
  fPrimGen(primGen),
  fKeffective(kEffective),
  fClearIt(false),
  fLostNeutrons("LostNeutrons", 0),
  fCounter1("Counter 1",0),
  fCounter2("Counter 2",0)
{
  // Register accumulables
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fLostNeutrons);
  accumulableManager->RegisterAccumulable(fCounter1);
  accumulableManager->RegisterAccumulable(fCounter2);
}

RunAction::~RunAction()
{ }

void RunAction::BeginOfRunAction(const G4Run*)
{
  // Reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

  // Clear files
  if(fClearIt)
  {
    std::ofstream ofs1 ("MIXCOMPMIXED001-1_Keff_bs"+std::to_string(batchSize)+"_id"+taskID+".txt", std::ios::out | std::ios::trunc);
    ofs1.close ();

    std::ofstream ofs2 ("MIXCOMPMIXED001-1_Dist_bs"+std::to_string(batchSize)+"_id"+taskID+".txt", std::ios::out | std::ios::trunc);
    ofs2.close ();
  }

  PrintParameters();
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  // Retrieve the number of generations (events) in the run
  G4int nofEvents = run->GetNumberOfEvent();

  // Do nothing, if no events were processed
  if (nofEvents == 0) return;

  // Merge accumulables
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Print files
  fKeffective->PrintKeff();

  if (IsMaster())
  {
    // Print
    G4cout
     << "\n--------------------End of Global Run-----------------------"
     << " \n The run was " << nofEvents << " generations (events) with a batch size of " << batchSize << G4endl;

    G4cout << "\nNumber of lost Neutrons: " << fLostNeutrons.GetValue() << " (Leakage fraction: " << fLostNeutrons.GetValue()/nofEvents << ")"

     << "\n\nCounter 1: " << fCounter1.GetValue()
     << "\n\nCounter 2: " << fCounter2.GetValue()
     << "\n"
     << G4endl;
  }
}

void RunAction::ClearFiles(G4bool value)
{
  if (value) fClearIt = true;
  else fClearIt = false;
}

void RunAction::PrintParameters()
{
  G4String value1 = "false";
  if (fClearIt) value1 = "true";
  G4cout
  << "Files cleared before run: " << value1
  << "\n"
  << G4endl;
}
