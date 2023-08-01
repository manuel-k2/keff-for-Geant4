#ifndef SteppingAction_h
#define SteppingAction_h 1

#include <G4UserSteppingAction.hh>
#include <globals.hh>
#include <G4TrackVector.hh>

class RunAction;
class NeutronKeffective;

class G4LogicalVolume;

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(RunAction*, NeutronKeffective*);
   ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*);

private:
    RunAction* fRunAction;
    NeutronKeffective* fKeffective;
};

#endif
