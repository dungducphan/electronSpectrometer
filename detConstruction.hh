#pragma once

#include <utility>
#include <tuple>
#include <fstream>

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SDManager.hh"
#include "G4FieldManager.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4AutoDelete.hh"

#include "spectrometerSD.hh"

#include "TMath.h"
#include "TString.h"

class G4VPhysicalVolume;

class G4LogicalVolume;

class G4UniformMagField;

class detConstruction : public G4VUserDetectorConstruction {
public:
    detConstruction();
    virtual ~detConstruction();

    G4VPhysicalVolume *Construct();
    void ConstructSDandField();

    G4double worldSize;

    // Magnet
    G4double magneticField_Y;
    G4double magneticField_X;
    G4double magneticField_Z;
    G4double magneticField_Position;
    G4double positionOffset;

    // DRZ-Cube
    G4double drzCube_X;
    G4double drzCube_Y;
    G4double drzCube_Z;
    G4double drzCube_Position;

    // DRZ-Plate
    G4double drzPlate_X;
    G4double drzPlate_Y;
    G4double drzPlate_Z;
    G4double drzPlate_1_Position_Z;
    G4double drzPlate_2_Position_Z;
    G4double drzPlate_Position_X;

    // Image Plate (IP)
    G4double IP_X;
    G4double IP_Y;
    G4double IP_Z;
    G4double IP_Position_Z;
    G4double IP_Position_X;

    G4LogicalVolume *logicMagField = nullptr;
    G4VPhysicalVolume *physMagField = nullptr;

    G4LogicalVolume *logicDRZCube = nullptr;
    G4VPhysicalVolume *physDRZCube = nullptr;

    G4LogicalVolume *logicIP = nullptr;
    G4VPhysicalVolume *physIP = nullptr;

    G4LogicalVolume *logicDRZPlate= nullptr;
    G4VPhysicalVolume *physDRZPlate_1 = nullptr;
    G4VPhysicalVolume *physDRZPlate_2 = nullptr;

    G4LogicalVolume *logicWorld = nullptr;
    G4VPhysicalVolume *physWorld = nullptr;

    G4VPhysicalVolume* MakePixelizedDetectorVolume(G4int UnitID, G4double* UnitSize, G4Material* mat, G4RotationMatrix* rotMat, G4ThreeVector& position, G4bool chekcOverLaps);
    G4double pixelSize_X;
    G4double pixelSize_Y;

    G4Material *worldMat;
    G4Material *aluminum;
    G4Material *phosphor;
    G4Material *pet;
    G4Material *drz;
};
