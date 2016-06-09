/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id: be4e879b33241dd90f04560177057fb1aecebf27 $

#include "ChargeDetectorConstruction.hh"
#include "ChargeDetectorMessenger.hh"
#include "ChargeElectrodeSensitivity.hh"
#include "G4CMPSurfaceProperty.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4CMPConfigManager.hh"
#include "G4CMPFieldManager.hh"
#include "G4CMPMeshElectricField.hh"
#include "G4Box.hh"
#include "G4IntersectionSolid.hh"
#include "G4Colour.hh"
#include "G4LatticeManager.hh"
#include "G4LatticePhysical.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4UniformElectricField.hh"

ChargeDetectorConstruction::ChargeDetectorConstruction() :
  sensitivity(nullptr), topSurfProp(nullptr), botSurfProp(nullptr),
  wallSurfProp(nullptr), latManager(G4LatticeManager::GetLatticeManager()),
  fEMField(nullptr), air(nullptr), substrate(nullptr),
  aluminum(nullptr), worldPhys(nullptr), thickness(8.*mm),
  epotScale(0.), voltage(0.), constructed(false), xtalMaterial("Ge"), xtalOrientation("001"), epotFileName(""),
  outputFileName(""), xtalTheta(0.0*deg), xtalPhi(45.*deg){
  /* Default initialization does not leave object in unusable state.
   * Doesn't matter because run initialization will call Construct() and all
   * will be well.
   */
  fMessenger = new ChargeDetectorMessenger(this);
}
  
ChargeDetectorConstruction::~ChargeDetectorConstruction()
{
  delete fEMField;
  delete topSurfProp;
  delete botSurfProp;
  delete wallSurfProp;
  delete fMessenger;
}

G4VPhysicalVolume* ChargeDetectorConstruction::Construct()
{
  if (constructed) {
    if (!G4RunManager::IfGeometryHasBeenDestroyed()) {
      // Run manager hasn't cleaned volume stores. This code shouldn't execute
      G4GeometryManager::GetInstance()->OpenGeometry();
      G4PhysicalVolumeStore::GetInstance()->Clean();
      G4LogicalVolumeStore::GetInstance()->Clean();
      G4SolidStore::GetInstance()->Clean();
    }
    // Only regenerate E field if it has changed since last construction.
    if (epotFileName != G4CMPConfigManager::GetEpotFile() ||
        epotScale != G4CMPConfigManager::GetEpotScale() ||
        voltage != G4CMPConfigManager::GetVoltage()) {
      epotFileName = G4CMPConfigManager::GetEpotFile();
      epotScale = G4CMPConfigManager::GetEpotScale();
      voltage = G4CMPConfigManager::GetVoltage();
      delete fEMField; fEMField = nullptr;
    }
    // Sensitivity doesn't need to ever be deleted, just updated.
    if (outputFileName != G4CMPConfigManager::GetHitOutput()) {
      outputFileName = G4CMPConfigManager::GetHitOutput();
      sensitivity->SetOutputFile(outputFileName);
    }
    // Have to completely remove all lattices to avoid warning on reconstruction
    latManager->Reset();
    // Clear all LogicalSurfaces
    // NOTE: No need to redefine the G4CMPSurfaceProperties
    G4LogicalBorderSurface::CleanSurfaceTable();
  } else { // First setup of geometry
    epotScale = G4CMPConfigManager::GetEpotScale();
    voltage = G4CMPConfigManager::GetVoltage();
    epotFileName = G4CMPConfigManager::GetEpotFile();
    outputFileName = G4CMPConfigManager::GetHitOutput();
  }
  DefineMaterials();
  SetupGeometry();
  constructed = true;
  return worldPhys;
}

void ChargeDetectorConstruction::SetXtalMaterial(G4String mat){
  xtalMaterial=mat;
}

void ChargeDetectorConstruction::SetXtalOrientation(G4String orient){
  if(orient == "001" or orient == "010" or orient == "100"){
    std::cout << "Using 001 Orientation" << std::endl;
    xtalTheta=0.0*deg;
    xtalPhi=45.*deg;
  }
  else if(orient == "110" or orient == "011" or orient == "101"){
    std::cout << "Using 110 Orientation" << std::endl;
    xtalTheta=90.0*deg;
    xtalPhi=45.*deg;
  }
  else if(orient == "111"){
    std::cout << "Using 111 Orientation" << std::endl;
    xtalTheta=54.7356*deg;
    xtalPhi=45.*deg;
  }
  else{
    std::cerr << "Orientation not recognized" << std::endl;
    exit(1);
  }
  
  xtalOrientation=orient;
}

void ChargeDetectorConstruction::DefineMaterials() { 
  G4NistManager* nistManager = G4NistManager::Instance();

  //have messenger determine subtrate
  air = nistManager->FindOrBuildMaterial("G4_AIR");
  substrate = nistManager->FindOrBuildMaterial("G4_"+xtalMaterial);
  aluminum = nistManager->FindOrBuildMaterial("G4_Al");

  // Attach lattice information for set crystal material
  latManager->LoadLattice(substrate, xtalMaterial);
}

