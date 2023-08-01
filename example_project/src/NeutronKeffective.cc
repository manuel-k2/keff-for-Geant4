#include "NeutronKeffective.hh"

#include <G4Event.hh>
#include <G4Track.hh>
#include <G4Step.hh>
#include <G4VProcess.hh>
#include <G4Neutron.hh>

#include <G4GeneralParticleSource.hh>

#include <G4ParticleTable.hh>

#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>

#include "my_globals.h"

NeutronKeffective::NeutronKeffective()
:batch(0), fGPS(0), currentGen(0), maxGen(999),
nofSec(0), energy(0), position(0), direction(0),
randomSecStats(0), daughters(0), clone_daughters(0)
{
  fGPS = new G4GeneralParticleSource();
  particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
}

NeutronKeffective::~NeutronKeffective()
{
  delete fGPS;
}

void NeutronKeffective::KeffEvent(G4Event* anEvent, G4int setBatchSize)
{
  // Particle source for keff calculation
  // Number of events controls number of simulated generations
  // Implement this function in PrimaryGeneratorAction::GeneratePrimaries(...)

  // Set batch size
  batch = setBatchSize;

  // Generate initial primary vertices
  if (currentGen == 0)
  {
    G4cout << "Current generation: " << currentGen << G4endl;

    fGPS->SetParticleDefinition(particle);

    // Set inital neutron statistics as primmary neutron statistics
    for(unsigned int i = 0; i < batch; i++)
    {
      initStats = GetInitStats();

      energy = initStats[0];
      position = G4ThreeVector(initStats[1],initStats[2],initStats[3]);
      direction = G4ThreeVector(initStats[4],initStats[5],initStats[6]);
      initStats.clear();

      fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(energy);
      fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(position);
      fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(direction);
      fGPS->GeneratePrimaryVertex(anEvent);
    }

    currentGen = 1;
  }
  // Generate primary vertices based on secondary neutron statistics of previous generation
  else if (currentGen <= maxGen)
  {
    G4cout << "Current generation: " << currentGen << G4endl;

    // Store number of secondaries of previous generation
    StoreNofSecondaries();

    // Get set of random secondary neutron statistics
    std::vector<std::vector<G4double>> randomSecStatsVector;
    for(unsigned int i = 0; i < batch; i++)
    {
      randomSecStatsVector.push_back(GetRandomSecondaryStats());
      randomSecStats.clear();
    }

    // If there were no secondaries created in the previous event
    // clone dautghters for primary neutron statistics

    if (nofSec == 0)
    {
      CloneDaughters();
      G4cout << "No secondary neutrons were created in previous generation.\n"
             << "Use statistics of generation before." << G4endl;
    }

    // Clear daughters vector before starting new event

    ClearDaughters();


    // Set random previous secondary neutron statistics as new primmary neutron statistics
    for(unsigned int i = 0; i < batch; i++)
    {
      randomSecStats = randomSecStatsVector[i];

      energy = randomSecStats[0];
      position = G4ThreeVector(randomSecStats[1], randomSecStats[2], randomSecStats[3]);
      direction = G4ThreeVector(randomSecStats[4], randomSecStats[5], randomSecStats[6]);

      fGPS->GetCurrentSource()->GetEneDist()->SetMonoEnergy(energy);
      fGPS->GetCurrentSource()->GetPosDist()->SetCentreCoords(position);
      fGPS->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(direction);

      //G4cout << "Kinetic Energy in MeV RECEIVED: " << energy/MeV << G4endl;
      //G4cout << "Position RECEIVED: " << position << G4endl;
      //G4cout << "Momentum direction RECEIVED: " << direction << G4endl;

      fGPS->GeneratePrimaryVertex(anEvent);
    }

    randomSecStatsVector.clear();

    currentGen += 1;
  }
  else if (currentGen == maxGen)
  {
    G4cout << "Maximum generations reached (maxGen = " << maxGen << ")" << G4endl;
  }

  return;
}

