#include "detConstruction.hh"
#include "TString.h"

detConstruction::detConstruction() : G4VUserDetectorConstruction(),
                                     worldSize(5 * m),
                                     magneticField_Y(5 * cm),
                                     magneticField_X(10 * cm),
                                     magneticField_Z(25 * cm),
                                     positionOffset(10 * cm),
                                     drzCube_X(3 * 2.54 * cm),
                                     drzCube_Y(3 * 2.54 * cm),
                                     drzCube_Z(507 * um),
                                     drzPlate_X(9 * 2.54 * cm),
                                     drzPlate_Y(2.5 * 2.54 * cm),
                                     drzPlate_Z(507 * um),
                                     IP_X(35*cm),
                                     IP_Y(15*cm),
                                     IP_Z(500*um)
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

G4VPhysicalVolume* detConstruction::MakePixelizedDetectorVolume(G4int UnitID, G4double* UnitSize, G4Material* material, G4RotationMatrix* rotMat, G4ThreeVector& position, G4bool checkOverLaps = true) {
    G4Box *solidParent = new G4Box(Form("solidParent_%02i", UnitID), UnitSize[0]/2, UnitSize[1]/2, UnitSize[2]/2);
    G4LogicalVolume* logicParent = new G4LogicalVolume(solidParent, material, Form("logicParent_%02i", UnitID));
    G4VPhysicalVolume* physParent = new G4PVPlacement(rotMat, position, logicParent, Form("physParent_%02i", UnitID), logicWorld, false, 0, checkOverLaps);

    G4Box* solidPixel = new G4Box(Form("solidPixel_%02i", UnitID), pixelSize_X * 0.5, pixelSize_Y * 0.5, UnitSize[2] * 0.5);
    G4LogicalVolume* logicPixel = new G4LogicalVolume(solidPixel, material, Form("logicPixel_%02i", UnitID));

    unsigned int numberOfCells_X = UnitSize[0] / pixelSize_X;
    unsigned int numberOfCells_Y = UnitSize[1] / pixelSize_Y;
    for (unsigned int i = 0; i < numberOfCells_X; ++i) {
        for (unsigned  int j = 0; j < numberOfCells_Y; ++j) {
            G4ThreeVector pixelPosition((- UnitSize[0] + pixelSize_X) * 0.5 + i * pixelSize_X,
                                        (- UnitSize[1] + pixelSize_Y) * 0.5 + j * pixelSize_Y,
                                        0.);
            new G4PVPlacement(0, pixelPosition, logicPixel, Form("physPixel_%02i_%04i_%04i", UnitID, i, j), logicParent, false, 1E7 * UnitID + j + numberOfCells_X * i, checkOverLaps);
        }
    }

    return physParent;
}

G4VPhysicalVolume *detConstruction::Construct() {
    G4NistManager *nist = G4NistManager::Instance();
    G4bool checkOverlaps = true;

    worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    aluminum = nist->FindOrBuildMaterial("G4_Al");
    phosphor = new G4Material("Phosphor", 15, 30.974 * g/mole, 4.677 * g / cm3);
    pet      = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
    drz      = new G4Material("DRZ", 163.52 * mg/cm2, 2);
    drz->AddMaterial(phosphor, 88.67 * perCent);
    drz->AddMaterial(pet, 11.33 * perCent);

    pixelSize_X = 1000 * um;
    pixelSize_Y = 1000 * um;

    // World
    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0,
                                                     checkOverlaps);

    // Magnetic Field
    G4Box *solidMagField = new G4Box("solidMagField", magneticField_X / 2, magneticField_Y / 2, magneticField_Z / 2);
    logicMagField = new G4LogicalVolume(solidMagField, worldMat, "logicMagField");
    physMagField = new G4PVPlacement(0, G4ThreeVector(0, 0, worldSize / 2 - magneticField_Position), logicMagField, "physMagField", logicWorld, false, 0, checkOverlaps);


    G4double IP_Size[3] = {10 * cm, 10 * cm, IP_Z};
    G4ThreeVector ip_pos(0., 0., 0.);
    physIP = MakePixelizedDetectorVolume(2, &IP_Size[0], drz, 0, ip_pos);

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

    spectrometerSD *sd = new spectrometerSD("sd");
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);
    SetSensitiveDetector("logicPixel_02", sd);
}