void ChargeDetectorConstruction::SetupGeometry() {
  // World
  G4VSolid*          worldSolid = new G4Box("World", 1.*cm, 1.*cm, 1.*cm);
  G4LogicalVolume* worldLogical = new G4LogicalVolume(worldSolid, air, "World");
  worldPhys = new G4PVPlacement(0, G4ThreeVector(), worldLogical, "World", 0, false, 0);

  // Test Crystal
  G4VSolid*             substrateSolid = new G4Box("SubstrateBlock", 5.*mm, 5.*mm, thickness/2);
  G4LogicalVolume*    substrateLogical = new G4LogicalVolume(substrateSolid, substrate, "SubstrateLogical");
  G4VPhysicalVolume* substratePhysical = new G4PVPlacement(0, G4ThreeVector(), substrateLogical, "SubstratePhysical", worldLogical, false, 0);

  // Attach E field to germanium (logical volume, so all placements)
  AttachField(substrateLogical);
  // Physical lattice for each placed detector
  AttachLattice(substratePhysical);

  // Aluminum
  G4VSolid*            aluminumSolid = new G4Box("aluminiumSolid", 5.*mm, 5.*mm, 0.01*cm); //solid alumnium surface
  G4LogicalVolume*   aluminumLogical = new G4LogicalVolume(aluminumSolid, aluminum, "aluminumLogical");
  G4VPhysicalVolume* aluminumTopPhys = new G4PVPlacement(0, G4ThreeVector(0.,0.,thickness/2), aluminumLogical, "topAluminumPhysical", worldLogical, false, 0);
  G4VPhysicalVolume* aluminumBotPhys = new G4PVPlacement(0, G4ThreeVector(0.,0.,-thickness/2), aluminumLogical, "bottomAluminumPhysical", worldLogical, false, 1);

  // Define surface properties. Only should be done once
  if (!constructed) {
    topSurfProp = new G4CMPSurfaceProperty("topSurfProp", 1., 0., 0., 0., 0., 1., 1., 0.);
    botSurfProp = new G4CMPSurfaceProperty("botSurfProp", 1., 0., 0., 0., 0., 1., 1., 0.);
    wallSurfProp = new G4CMPSurfaceProperty("wallSurfProp", 1., 0., 0., 0., 0., 1., 1., 0.);
  }

  // Add surfaces between Ge-Al, and Ge-World
  new G4LogicalBorderSurface("XtalTop", substratePhysical, aluminumTopPhys, topSurfProp);
  new G4LogicalBorderSurface("XtalBot", substratePhysical, aluminumBotPhys, botSurfProp);
  new G4LogicalBorderSurface("xTalWall", substratePhysical, worldPhys,  wallSurfProp);

  // detector -- Note : Aluminum electrode sensitivity is attached to Germanium
  AttachSensitivity(substrateLogical);

  // Visualization attributes
  worldLogical->SetVisAttributes(G4VisAttributes::Invisible);
  G4VisAttributes* simpleBoxVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  substrateLogical->SetVisAttributes(simpleBoxVisAtt);
  aluminumLogical->SetVisAttributes(simpleBoxVisAtt);
}

void ChargeDetectorConstruction::AttachField(G4LogicalVolume* lv)
{
  if (!fEMField) { // Only create field if one doesn't exist.
    if (voltage != 0.0) {
      G4double fieldMag = voltage/thickness;
      fEMField = new G4UniformElectricField(fieldMag*G4ThreeVector(0., 0., 1.));
    } else {
      fEMField = new G4CMPMeshElectricField(epotFileName);
    }
  }

  // Ensure that logical volume has a field manager attached
  if (!lv->GetFieldManager()) { // Should always run
    G4FieldManager* fFieldMgr = new G4CMPFieldManager(fEMField);
    lv->SetFieldManager(fFieldMgr, true);
  }

  lv->GetFieldManager()->SetDetectorField(fEMField);
}

void ChargeDetectorConstruction::AttachLattice(G4VPhysicalVolume* pv)
{
  G4LatticePhysical* detLattice =
    new G4LatticePhysical(latManager->GetLattice(substrate)); 
  G4int h,k,l;// Buffers filled from UI command args
  G4CMPConfigManager::GetMillerOrientation(h,k,l);
  detLattice->SetMillerOrientation(h,k,l,45.0*deg);
  std::cout << "Orientation: " << h << " " << k << " " << l << std::endl;
  latManager->RegisterLattice(pv, detLattice);
}

void ChargeDetectorConstruction::AttachSensitivity(G4LogicalVolume *lv)
{
  if (!sensitivity) { // Only create detector if one doesn't exist.
    // NOTE: ChargeElectrodeSensitivity's ctor will call SetOutputFile()
    sensitivity = new ChargeElectrodeSensitivity("ChargeElectrode");
  }
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(sensitivity);
  lv->SetSensitiveDetector(sensitivity);
}
