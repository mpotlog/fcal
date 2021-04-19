#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
//#include "SteppingAction.hh"


ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{
  PrimaryGeneratorAction* primaryAction = new PrimaryGeneratorAction();
    SetUserAction(primaryAction);
  EventAction* eventAction = new EventAction();
    SetUserAction(eventAction);
  RunAction* runAction = new RunAction(eventAction);
    SetUserAction(runAction);
    //SetUserAction(new SteppingAction);
}  


