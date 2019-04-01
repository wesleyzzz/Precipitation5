#include "Precipitation5App.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

template <>
InputParameters
validParams<Precipitation5App>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

Precipitation5App::Precipitation5App(InputParameters parameters) : MooseApp(parameters)
{
  Precipitation5App::registerAll(_factory, _action_factory, _syntax);
}

Precipitation5App::~Precipitation5App() {}

void
Precipitation5App::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAll(f, af, s);
  Registry::registerObjectsTo(f, {"Precipitation5App"});
  Registry::registerActionsTo(af, {"Precipitation5App"});

  /* register custom execute flags, action syntax, etc. here */
}

void
Precipitation5App::registerApps()
{
  registerApp(Precipitation5App);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
Precipitation5App__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  Precipitation5App::registerAll(f, af, s);
}
extern "C" void
Precipitation5App__registerApps()
{
  Precipitation5App::registerApps();
}
