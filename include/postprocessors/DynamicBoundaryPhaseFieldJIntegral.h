//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "PhaseFieldJIntegral.h"
#include "SideIntegralPostprocessor.h"

class DynamicBoundaryPhaseFieldJIntegral : public PhaseFieldJIntegral
{
public:
  static InputParameters validParams();
  DynamicBoundaryPhaseFieldJIntegral(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;

  const ADMaterialProperty<Real> & _density;
  std::vector<const VariableValue *> _disp_dot;
};
