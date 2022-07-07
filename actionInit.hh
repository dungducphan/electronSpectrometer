#include "G4VUserActionInitialization.hh"

#include "generator.hh"
#include "runaction.hh"

class actionInit : public G4VUserActionInitialization {
  public:
    actionInit(std::vector<std::pair<G4double, G4double>> gunEne);
    virtual ~actionInit();

    virtual void Build() const;
    virtual void BuildForMaster() const;

    std::vector<std::pair<G4double, G4double>> gunEneDist;
};
