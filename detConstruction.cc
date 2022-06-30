#include "detConstruction.hh"

detConstruction::detConstruction() : G4VUserDetectorConstruction() {}

detConstruction::~detConstruction() {}

G4VPhysicalVolume *detConstruction::Construct() {
    G4NistManager *nist = G4NistManager::Instance();
    G4bool checkOverlaps = true;

    // World
    G4double worldSize = 5 * m;
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0,
                                                     checkOverlaps);

    // Magnetic Field
    G4double magneticFieldRadius = 10 * cm;
    G4double magneticFieldHeight = 5 * cm;
    G4Tubs *solidMagField = new G4Tubs("solidMagField", 0, magneticFieldRadius, magneticFieldHeight, 0, TMath::TwoPi());
    logicMagField = new G4LogicalVolume(solidMagField, worldMat, "logicMagField");
    G4RotationMatrix* rotMat = new G4RotationMatrix(0, TMath::Pi()/2, 0);
    physMagField = new G4PVPlacement(rotMat, G4ThreeVector(), logicMagField, "physMagField", logicWorld, false, 0, checkOverlaps);

    return physWorld;
}

void detConstruction::ConstructSDandField() {
    G4MagneticField *magField;
    magField = new G4UniformMagField(G4ThreeVector(0., 10.0 * tesla, 0.));

    G4FieldManager *localFieldMgr = new G4FieldManager(magField);
    logicMagField->SetFieldManager(localFieldMgr, true);
    localFieldMgr->SetDetectorField(magField);
    localFieldMgr->CreateChordFinder(magField);

}

