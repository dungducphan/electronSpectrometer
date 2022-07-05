#include "generator.hh"

generator::generator() {
    fGeneralParticleSource = new G4GeneralParticleSource();

    fDetCon = (detConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();

    G4SingleParticleSource *sps = fGeneralParticleSource->GetCurrentSource();
    G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");

    sps->SetParticleDefinition(particle);
    sps->SetNumberOfParticles(10000);

    sps->GetPosDist()->SetPosDisType("Point"); // Point, Beam, Plane, Surface, Volume
    sps->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., fDetCon->worldSize/2 - fDetCon->positionOffset));
    sps->GetPosDist()->ConfineSourceToVolume("NULL");

    sps->GetAngDist()->SetAngDistType("iso"); // Isotropic, Cosine-law, Beam, User-defined

    sps->GetAngDist()->SetMinTheta(0. * rad);
    sps->GetAngDist()->SetMaxTheta(TMath::PiOver4() / 400. * rad);
    sps->GetAngDist()->SetMinPhi(0 * rad);
    sps->GetAngDist()->SetMaxPhi(TMath::Pi() * 2. * rad);

    // FIXME: before giving Ou an image, try changing this energy distribution to something more skewed toward lower energies.
    sps->GetEneDist()->SetEnergyDisType("Gauss"); // Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse gamma), User, Arb, Epn (energy per nucleon)
    sps->GetEneDist()->SetMonoEnergy(2 * GeV);
    sps->GetEneDist()->SetBeamSigmaInE(0.05 * GeV);
}

generator::~generator() {
    delete fGeneralParticleSource;
}

void generator::GeneratePrimaries(G4Event *anEvent) {
    fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}
