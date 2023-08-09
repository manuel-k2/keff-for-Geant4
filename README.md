# keff for Geant4

## Author:
    Manuel Kreutle
    Zentrum für Naturwissenschaften und Friedensforschung,
    Universität Hamburg, Germany.
    Contact: mkreutle@physnet.uni-hamburg.de

### Citation:
When using this code please cite the reference below to give credit to the work that went into developping this code:  
  * M. Kreutle, A. Borella, S. Hebel, G. Kirchner, Validating Geant4 with Monte Carlo simulations in the context of nuclear disarmament verification, submitted to Nuclear Instruments and Methods in Physics Research Section A (2023)

### Description:
This repository contains a custom extension for Geant4 [1] to calculate the effective neutron multiplication factor keff of a set-up.

It runs as a standalone simulation that takes the event number as the number of simulated neutron generations. The batch size can be set in the User action or as argument input.

### How to implement:
- Add NeutronKeffective.cc to your src folder
- Add NeutronKeffective.hh to your include folder.
- Add NeutronKeffective as UserAction to ActionInitialization
- Add the following functions to UserActions:
  - KeffEvent(...) to PrimaryGeneratorAction::GeneratePrimaries(...)
  - AnalyseParticles(...) to SteppinAction::UserSteppingAction(...)
  - PrintKeff(...) to RunAction::EndOfRunAction(...)

### Example project:
The implementation of the NeutronKeffective UserAction can also be observed in the example_project that simulates the criticality experiment MIXCOMPMIXED001 Case 1 from the NEA International Handbook of Evaluated Criticality Safety Benchmark Experiments [2]. It is built as the general Geant4 example projects (execute "cmake .." and "make" in build folder) [3].

Set number of generations as number of events in run.mac in build folder:

  "run/beamOn XX"

with XX the number of simulated fission generations.

Execute in the build folder:

  "./fission run.mac 1 XXX"

with XXX the batch size.

### References:
[1] https://geant4.web.cern.ch/
[2] OECD Nuclear Energy Agency, International handbook of evaluated criticality safety benchmark experiments, NEA/NSC/DOC(95)/03 (2016).
[3] https://geant4-userdoc.web.cern.ch/Doxygen/examples_doc/html/index.html
