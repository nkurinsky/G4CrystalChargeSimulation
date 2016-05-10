/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id: 165a1df2a9364e285f19b48f5c016259faee053b $

#ifndef ChargeDetectorConstruction_h
#define ChargeDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class ChargeElectrodeSensitivity;
class G4CMPSurfaceProperty;
class G4LatticeManager;
class G4Material;
class G4VPhysicalVolume;
class G4VUserDetectorConstruction;
class G4ElectricField;

class ChargeDetectorMessenger;

class ChargeDetectorConstruction : public G4VUserDetectorConstruction {
public:
  ChargeDetectorConstruction();
  virtual ~ChargeDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  void SetXtalMaterial(G4String mat);
  void SetXtalOrientation(G4String orient);
     
private:
  void DefineMaterials();
  void SetupGeometry();
  void AttachField(G4LogicalVolume* lv);
  void AttachLattice(G4VPhysicalVolume* pv);
  void AttachSensitivity(G4LogicalVolume* lv);

private:
  ChargeElectrodeSensitivity* sensitivity;
  G4CMPSurfaceProperty* topSurfProp;
  G4CMPSurfaceProperty* botSurfProp;
  G4CMPSurfaceProperty* wallSurfProp;
  G4LatticeManager* latManager;
  G4ElectricField* fEMField;
  G4Material* air;
  G4Material* substrate;
  G4Material* aluminum;
  G4VPhysicalVolume* worldPhys;
  G4double thickness; // Useful for geom. and field
  G4double epotScale;
  G4double voltage;
  G4bool constructed;
  G4String xtalMaterial;
  G4String xtalOrientation;
  G4String epotFileName;
  G4String outputFileName;

  ChargeDetectorMessenger * fMessenger;
};

#endif

