
#ifndef ChargeDetectorMessenger_hh
#define ChargeDetectorMessenger_hh 1

#include "globals.hh"
#include "G4UImessenger.hh"

class ChargeDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

/// Messenger class that defines commands for B2bDetectorConstruction.
///
/// It implements commands:
/// - /det/setXtalMaterial name
/// - /det/setXtalOrientation name

class ChargeDetectorMessenger: public G4UImessenger
{
public:
  ChargeDetectorMessenger(ChargeDetectorConstruction* );
  virtual ~ChargeDetectorMessenger();
   
  virtual void SetNewValue(G4UIcommand*, G4String);
   
private:
  ChargeDetectorConstruction*  fDetectorConstruction; 
  G4UIdirectory*           fDetDirectory;
  G4UIcmdWithAString*      fXtalMatCmd;
  G4UIcmdWithAString*      fXtalOrientCmd;
  G4UIcmdWithAString*      fXtalThickCmd;
  G4UIcmdWithAString*      fXtalRateCmd;
};

#endif /* G4CMPConfigMessenger_hh */
