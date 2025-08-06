//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "DynamicVolumePhaseFieldJIntegral.h"
#include "InputParameters.h"
#include "RankTwoTensorForward.h"
#include "Registry.h"
#include "libmesh/libmesh_common.h"

registerMooseObject("raccoonApp", DynamicVolumePhaseFieldJIntegral);

InputParameters
DynamicVolumePhaseFieldJIntegral::validParams()
{
  InputParameters params = ElementIntegralPostprocessor::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("Compute the volume integration part of the dynamic J integral for a "
                             "phase-field model of fracture");
  params.addRequiredParam<RealVectorValue>("J_direction", "direction of J integral");
  params.addRequiredParam<MaterialPropertyName>(
      "density", "The material property defining the density of the material");
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  return params;
}

DynamicVolumePhaseFieldJIntegral::DynamicVolumePhaseFieldJIntegral(
    const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    BaseNameInterface(parameters),
    _density(getADMaterialProperty<Real>("density")),
    _ndisp(coupledComponents("displacements")),
    _t(getParam<RealVectorValue>("J_direction")),
    _disp_dotdot(coupledDotDots("displacements")),
    _disp_dot(coupledDots("displacements")),
    _grad_disp(coupledGradients("displacements")),
    _grad_disp_dot(coupledGradientDots("displacements"))
{
  // set unused dimensions to zero
  for (unsigned i = _ndisp; i < 3; ++i)
  {
    _disp_dotdot.push_back(&_zero);
    _disp_dot.push_back(&_zero);
    _grad_disp.push_back(&_grad_zero);
    _grad_disp_dot.push_back(&_grad_zero);
  }
}

Real
DynamicVolumePhaseFieldJIntegral::computeQpIntegral()
{
  // Kinetic part
  RealVectorValue A((*_disp_dotdot[0])[_qp], (*_disp_dotdot[1])[_qp], (*_disp_dotdot[2])[_qp]);
  auto H1 = RankTwoTensor::initializeFromRows(
      (*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
  auto psi_k = _density[_qp] * H1.transpose() * A;

  // Inertial part
  RealVectorValue V((*_disp_dot[0])[_qp], (*_disp_dot[1])[_qp], (*_disp_dot[2])[_qp]);
  auto H2 = RankTwoTensor::initializeFromRows(
      (*_grad_disp_dot[0])[_qp], (*_grad_disp_dot[1])[_qp], (*_grad_disp_dot[2])[_qp]);
  auto psi_i = _density[_qp] * H2.transpose() * V;

  return MetaPhysicL::raw_value(_t * (psi_k - psi_i));
}
