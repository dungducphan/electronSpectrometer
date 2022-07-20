#include "spectrometerSD.hh"

spectrometerSD::spectrometerSD(const G4String &name) : G4VSensitiveDetector(name) {
    delimiter = "_";
}

spectrometerSD::~spectrometerSD() {}

void spectrometerSD::ParseVolumeName(std::string DetectorName, unsigned int& UnitID, unsigned int& X_index, unsigned int& Y_index) {
    size_t pos = 0;
    std::vector<std::string> token;
    while ((pos = DetectorName.find(delimiter)) != std::string::npos) {
        token.push_back(DetectorName.substr(0, pos));
        DetectorName.erase(0, pos + delimiter.length());
    }
    token.push_back(DetectorName);

    UnitID  = std::stoi(token.at(1));
    X_index = std::stoi(token.at(2));
    Y_index = std::stoi(token.at(3));
}

G4bool spectrometerSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {
    G4Track *track = aStep->GetTrack();

    if (track->GetDefinition() != G4ParticleTable::GetParticleTable()->FindParticle(11) &&
        track->GetDefinition() != G4ParticleTable::GetParticleTable()->FindParticle(22)) {
        return false;
    }

    G4int PID = track->GetDefinition()->GetPDGEncoding();

    G4StepPoint *prePoint = aStep->GetPreStepPoint();
    std::string det_name = prePoint->GetPhysicalVolume()->GetName();
    unsigned int unitID, Xcell_idx, Ycell_idx;
    ParseVolumeName(det_name, unitID, Xcell_idx, Ycell_idx);

    G4double energy = prePoint->GetKineticEnergy();
    G4double edep  = aStep->GetTotalEnergyDeposit();

    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(0, energy / MeV);
    man->FillNtupleDColumn(1, Xcell_idx);
    man->FillNtupleDColumn(2, Ycell_idx);
    man->FillNtupleDColumn(3, unitID);
    man->FillNtupleDColumn(4, PID);
    man->FillNtupleDColumn(5, edep / eV);
    man->AddNtupleRow(0);

    return true;
}