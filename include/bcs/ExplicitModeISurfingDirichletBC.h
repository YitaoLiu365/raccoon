//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ExplicitDirichletBCBase.h"
#include "InputParameters.h"

class ExplicitModeISurfingDirichletBC : public ExplicitDirichletBCBase
{
public:
  static InputParameters validParams();

  ExplicitModeISurfingDirichletBC(const InputParameters & parameters);

protected:
  virtual Real computeQpValue() override;

  const Point & _c;
  const Real _tc;
  const RealVectorValue & _v;
  const unsigned int _component;
  const Real _Gc;
  const Real _K;
  const Real _G;
};
