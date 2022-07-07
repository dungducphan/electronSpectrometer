#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SingleParticleSource.hh"
#include "G4IonTable.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4String.hh"
#include "G4MTRunManager.hh"
#include "G4RunManager.hh"

#include "detConstruction.hh"

#include "TMath.h"

class generator : public G4VUserPrimaryGeneratorAction {
public:
  generator(std::vector<std::pair<G4double, G4double>> gunEne);
  ~generator() override;

  void GeneratePrimaries(G4Event* anEvent) override;

private:
  G4GeneralParticleSource *fGeneralParticleSource = nullptr;
  detConstruction* fDetCon;
};
