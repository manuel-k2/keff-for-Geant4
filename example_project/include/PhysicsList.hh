#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicsList: public G4VModularPhysicsList
{
  public:
    PhysicsList();
    ~PhysicsList();

  protected:
    void ConstructParticle();
    void SetCuts();

  public:
    void SetModPhysList(G4bool flag) {fModPhysList = flag;};

  private:
    G4bool            fModPhysList;

};

#endif
