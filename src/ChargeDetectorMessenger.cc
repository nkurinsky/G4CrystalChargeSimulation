
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

  fXtalThickCmd = new G4UIcmdWithAString("/det/setXtalThickness",this);
  fXtalThickCmd->SetGuidance("Select Thickness (mm) of the crystal");
  fXtalThickCmd->SetParameterName("choice",false);
  fXtalThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fXtalRateCmd = new G4UIcmdWithAString("/det/setXtalIVRate",this);
  fXtalRateCmd->SetGuidance("Select Intervalley Scattering Rate (Hz) of the crystal");
  fXtalRateCmd->SetParameterName("choice",false);
  fXtalRateCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fXtalEMfpCmd = new G4UIcmdWithAString("/det/setXtaleL0",this);
  fXtalEMfpCmd->SetGuidance("Select Intravalley Scattering Length of the crystal");
  fXtalEMfpCmd->SetParameterName("choice",false);
  fXtalEMfpCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fXtalHMfpCmd = new G4UIcmdWithAString("/det/setXtalhL0",this);
  fXtalHMfpCmd->SetGuidance("Select Intraband Scattering Length of the crystal");
  fXtalHMfpCmd->SetParameterName("choice",false);
  fXtalHMfpCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

ChargeDetectorMessenger::~ChargeDetectorMessenger()
{
  delete fXtalMatCmd;
  delete fXtalOrientCmd;
  delete fXtalThickCmd;
  delete fXtalRateCmd;
  delete fXtalEMfpCmd;
  delete fXtalHMfpCmd;
  delete fDetDirectory;
}


void ChargeDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fXtalMatCmd )
   { fDetectorConstruction->SetXtalMaterial(newValue);}

  if( command == fXtalOrientCmd )
   { fDetectorConstruction->SetXtalOrientation(newValue);}

  if( command == fXtalThickCmd ){ 
    G4double thick = atof(newValue);
    fDetectorConstruction->SetXtalThickness(thick);
  }

  if( command == fXtalRateCmd ){
    G4double rate = atof(newValue);
    fDetectorConstruction->SetXtalIVRate(rate);
  }

  if( command == fXtalEMfpCmd ){
    G4double mfp = atof(newValue);
    fDetectorConstruction->SetXtaleL0(mfp);
  }

  if( command == fXtalHMfpCmd ){
    G4double mfp = atof(newValue);
    fDetectorConstruction->SetXtalhL0(mfp);
  }
} 
