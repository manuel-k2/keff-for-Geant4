#----------------------------------------------------------------------------
# Setup project-specific variables
# override built-in version with new version of libFission.*
set(USE_FISSION_NEW false CACHE BOOL "Override built-in version with new version of libFission.*")
# use modular hadronic physics list(s)
set(USE_MOD_PHYSLIST false CACHE BOOL "Use modular hadronic physics list(s)")

# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
# Check if we have a fortran compiler
find_program(FORT NAMES $ENV{FC}
                        gfortran
                        ifort)
if(NOT FORT)
  project(fission C CXX)
  message(STATUS
          "No FORTRAN compiler")
else()
  project(fission C CXX Fortran)
  message(STATUS
          "Found FORTRAN compiler")
endif()

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# Allow for multithreading
#option(GEANT4_BUILD_MULTITHREADED "Enable multithreading in Geant4" ON)
#option(G4MULTITHREADED "Enable multithreading in Geant4" OFF)
#set(G4MULTITHREADED false CACHE BOOL "Enable multithreading in Geant4" FORCE)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(fission fission.cc ${sources} ${headers})
target_link_libraries(fission ${Geant4_LIBRARIES} )

#----------------------------------------------------------------------------
# Copy all macros to the build directory
#
set(fission_SCRIPTS
    run.mac
  )

foreach(_script ${fission_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS fission DESTINATION bin)

# Check if we override built-in version with new version (libFission.*) of the library
if(USE_FISSION_NEW)
  find_library(LFISSION Fission
               PATHS ${CMAKE_SOURCE_DIR}/../lib
               DOC "fission library"
               NO_DEFAULT_PATH
              )
  if(NOT LFISSION)
    message(FATAL_ERROR
            "fission library not found")
  else()
    target_link_libraries(fission 
                          ${LFISSION} 
                         )
    configure_file(${LFISSION} .
                  COPYONLY
                  )
    message(STATUS ${LFISSION})
  endif()
  message(STATUS
          "Overriding fission library libFission.* with version in ../../lib")
  include_directories(${CMAKE_SOURCE_DIR}/../include)
  add_definitions(-DFISSION_NEW)
endif()

if(USE_MOD_PHYSLIST)
  add_definitions(-DMOD_PHYSLIST)
endif()

add_definitions(-DG4MULTITHREADED)
