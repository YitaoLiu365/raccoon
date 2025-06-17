//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PenaltyIrreversibility.h"

registerMooseObject("raccoonApp", PenaltyIrreversibility);

InputParameters
PenaltyIrreversibility::validParams()
{
  InputParameters params = KernelValue::validParams();
  params.addClassDescription("Enforce irreversibility using a penalty approach.");
  params.addRequiredParam<Real>("penalty", "Penalty parameter");
  return params;
}

PenaltyIrreversibility::PenaltyIrreversibility(const InputParameters & parameters)
  : KernelValue(parameters), _penalty(getParam<Real>("penalty")), _u_old(valueOld())
{
}

Real
PenaltyIrreversibility::precomputeQpResidual()
{
  auto delta_u = (_u[_qp] - _u_old[_qp]);
  // auto delta_u = (_u[_qp] - _u_old[_qp]) / abs(_u[_qp] - _u_old[_qp]);

  if (delta_u >= 0)
    return 0;

  return _penalty * 2 * delta_u;
}

Real
PenaltyIrreversibility::precomputeQpJacobian()
{
  auto delta_u = (_u[_qp] - _u_old[_qp]);

  if (delta_u >= 0)
    return 0;

  // auto ddelta_u_du = 1;
  // return _penalty * 2 * delta_u * ddelta_u_du * _phi[_j][_qp];
  return _penalty * 2 * _phi[_j][_qp];

  // return 0;
}