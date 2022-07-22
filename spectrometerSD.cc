#include "spectrometerSD.hh"

spectrometerSD::spectrometerSD(const G4String &name) : G4VSensitiveDetector(name) {
    delimiter = "_";

    G4AnalysisManager* man = G4AnalysisManager::Instance();
    DRZC_ID = man->GetH2Id("DRZC");
    DRZ1_ID = man->GetH2Id("DRZ1");
    DRZ2_ID = man->GetH2Id("DRZ2");
    IMPL_ID = man->GetH2Id("IMPL");
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

    G4double edep  = aStep->GetTotalEnergyDeposit();

    G4AnalysisManager* man = G4AnalysisManager::Instance();
    if (unitID == 0) man->FillH2(DRZC_ID, Xcell_idx + 0.1, Ycell_idx + 0.1, edep / eV);
    if (unitID == 1) man->FillH2(DRZ1_ID, Xcell_idx + 0.1, Ycell_idx + 0.1, edep / eV);
    if (unitID == 3) man->FillH2(DRZ2_ID, Xcell_idx + 0.1, Ycell_idx + 0.1, edep / eV);
    if (unitID == 2) man->FillH2(IMPL_ID, Xcell_idx + 0.1, Ycell_idx + 0.1, edep / eV);

    return true;
}