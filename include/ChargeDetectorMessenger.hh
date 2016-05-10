/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef G4CMPConfigMessenger_hh
#define G4CMPConfigMessenger_hh 1

// $Id: 847f13c417b0f5754afc3994ddab84c0758f9e6b $
// File:  G4CMPConfigMessenger.hh
//
// Description:	Macro command defitions to set user configuration in
//		G4CMPConfigManager.
//
// 20140904  Michael Kelsey
// 20141029  Add command to set output e/h positions file
// 20141231  Add command to set scale (relative to l0) for minimum steps
// 20150106  Add command to toggle generating Luke phonons
// 20150122  Add command to rescale Epot file voltage by some factor
// 20150603  Add command to limit reflections in DriftBoundaryProcess

#include "G4UImessenger.hh"

class G4CMPConfigManager;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcommand;


class G4CMPConfigMessenger : public G4UImessenger {
public:
  G4CMPConfigMessenger(G4CMPConfigManager* theData);
  virtual ~G4CMPConfigMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String value);

protected:
  // Create or access directory path common to all commands
  void CreateDirectory(const char* path, const char* desc);

  // Create G4UIcommand (arbitrary subclass) within current command path
  template <class T>
  T* CreateCommand(const G4String& commandName, const G4String& description);

private:
  G4CMPConfigManager* theManager;

  G4bool localCmdDir;		// Flag if directory was created or found
  G4UIdirectory* cmdDir;

  G4UIcmdWithAnInteger* verboseCmd;
  G4UIcmdWithAnInteger* bounceCmd;
  G4UIcmdWithADoubleAndUnit* voltageCmd;
  G4UIcmdWithADouble* minstepCmd;
  G4UIcmdWithADouble* makePhononCmd;
  G4UIcmdWithADouble* escaleCmd;
  G4UIcmdWithAString* fileCmd;
  G4UIcmdWithAString* dirCmd;
  G4UIcmdWithAString* hitsCmd;
};

#include "G4CMPConfigMessenger.icc"

#endif /* G4CMPConfigMessenger_hh */
