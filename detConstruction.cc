#include "detConstruction.hh"

detConstruction::detConstruction() : G4VUserDetectorConstruction(),
                                     worldSize(5 * m),
                                     magneticField_Y(5 * cm),
                                     magneticField_X(10 * cm),
                                     magneticField_Z(25 * cm),
                                     positionOffset(10 * cm),
                                     drzCube_X(3 * 2.54 * cm),
                                     drzCube_Y(3 * 2.54 * cm),
                                     drzCube_Z(1 * mm),
                                     drzPlate_X(9 * 2.54 * cm),
                                     drzPlate_Y(2.5 * 2.54 * cm),
                                     drzPlate_Z(1 * mm),
                                     IP_X(35*cm),
                                     IP_Y(15*cm),
                                     IP_Z(1*mm)
{
    magneticField_Position = positionOffset + 83.50 * cm;
    drzCube_Position = positionOffset + 63.31 * cm;
    drzPlate_1_Position_Z = positionOffset + 173.92 * cm;
    drzPlate_2_Position_Z = positionOffset + 275.22 * cm;
    drzPlate_Position_X = (8.08 - 2.69) * cm;
    IP_Position_Z = positionOffset + 254.6 * cm;
    IP_Position_X = 10 * cm;

}

detConstruction::~detConstruction() {}

G4VPhysicalVolume *detConstruction::Construct() {
    G4NistManager *nist = G4NistManager::Instance();
    G4bool checkOverlaps = true;

    G4Material *worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    G4Material *aluminum = nist->FindOrBuildMaterial("G4_Al");

    // World
    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0,
                                                     checkOverlaps);

    // Magnetic Field
    G4Box *solidMagField = new G4Box("solidMagField", magneticField_X / 2, magneticField_Y / 2, magneticField_Z / 2);
    logicMagField = new G4LogicalVolume(solidMagField, worldMat, "logicMagField");
    physMagField = new G4PVPlacement(0, G4ThreeVector(0, 0, worldSize / 2 - magneticField_Position), logicMagField, "physMagField", logicWorld, false, 0, checkOverlaps);

    // DRZ Cube
    G4Box *solidDRZCube = new G4Box("solidDRZCube", drzCube_X/2, drzCube_Y/2, drzCube_Z/2);
    // FIXME: change material to make multiple Coulomb scattering
    logicDRZCube = new G4LogicalVolume(solidDRZCube, worldMat, "logicDRZCube");
    G4RotationMatrix* rotMat_DRZCube = new G4RotationMatrix(G4ThreeVector(0, 1, 0), -TMath::PiOver4());
    physDRZCube = new G4PVPlacement(rotMat_DRZCube, G4ThreeVector(0, 0, worldSize/2 - drzCube_Position), logicDRZCube, "physDRZCube", logicWorld, false, 0, checkOverlaps);

    // DRZ Plate 1
    G4Box *solidDRZPlate = new G4Box("solidDRZPlate", drzPlate_X/2, drzPlate_Y/2, drzPlate_Z/2);
    // FIXME: change material to make multiple Coulomb scattering
    logicDRZPlate = new G4LogicalVolume(solidDRZPlate, worldMat, "logicDRZPlate");
    G4RotationMatrix* rotMat_DRZPlate = new G4RotationMatrix(G4ThreeVector(0, 1, 0), TMath::PiOver4());
    physDRZPlate_1 = new G4PVPlacement(rotMat_DRZPlate, G4ThreeVector(drzPlate_Position_X, 0, worldSize/2 - drzPlate_1_Position_Z), logicDRZPlate, "physDRZPlate_1", logicWorld, false, 0, checkOverlaps);
    physDRZPlate_2 = new G4PVPlacement(rotMat_DRZPlate, G4ThreeVector(drzPlate_Position_X, 0, worldSize/2 - drzPlate_2_Position_Z), logicDRZPlate, "physDRZPlate_2", logicWorld, false, 0, checkOverlaps);

    // IP
    G4Box *solidIP = new G4Box("solidIP", IP_X/2, IP_Y/2, IP_Z/2);
    // FIXME: change material to make multiple Coulomb scattering
    logicIP = new G4LogicalVolume(solidIP, worldMat, "logicIP");
    physIP = new G4PVPlacement(0, G4ThreeVector(IP_Position_X, 0, worldSize/2 - IP_Position_Z), logicIP, "physIP", logicWorld, false, 0, checkOverlaps);

    // Aluminum Laser Block
    G4Box *solidAlLB = new G4Box("solidAlLB", 5 * cm / 2, 5 * cm / 2, 100 * um / 2);
    G4LogicalVolume* logicAlLB = new G4LogicalVolume(solidAlLB, aluminum, "logicAlLB");
    G4VPhysicalVolume* physAlLB = new G4PVPlacement(0, G4ThreeVector(0, 0, worldSize/2 - 25 * cm), logicAlLB, "physAlLB", logicWorld, false, 0, checkOverlaps);

    return physWorld;
}

void detConstruction::ConstructSDandField() {
    G4MagneticField *magField;
    magField = new G4UniformMagField(G4ThreeVector(0., -0.86 * tesla, 0.));

    G4FieldManager *localFieldMgr = new G4FieldManager(magField);
    logicMagField->SetFieldManager(localFieldMgr, true);
    localFieldMgr->SetDetectorField(magField);
    localFieldMgr->CreateChordFinder(magField);

}

