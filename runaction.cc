#include "runaction.hh"

runAction::runAction() : G4UserRunAction() {
    man = G4AnalysisManager::Instance();
    man->CreateH2("DRZC", "", 762, 0, 762, 762, 0, 762);
    man->CreateH2("DRZ1", "", 2286, 0, 2286, 635, 0, 635);
    man->CreateH2("IMPL", "", 3500, 0, 3500, 1500, 0, 1500);
    man->CreateH2("DRZ2", "", 2286, 0, 2286, 635, 0, 635);

    man->CreateH1("Energy", "", 5000, 0, 10);
}

runAction::~runAction() {}

void runAction::BeginOfRunAction(const G4Run *run) {
    man = G4AnalysisManager::Instance();
    man->OpenFile(Form("BeamImage_run%05i.root", (int) run->GetRunID()));
}

void runAction::EndOfRunAction(const G4Run *) {
    man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}

