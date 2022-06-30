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

#include "TMath.h"

class G4VPhysicalVolume;

class G4LogicalVolume;

class G4UniformMagField;

class detConstruction : public G4VUserDetectorConstruction {
public:
    detConstruction();
    virtual ~detConstruction();

    G4VPhysicalVolume *Construct();
    void ConstructSDandField();

private:
    G4LogicalVolume *logicMagField = nullptr;
    G4VPhysicalVolume *physMagField = nullptr;

    G4LogicalVolume *logicWorld = nullptr;
    G4VPhysicalVolume *physWorld = nullptr;
};
