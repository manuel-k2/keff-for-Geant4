#include "NeutronHPphysics.hh"

#include "PhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "G4SystemOfUnits.hh"

// Processes

#include "G4ParticleDefinition.hh"
#include "G4HadronElasticProcess.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPThermalScatteringData.hh"
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPThermalScattering.hh"

#include "G4HadronInelasticProcess.hh"
#include "G4NeutronHPInelasticData.hh"
#include "G4NeutronHPInelastic.hh"

#include "G4NeutronCaptureProcess.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPCapture.hh"

#include "G4NeutronFissionProcess.hh"
#include "G4NeutronHPFissionData.hh"
#include "G4NeutronHPFission.hh"

#include "G4FissLib.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronHPphysics::NeutronHPphysics(const G4String& name)
:  G4VPhysicsConstructor(name), fThermal(true)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutronHPphysics::~NeutronHPphysics()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronHPphysics::ConstructProcess()
{
   G4ProcessManager* pManager = G4Neutron::Neutron()->GetProcessManager();

   // process: elastic
   //
   G4HadronElasticProcess* theElasticProcess = new G4HadronElasticProcess();
   pManager->AddDiscreteProcess(theElasticProcess);
   //
   // cross section data set
   G4NeutronHPElasticData* dataSet1a = new G4NeutronHPElasticData();
   G4NeutronHPThermalScatteringData* dataSet1b
                               = new G4NeutronHPThermalScatteringData();
   theElasticProcess->AddDataSet(dataSet1a);
   if (fThermal) theElasticProcess->AddDataSet(dataSet1b);
   //
   // models
   G4NeutronHPElastic*           model1a = new G4NeutronHPElastic();
   G4NeutronHPThermalScattering* model1b = new G4NeutronHPThermalScattering();
   if (fThermal)  model1a->SetMinEnergy(4*eV);
   theElasticProcess->RegisterMe(model1a);
   if (fThermal) theElasticProcess->RegisterMe(model1b);

   // process: inelastic
   //
   G4ParticleDefinition* neutron = G4Neutron::Definition();
   G4HadronInelasticProcess* theInelasticProcess = new G4HadronInelasticProcess("neutronInelastic",neutron);
   pManager->AddDiscreteProcess(theInelasticProcess);
   //
   // cross section data set
   G4NeutronHPInelasticData* dataSet2 = new G4NeutronHPInelasticData();
   theInelasticProcess->AddDataSet(dataSet2);
   //
   // models
   G4NeutronHPInelastic* model2a = new G4NeutronHPInelastic();
   theInelasticProcess->RegisterMe(model2a);

   // process: nCapture
   //
   G4NeutronCaptureProcess* theCaptureProcess = new G4NeutronCaptureProcess();
   pManager->AddDiscreteProcess(theCaptureProcess);
   //
   // cross section data set
   G4NeutronHPCaptureData* dataSet3 = new G4NeutronHPCaptureData();
   theCaptureProcess->AddDataSet(dataSet3);
   //
   // models
   G4NeutronHPCapture* model3 = new G4NeutronHPCapture();
   theCaptureProcess->RegisterMe(model3);

   // process: nFission
   //
   G4NeutronFissionProcess* theFissionProcess = new G4NeutronFissionProcess();
   pManager->AddDiscreteProcess(theFissionProcess);
   //
   // cross section data set
   G4NeutronHPFissionData* dataSet4 = new G4NeutronHPFissionData();
   theFissionProcess->AddDataSet(dataSet4);
   //
   // models
   G4NeutronHPFission* theFissionModel = new G4NeutronHPFission();
   //G4FissLib* theFissionModel = new G4FissLib;
   theFissionProcess->RegisterMe(theFissionModel);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double NeutronHPphysics::rng4llnlfisslib(void)
{
  return G4UniformRand();
}
