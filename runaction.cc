#include "runaction.hh"

runAction::runAction() : G4UserRunAction() {
    man = G4AnalysisManager::Instance();
    man->CreateNtuple("BeamImage", "BeamImage");
    man->CreateNtupleDColumn("E");
    man->CreateNtupleDColumn("X");
    man->CreateNtupleDColumn("Y");
    man->CreateNtupleDColumn("Z");
    man->CreateNtupleDColumn("T");
    man->CreateNtupleDColumn("detID");
    man->FinishNtuple(0);
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

