#pragma once 

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

class G4Run;

class eventAction : public G4UserEventAction {
  public:
    eventAction();
    virtual ~eventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);

  private:
    G4AnalysisManager *man;
};
