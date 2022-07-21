#include "generator.hh"

generator::generator() {
    fRndNumberGen = new TRandom3(time(nullptr));

    fGeneralParticleSource = new G4GeneralParticleSource();

    fDetCon = (detConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();

    G4SingleParticleSource *sps = fGeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");

    sps->SetParticleDefinition(particle);
    sps->SetNumberOfParticles(100000);

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

    sps->GetEneDist()->SetEnergyDisType("User");
    TH1D* hEnergySpectrum = EnergySpectrumSample();
    for (int i = 1; i < hEnergySpectrum->GetSize() - 1; i++) {
        G4double ehi = hEnergySpectrum->GetBinLowEdge(i + 1) * GeV;
        G4double val = hEnergySpectrum->GetBinContent(i);
        sps->GetEneDist()->UserEnergyHisto(G4ThreeVector(ehi, val, 0));
    }

}

generator::~generator() {
    delete fGeneralParticleSource;
    delete fRndNumberGen;
}

void generator::GeneratePrimaries(G4Event *anEvent) {
    fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}

double generator::EnergySpectrumSample_SampleMaximumEnergy() {
    return fRndNumberGen->Gaus(4.2, 1.8);
}

double generator::EnergySpectrumSample_SampleMinimumEnergy() {
    return fRndNumberGen->Gaus(0.8, 1.6);
}

double generator::EnergySpectrumSample_SampleRamUpEnergy() {
    return fEnergySpectrumSample_MaximumEnergy - fRndNumberGen->Uniform(0.3, 0.7);
}

double generator::EnergySpectrumSample_SampleRamDownEnergy() {
    return fEnergySpectrumSample_MinimumEnergy + fRndNumberGen->Gaus(0.7, 1.2);
}

int generator::EnergySpectrumSample_SampleNumberOfEnergyPeaks() {
    int x = 0;
    do {
        x = fRndNumberGen->Poisson(2.5);
    } while (x < 1 || x > 4);

    return x;
}

double generator::EnergySpectrumSample_SamplePedestalEnergyIntensity() {
    return fRndNumberGen->Uniform(0.5, 0.7);
}

std::vector<PeakParam_t> generator::EnergySpectrumSample_SamplePeakParameters(int numberOfPeaks) {
    std::vector<PeakParam_t> ret;

    for (unsigned int i = 0; i < fEnergySpectrumSample_NumberOfPeaks; i++) {
        double mean = fRndNumberGen->Uniform(fEnergySpectrumSample_MinimumEnergy,
                                             fEnergySpectrumSample_MaximumEnergy);

        double sigma = fRndNumberGen->Uniform(0.05, 0.6);
        double intensity;
        do {
            intensity = fRndNumberGen->PoissonD(2.8);
        } while (intensity <= 1 || intensity >= 7);
        intensity *= fEnergySpectrumSample_PedestalIntensity;

        PeakParam_t tmp(intensity, mean, sigma);
        ret.push_back(tmp);
    }

    return ret;
}

TH1D *generator::EnergySpectrumSample() {
    fEnergySpectrumSample_MaximumEnergy = EnergySpectrumSample_SampleMaximumEnergy();
    fEnergySpectrumSample_MinimumEnergy = EnergySpectrumSample_SampleMinimumEnergy();
    fEnergySpectrumSample_RamUpEnergy = EnergySpectrumSample_SampleRamUpEnergy();
    fEnergySpectrumSample_RamDownEnergy = EnergySpectrumSample_SampleRamDownEnergy();
    fEnergySpectrumSample_NumberOfPeaks = EnergySpectrumSample_SampleNumberOfEnergyPeaks();
    fEnergySpectrumSample_PedestalIntensity = EnergySpectrumSample_SamplePedestalEnergyIntensity();
    fEnergySpectrumSample_PeakParams = EnergySpectrumSample_SamplePeakParameters(fEnergySpectrumSample_NumberOfPeaks);

    TH1D* hEnergySpectrum = new TH1D("hEnergySpectrum", "", 1000,
                                     fEnergySpectrumSample_MinimumEnergy, fEnergySpectrumSample_MaximumEnergy);
    std::vector<TF1*> peakFuncs;
    for (unsigned int i = 0; i < fEnergySpectrumSample_NumberOfPeaks; i++) {
        TF1* tmp_peakFunc = new TF1(Form("peak_%02i", i), "gaus(0)",
                                    fEnergySpectrumSample_MinimumEnergy, fEnergySpectrumSample_MaximumEnergy);
        tmp_peakFunc->SetParameter(0, std::get<0>(fEnergySpectrumSample_PeakParams.at(i)));
        tmp_peakFunc->SetParameter(1, std::get<1>(fEnergySpectrumSample_PeakParams.at(i)));
        tmp_peakFunc->SetParameter(2, std::get<2>(fEnergySpectrumSample_PeakParams.at(i)));
        peakFuncs.push_back(tmp_peakFunc);
    }

    std::cout << "OKO" << std::endl;

    for (unsigned int i = 1; i < hEnergySpectrum->GetSize() - 1; i++) {
        double x = hEnergySpectrum->GetBinCenter(i);
        double val = 0.;
        for (unsigned int j = 0; j < fEnergySpectrumSample_NumberOfPeaks; j++) {
            val += peakFuncs.at(j)->Eval(x);
        }
        if (fEnergySpectrumSample_RamDownEnergy <= x && x <= fEnergySpectrumSample_RamUpEnergy) {
            val += fEnergySpectrumSample_PedestalIntensity;
        } else if (x < fEnergySpectrumSample_RamDownEnergy) {
            val += fEnergySpectrumSample_PedestalIntensity * (x -
                    fEnergySpectrumSample_MinimumEnergy) / (fEnergySpectrumSample_RamDownEnergy -
                    fEnergySpectrumSample_MinimumEnergy);
        } else {
            val += fEnergySpectrumSample_PedestalIntensity * (fEnergySpectrumSample_MaximumEnergy - x) /
                    (fEnergySpectrumSample_MaximumEnergy -
                    fEnergySpectrumSample_RamUpEnergy);
        }

        val += fRndNumberGen->Uniform(-0.5, 0.5) * TMath::Sqrt(val);
        hEnergySpectrum->SetBinContent(i, val);
    }

    return hEnergySpectrum;
}
