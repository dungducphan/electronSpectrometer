#include "generator.hh"

generator::generator(std::vector<std::pair<G4double, G4double>> gunEne) {
    fGeneralParticleSource = new G4GeneralParticleSource();

    fDetCon = (detConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();

    G4SingleParticleSource *sps = fGeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");

    sps->SetParticleDefinition(particle);
    sps->SetNumberOfParticles(100);

    sps->GetPosDist()->SetPosDisType("Volume"); // Point, Beam, Plane, Surface, Volume
    sps->GetPosDist()->SetPosDisShape("Cylinder");
    sps->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., fDetCon->worldSize/2 - fDetCon->positionOffset));
    sps->GetPosDist()->SetHalfZ(0.5 * mm);
    sps->GetPosDist()->SetRadius(10 * um);

    sps->GetAngDist()->SetAngDistType("iso"); // Isotropic, Cosine-law, Beam, User-defined

    sps->GetAngDist()->SetMinTheta(0. * rad);
    sps->GetAngDist()->SetMaxTheta(TMath::PiOver4() / 400. * rad);
    sps->GetAngDist()->SetMinPhi(0 * rad);
    sps->GetAngDist()->SetMaxPhi(TMath::Pi() * 2. * rad);

    // Somewhat realistic energy distribution
    sps->GetEneDist()->SetEnergyDisType("User");
    for (int i = 0; i < gunEne.size(); i++) {
        G4double ehi = std::get<0>(gunEne.at(i)) * MeV;
        G4double val = std::get<1>(gunEne.at(i));
        sps->GetEneDist()->UserEnergyHisto(G4ThreeVector(ehi, val, 0));
    }
}

generator::~generator() {
    delete fGeneralParticleSource;
}

void generator::GeneratePrimaries(G4Event *anEvent) {
    fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}
