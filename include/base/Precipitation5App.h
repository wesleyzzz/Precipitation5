//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#ifndef PRECIPITATION5APP_H
#define PRECIPITATION5APP_H

#include "MooseApp.h"

class Precipitation5App;

template <>
InputParameters validParams<Precipitation5App>();

class Precipitation5App : public MooseApp
{
public:
  Precipitation5App(InputParameters parameters);
  virtual ~Precipitation5App();

  static void registerApps();
  static void registerAll(Factory & f, ActionFactory & af, Syntax & s);
};

#endif /* PRECIPITATION5APP_H */
