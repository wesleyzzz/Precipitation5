//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#ifndef PRECIPITATION5TESTAPP_H
#define PRECIPITATION5TESTAPP_H

#include "MooseApp.h"

class Precipitation5TestApp;

template <>
InputParameters validParams<Precipitation5TestApp>();

class Precipitation5TestApp : public MooseApp
{
public:
  Precipitation5TestApp(InputParameters parameters);
  virtual ~Precipitation5TestApp();

  static void registerApps();
  static void registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs = false);
};

#endif /* PRECIPITATION5TESTAPP_H */
