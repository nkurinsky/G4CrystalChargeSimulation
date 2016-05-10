
#include "ChargeDetectorMessenger.hh"
#include "ChargeDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

ChargeDetectorMessenger::ChargeDetectorMessenger(ChargeDetectorConstruction* Det)
  : G4UImessenger(),
    fDetectorConstruction(Det)
{
  fDetDirectory = new G4UIdirectory("/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  fXtalMatCmd = new G4UIcmdWithAString("/det/setXtalMaterial",this);
  fXtalMatCmd->SetGuidance("Select Material of the Crystal.");
  fXtalMatCmd->SetParameterName("choice",false);
  fXtalMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fXtalOrientCmd = new G4UIcmdWithAString("/det/setXtalOrientation",this);
  fXtalOrientCmd->SetGuidance("Select Orientation of the Crystal.");
  fXtalOrientCmd->SetParameterName("choice",false);
  fXtalOrientCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

ChargeDetectorMessenger::~ChargeDetectorMessenger()
{
  delete fXtalMatCmd;
  delete fXtalOrientCmd;
  delete fDetDirectory;
}


void ChargeDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fXtalMatCmd )
   { fDetectorConstruction->SetXtalMaterial(newValue);}

  if( command == fXtalOrientCmd )
   { fDetectorConstruction->SetXtalOrientation(newValue);}
} 
