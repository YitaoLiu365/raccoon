//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"
#include "BaseNameInterface.h"
#include "Material.h"
#include "MooseTypes.h"

class DynamicVolumePhaseFieldJIntegral : public ElementIntegralPostprocessor,
                                         public BaseNameInterface
{
public:
  static InputParameters validParams();
  DynamicVolumePhaseFieldJIntegral(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;

  const ADMaterialProperty<Real> & _density;
  const unsigned int _ndisp;
  const RealVectorValue _t;
  std::vector<const VariableValue *> _disp_dotdot;
  std::vector<const VariableValue *> _disp_dot;
  std::vector<const VariableGradient *> _grad_disp;
  std::vector<const VariableGradient *> _grad_disp_dot;
};