void NeutronKeffective::AnalyseParticles(const G4Step* step)
{
  // Save secondary neutron statistics from fission and (n,Xn) reactions for next generation's primary neutrons and kill them if saved
  // Implement this function in SteppinAction::UserSteppingAction(...)

  G4Track* track = step->GetTrack();
  G4String particle = track->GetDefinition()->GetParticleName();

  // Kill all non-neutrons
  if (particle != "neutron")
  {
    track->SetTrackStatus(fKillTrackAndSecondaries);
    return;
  }

  // Save postiion of reaction
  G4ThreeVector pos = step->GetPostStepPoint()->GetPosition();

  // Check for secondary particles in current step
  const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();

  const G4Track* heldBackTrack;
  G4int secondaryNeutronCount = 0;
  G4bool markedForDeletion = false;

  // Iterate over secondary particles
  std::vector<const G4Track*>::const_iterator it;
  for (it = secondaries->begin(); it != secondaries->end(); it++)
  {
    G4String secondaryParticle = (*it)->GetDefinition()->GetParticleName();

    // Check for secondary neutrons
    if (secondaryParticle == "neutron")
    {
      G4String creatorProcess = (*it)->GetCreatorProcess()->GetProcessName();

      // Check cor fission reaction as source of secondary neutrons
      if (creatorProcess == "nFission")
      {
        SaveSecondaryNeutronStats((*it), pos);
        // Mark track for later deletion
        markedForDeletion = true;
      }
      // Check for inelastic neutron reaction as source of secondary neutrons
      else if (creatorProcess == "neutronInelastic")
      {
        // Hold back first secondary neutron track
        if (secondaryNeutronCount == 0)
        {
          heldBackTrack = (*it);
          secondaryNeutronCount +=1;
        }
        // If there is more then one secondary neutron, (n,Xn) reaction detected
        // Add held back neutron's and additional neutrons' statistic
        else if (secondaryNeutronCount == 1)
        {
          SaveSecondaryNeutronStats(heldBackTrack, pos);
          SaveSecondaryNeutronStats((*it), pos);
          // Mark track for later deletion
          markedForDeletion = true;
          secondaryNeutronCount +=1;
        }
        else if (secondaryNeutronCount > 1)
        {
          SaveSecondaryNeutronStats((*it), pos);
          secondaryNeutronCount +=1;
        }
      }
    }
  }

  // Kill tracks whose secondary neutron statistics have been saved
  if (markedForDeletion)
    track->SetTrackStatus(fKillTrackAndSecondaries);

  return;
}

void NeutronKeffective::SaveSecondaryNeutronStats(const G4Track* track, G4ThreeVector pos)
{
  // Get kinetic energy and momentum direction of secondary neutrons
  G4double kinE = track->GetDynamicParticle()->GetKineticEnergy();
  G4ThreeVector dir = track->GetDynamicParticle()->GetMomentumDirection();

  //G4cout << "Kinetic Energy in MeV SAVED: " << kinE/MeV << G4endl;
  //G4cout << "Position SAVED: " << pos << G4endl;
  //G4cout << "Momentum direction SAVED: " << dir << G4endl;

  // Store secondary neutron statistics in daughters vector
  daughters.push_back(kinE);
  for (unsigned int i = 0; i < 3; ++i) daughters.push_back(pos[i]);
  for (unsigned int i = 0; i < 3; ++i) daughters.push_back(dir[i]);

  return;
}

std::vector<G4double> NeutronKeffective::GetInitStats()
{
  // Get statistics of an initial neutron

  G4double kinE = 2e-7*MeV;

  G4double x, y, z = 0;

  G4double psi = twopi*G4UniformRand();
  G4double theta  = std::acos(2*G4UniformRand()-1);
  G4double dx = std::sin(theta)*std::cos(psi);
  G4double dy = std::sin(theta)*std::sin(psi);
  G4double dz = std::cos(theta);

  initStats = {kinE, x, y, z, dx, dy, dz};
  return initStats;
}

std::vector<G4double> NeutronKeffective::GetRandomSecondaryStats()
{
  // Get statistics of a random secondary neutron

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, nofSec);

  // Create a vector containing random secondary neutron statistics
  G4int randomSecIndex = distr(gen);
  for (unsigned int i = 0; i < 7; i++)
  {
    if (nofSec == 0)
      randomSecStats.push_back(clone_daughters[randomSecIndex*7 + i]);
    else
      randomSecStats.push_back(daughters[randomSecIndex*7 + i]);
  }

  //G4cout << "Kinetic Energy in MeV SENT: " << randomSecStats[0]/MeV << G4endl;
  //G4cout << "Postion SENT: " << G4ThreeVector(randomSecStats[1], randomSecStats[2], randomSecStats[3]) << G4endl;
  //G4cout << "Momentrum direction SENT: " << G4ThreeVector(randomSecStats[4], randomSecStats[5], randomSecStats[6]) << G4endl;

  return randomSecStats;
}

void NeutronKeffective::StoreNofSecondaries()
{
  // Store current generation's number of secondary neutrons in a vector

  nofSec = daughters.size()/7;
  secondaryNeutronVector.push_back(nofSec);
  G4cout << "Number of secondary neutrons: " << nofSec << G4endl;

  return;
}

void NeutronKeffective::ClearDaughters()
{
  // Clear vector with secondary neutron statistics

  daughters.clear();
}

void NeutronKeffective::CloneDaughters()
{
  // Clone vector with secondary neutron statistics

  clone_daughters = daughters;
}

void NeutronKeffective::PrintKeff()
{
  // Print keff info from vectors in files
  // Implement this function in RunAction::EndOfRunAction(...)

  std::fstream outfile_keff;
  outfile_keff.open("Keff_bs"+std::to_string(batchSize)
                    +"_gen"+std::to_string(maxGen)
                    +".txt", std::ios::app);
  for (unsigned int i = 0; i < secondaryNeutronVector.size(); ++i)
    outfile_keff << secondaryNeutronVector[i] << G4endl;
  outfile_keff.close();
  return;
}
