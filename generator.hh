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
#include "TH1D.h"

#include <utility>

typedef double MeanEnergy_t;
typedef double StdDevEnergy_t;
typedef double RelativeIntensity_t;
typedef std::tuple<MeanEnergy_t, StdDevEnergy_t, RelativeIntensity_t> PeakParam_t;

class generator : public G4VUserPrimaryGeneratorAction {
public:
  generator(std::vector<std::pair<G4double, G4double>> gunEne);
  ~generator() override;

  void GeneratePrimaries(G4Event* anEvent) override;

  TH1D*  EnergySpectrumSample();
  double EnergySpectrumSample_SampleMaximumEnergy();
  double EnergySpectrumSample_SampleRamUpEnergy();
  double EnergySpectrumSample_SampleMinimumEnergy();
  double EnergySpectrumSample_SampleRamDownEnergy();
  int    EnergySpectrumSample_SampleNumberOfEnergyPeaks();
  double EnergySpectrumSample_SamplePedestalEnergyIntensity();
  std::vector<PeakParam_t> EnergySpectrumSample_SamplePeakParameters(int numberOfPeaks);




private:
  G4GeneralParticleSource *fGeneralParticleSource = nullptr;
  detConstruction* fDetCon;
};
