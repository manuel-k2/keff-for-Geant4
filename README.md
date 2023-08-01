# keff for Geant4
## Author: Manuel Kreutle, Zentrum für Naturwissenschaften und Friedensforschung, Universität Hamburg, Germany.


### Description:
This repository contains a custom extension for Geant4 [1] to calculate the effective neutron multiplication factor keff of a set-up.

It runs as a standalone simulation that takes the event number as the number of simulated neutron generations. The batch size can be set in the User action or as argument input ()

The algorithm has been applied and validated [2].

### How to implement:
- Add NeutronKeffective.cc to your src folder
- Add NeutronKeffective.hh to your include folder.
- Add NeutronKeffective as UserAction to ActionInitialization
- Add the following functions to UserActions:
  - KeffEvent(...) to PrimaryGeneratorAction::GeneratePrimaries(...)
  - AnalyseParticles(...) to SteppinAction::UserSteppingAction(...)
  - PrintKeff(...) to RunAction::EndOfRunAction(...)

### Example project:
The implementation of the NeutronKeffective UserAction can also be observed in the example_project that simulates the criticality experiment MIXCOMPMIXED001 Case 1 from the NEA International Handbook of Evaluated Criticality Safety Benchmark Experiments [3].

It is built as the general Geant4 example projects (execute "cmake .." and "make" in build folder) [4].

Set number of generations as number of events in run.mac in build folder:
"run/beamOn XX"
with XX the number of simulated fission generations.

Execute in the build folder:
"./fission run.mac 1 XXX"
with XXX the batch size.

### License:
tba

### References:
[1] https://geant4.web.cern.ch/
[2] tba
[3] OECD Nuclear Energy Agency, International handbook of evaluated criticality safety benchmark experiments, NEA/NSC/DOC(95)/03 (2016).
[4] https://geant4-userdoc.web.cern.ch/Doxygen/examples_doc/html/index.html

### Address questions to the following email: mkreutle@physnet.uni-hamburg.de
