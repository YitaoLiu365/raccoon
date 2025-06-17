//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "KernelValue.h"

class PenaltyIrreversibility : public KernelValue
{
public:
  static InputParameters validParams();

  PenaltyIrreversibility(const InputParameters & parameters);

protected:
  Real precomputeQpResidual() override;
  Real precomputeQpJacobian() override;

  /// Penalty value
  const Real _penalty;

  /// Old variable value
  const VariableValue & _u_old;
};
