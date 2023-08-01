// package includes
//
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

// geant4 includes
//
#include <G4MTRunManager.hh>
#include <G4RunManager.hh>

#ifdef G4UI_USE
    #include <G4UIExecutive.hh>
#endif
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

// visualization
//
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

// misc includes
//
#include <fstream>
#include <math.h>
#include "G4ios.hh"
#include <Randomize.hh>
#include <stdlib.h>

// system time to seed
//
#include <time.h>

// include globals
#include "my_globals.h"
G4String taskID;
G4int batchSize;


// print files
#include <fstream>

//------------------------------------------------------------------------------
int main(int argc,char** argv)
{
  bool interactive=false;
  G4String multithreadFlag = "-mt";
  G4String filename;
  taskID="";
  char *temp;
  batchSize=1;

  //....Default: run interactive session
  //....First option: run macro-file
  //....Second option: set taskID
  //....Third option: set batch size
  if (argc==1)
    interactive = true;
  if (argc==2)
  {
    if (G4StrUtil::contains(G4String(argv[1]),".mac"))
      filename = G4String(argv[1]);
    else taskID = G4String(argv[1]);
  }
  if (argc==3)
  {
    if (G4StrUtil::contains(G4String(argv[1]),".mac"))
    {
      filename = G4String(argv[1]);
      taskID = G4String(argv[2]);
    }
    else
    {
      taskID = G4String(argv[1]);
      batchSize = strtol(argv[2],&temp,10);
    }
  }
  if (argc==4)
  {
    filename = G4String(argv[1]);
    taskID = G4String(argv[2]);
    batchSize = strtol(argv[3],&temp,10);
  }

  // Create seed from seconds passed since 1 January 2000
  //-----------------------------
  time_t timer;
  struct tm y2k = {0,0,0,0,0,0,0,0,0,0,0};
  double seconds;

  y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
  y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;
  time(&timer);

  seconds = difftime(timer,mktime(&y2k));
  int seed = (int) seconds;
  int add = (int) G4UniformRand()*atof(taskID.c_str());
  seed+=add;

  // Choose the Random engine
  //-------------------------
  CLHEP::RanecuEngine* ran_eng = new CLHEP::RanecuEngine;
  ran_eng->setSeed(seed);
  G4Random::setTheEngine(ran_eng);


  // Run manager
  //------------
  G4RunManager* theRunManager = new G4RunManager;

  // UserInitialization classes
  //---------------------------
  DetectorConstruction* theDetector = new DetectorConstruction;
  PhysicsList* thePhysicsList = new PhysicsList;

  theRunManager->SetUserInitialization(theDetector);
  theRunManager->SetUserInitialization(thePhysicsList);

  // UserAction classes
  //-------------------
  theRunManager->SetUserInitialization(new ActionInitialization(theDetector));

  #ifdef G4VIS_USE
    // Visualization, if you choose to have it!
    //-----------------------------------------
    G4VisManager* theVisManager = new G4VisExecutive;
    theVisManager->Initialize();
  #endif

  // Initialize G4 kernel
  //---------------------
  theRunManager->Initialize();

  // User interactions
  //------------------
  if(filename)
    G4UImanager::GetUIpointer()->ApplyCommand("/control/execute "+filename);

  //....Start an interactive session
  if(interactive)
  {
    G4cout << "\nType 'exit' to end the program.\n";
    G4UIterminal(new G4UItcsh).SessionStart();
  }

  #ifdef G4VIS_USE
    delete theVisManager;
  #endif
  delete theRunManager;
  return EXIT_SUCCESS;
}
