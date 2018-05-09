/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

/// \file charge/g4cmpCharge.cc
/// \brief Main program of the G4CMP/charge example
//
// $Id: ae28f7dde8e28eeed72d6b9ab09a3298ff57d367 $
//
// 20140509  Add conditional code for Geant4 10.0 vs. earlier
// 20150112  Remove RM->Initialize() to support macro configuration
// 20160111  Remove check for Geant4 > 10.0 since we hard depend on 10.2+
// 20180509  Updated to work with the latest version of G4CMP

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "ChargeActionInitialization.hh"
#include "ChargeDetectorConstruction.hh"
#include "ChargeConfigManager.hh"
#include "ChargeConfigMessenger.hh"
#include "G4CMPConfigManager.hh"
#include "G4CMPPhysicsList.hh"

int main(int argc,char** argv)
{
 // Construct the run manager
 G4RunManager* runManager = new G4RunManager;

 // Set mandatory initialization classes
 ChargeDetectorConstruction* detector = new ChargeDetectorConstruction;
 runManager->SetUserInitialization(detector);

 G4VUserPhysicsList* physics = new G4CMPPhysicsList();
 physics->SetCuts();
 runManager->SetUserInitialization(physics);
    
 // Set user action classes (different for Geant4 10.0)
 runManager->SetUserInitialization(new ChargeActionInitialization);

 // Create G4CMP configuration manager to ensure macro commands exist
 // G4CMPConfigManager::GetVerboseLevel();
 G4CMPConfigManager::Instance();
 ChargeConfigManager::Instance();

 
#ifdef G4VIS_USE
 // Visualization manager
 G4VisManager* visManager = new G4VisExecutive("quiet");
 visManager->Initialize();
#endif
    
 // Get the pointer to the User Interface manager
 G4UImanager* UImanager = G4UImanager::GetUIpointer();  

 if (argc==1)   // Define UI session for interactive mode
   {
#ifdef G4UI_USE
     G4UIExecutive * ui = new G4UIExecutive(argc,argv);
     ui->SessionStart();
     delete ui;
#endif
   }
 else           // Batch mode
   {
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
     UImanager->ApplyCommand(command+fileName);
   }

#ifdef G4VIS_USE
 delete visManager;
#endif
 delete runManager;
 
 return 0;
}


