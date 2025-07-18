//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "DynamicBoundaryPhaseFieldJIntegral.h"
#include "InputParameters.h"
#include "Registry.h"
#include "libmesh/libmesh_common.h"

registerMooseObject("raccoonApp", DynamicBoundaryPhaseFieldJIntegral);

InputParameters
DynamicBoundaryPhaseFieldJIntegral::validParams()
{
  InputParameters params = PhaseFieldJIntegral::validParams();
  params.addClassDescription("Compute the boundary integration part of the dynamic J integral for "
                             "a phase-field model of fracture ");
  params.addParam<MaterialPropertyName>(
      "density", 1, "The material property defining the density of the material");
  return params;
}

DynamicBoundaryPhaseFieldJIntegral::DynamicBoundaryPhaseFieldJIntegral(
    const InputParameters & parameters)
  : PhaseFieldJIntegral(parameters),
    _density(getADMaterialPropertyByName<Real>("density")),
    _disp_dot(coupledDots("displacements"))
{
  // set unused dimensions to zero
  for (unsigned i = _ndisp; i < 3; ++i)
    _disp_dot.push_back(&_zero);
}

Real
DynamicBoundaryPhaseFieldJIntegral::computeQpIntegral()
{
  // Kinetic energy
  auto psik =
      0.5 * _density[_qp] *
      ((*_disp_dot[0])[_qp] * (*_disp_dot[0])[_qp] + (*_disp_dot[1])[_qp] * (*_disp_dot[1])[_qp] +
       (*_disp_dot[2])[_qp] * (*_disp_dot[2])[_qp]);
  // H = grad(displacements)
  auto H = RankTwoTensor::initializeFromRows(
      (*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
  RankTwoTensor I2(RankTwoTensor::initIdentity);
  ADRankTwoTensor Sigma = (_psie[_qp] + psik) * I2 - H.transpose() * _stress[_qp];
  RealVectorValue n = _normals[_qp];

  // Include the dynamic term
  return raw_value(_t * Sigma * n);
}
