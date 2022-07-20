#include "generator.hh"

generator::generator(std::vector<std::pair<G4double, G4double>> gunEne) {
    fGeneralParticleSource = new G4GeneralParticleSource();

    fDetCon = (detConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();

    G4SingleParticleSource *sps = fGeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");

    sps->SetParticleDefinition(particle);
    sps->SetNumberOfParticles(10000);

    sps->GetPosDist()->SetPosDisType("Volume"); // Point, Beam, Plane, Surface, Volume
    sps->GetPosDist()->SetPosDisShape("Cylinder");
    sps->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., fDetCon->worldSize_Z/2 - fDetCon->positionOffset));
    sps->GetPosDist()->SetHalfZ(250 * um);
    sps->GetPosDist()->SetRadius(50 * um);

    sps->GetAngDist()->SetAngDistType("iso"); // Isotropic, Cosine-law, Beam, User-defined

    sps->GetAngDist()->SetMinTheta(0. * rad);
    sps->GetAngDist()->SetMaxTheta(TMath::PiOver4() / 400. * rad);
    sps->GetAngDist()->SetMinPhi(0 * rad);
    sps->GetAngDist()->SetMaxPhi(TMath::Pi() * 2. * rad);

//    sps->GetEneDist()->SetEnergyDisType("User");
//    for (int i = 0; i < gunEne.size(); i++) {
//        G4double ehi = std::get<0>(gunEne.at(i)) * MeV;
//        G4double val = std::get<1>(gunEne.at(i));
//        sps->GetEneDist()->UserEnergyHisto(G4ThreeVector(ehi, val, 0));
//    }

    sps->GetEneDist()->SetEnergyDisType("User");
    TH1D* hEnergySpectrum = EnergySpectrumSample();
    for (int i = 1; i < hEnergySpectrum->GetSize() - 1; i++) {
        G4double ehi = hEnergySpectrum->GetBinLowEdge(i + 1);
        G4double val = hEnergySpectrum->GetBinContent(i);
        sps->GetEneDist()->UserEnergyHisto(G4ThreeVector(ehi, val, 0));
    }

}

generator::~generator() {
    delete fGeneralParticleSource;
}

void generator::GeneratePrimaries(G4Event *anEvent) {
    fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}

TH1D *generator::EnergySpectrumSample() {
    double maximumEnergy = EnergySpectrumSample_SampleMaximumEnergy();
    double ramUpEnergy = EnergySpectrumSample_SampleRamUpEnergy();
    double minimumEnergy = EnergySpectrumSample_SampleMinimumEnergy();
    double ramDownEnergy = EnergySpectrumSample_SampleRamDownEnergy();
    int    numberOfPeaks = EnergySpectrumSample_SampleNumberOfEnergyPeaks();
    std::vector<PeakParam_t> peakParams = EnergySpectrumSample_SamplePeakParameters(numberOfPeaks);
    double pedesdalIntensity = EnergySpectrumSample_SamplePedestalEnergyIntensity();

    TH1D* hEnergySpectrum = new TH1D("hEnergySpectrum", "", 1000, minimumEnergy, maximumEnergy);
    // FIXME: implement this function and other functions above

    return nullptr;
}
