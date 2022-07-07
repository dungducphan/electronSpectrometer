#include "actionInit.hh"

actionInit::actionInit(std::vector<std::pair<G4double, G4double>> gunEne) : G4VUserActionInitialization() {
    gunEneDist = gunEne;
}

actionInit::~actionInit() {}

void actionInit::BuildForMaster() const {
  SetUserAction(new runAction());
}

void actionInit::Build() const {
  SetUserAction(new runAction());
  SetUserAction(new generator(gunEneDist));
}
