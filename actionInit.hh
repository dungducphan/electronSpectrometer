#include "G4VUserActionInitialization.hh"

#include "generator.hh"
#include "runaction.hh"
#include "eventaction.hh"

class actionInit : public G4VUserActionInitialization {
  public:
    actionInit();
    virtual ~actionInit();

    virtual void Build() const;
    virtual void BuildForMaster() const;
};
