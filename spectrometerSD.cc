#include "spectrometerSD.hh"

spectrometerSD::spectrometerSD(const G4String &name) : G4VSensitiveDetector(name) {}

spectrometerSD::~spectrometerSD() {}

G4bool spectrometerSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {
    G4Track *track = aStep->GetTrack();

    if (track->GetDefinition() != G4ParticleTable::GetParticleTable()->FindParticle(11) &&
        track->GetDefinition() != G4ParticleTable::GetParticleTable()->FindParticle(22)) {
        return false;
    }

    G4int PID;
    if (track->GetDefinition() == G4ParticleTable::GetParticleTable()->FindParticle(11)) {
        PID = 1;
    } else if (track->GetDefinition() == G4ParticleTable::GetParticleTable()->FindParticle(22)) {
        PID = 2;
    } else {
        PID = 0;
    }

    G4StepPoint *prePoint = aStep->GetPreStepPoint();
    G4int SensitiveDetectorID;
    if (prePoint->GetPhysicalVolume()->GetName() == "physDRZCube") {
        SensitiveDetectorID = 0;
    } else if (prePoint->GetPhysicalVolume()->GetName() == "physDRZPlate_1") {
        SensitiveDetectorID = 1;
    } else if (prePoint->GetPhysicalVolume()->GetName() == "physIP") {
        SensitiveDetectorID = 2;
    } else if (prePoint->GetPhysicalVolume()->GetName() == "physDRZPlate_2") {
        SensitiveDetectorID = 3;
    } else {
        SensitiveDetectorID = -1;
    }

    G4double energy = prePoint->GetKineticEnergy();
    G4double x_hit = prePoint->GetPosition().getX();
    G4double y_hit = prePoint->GetPosition().getY();
    G4double z_hit = prePoint->GetPosition().getZ();
    G4double t_hit = prePoint->GetGlobalTime();

    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(0, energy / MeV);
    man->FillNtupleDColumn(1, x_hit / m);
    man->FillNtupleDColumn(2, y_hit / m);
    man->FillNtupleDColumn(3, z_hit / m);
    man->FillNtupleDColumn(4, t_hit / ns);
    man->FillNtupleDColumn(5, SensitiveDetectorID);
    man->FillNtupleDColumn(6, PID);
    man->AddNtupleRow(0);

    return true;
}