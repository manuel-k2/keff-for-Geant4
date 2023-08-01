#ifndef NeutronHPphysics_h
#define NeutronHPphysics_h 1

#include "G4VPhysicsConstructor.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NeutronHPphysics : public G4VPhysicsConstructor
{
  public:
    NeutronHPphysics(const G4String& name="neutron");
   ~NeutronHPphysics();

  public:
    virtual void ConstructParticle() { };
    virtual void ConstructProcess();

  public:
    void SetThermalPhysics(G4bool flag) {fThermal = flag;};

  private:
    G4bool  fThermal;

    static double rng4llnlfisslib(void);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
