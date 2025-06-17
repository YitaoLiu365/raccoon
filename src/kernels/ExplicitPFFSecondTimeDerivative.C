#include "ExplicitPFFSecondTimeDerivative.h"
#include "MooseTypes.h"

registerMooseObject("raccoonApp", ExplicitPFFSecondTimeDerivative);

InputParameters
ExplicitPFFSecondTimeDerivative::validParams()
{
  InputParameters params = MassMatrix::validParams();
  params.addClassDescription("Computes the coefficient of the second time derivative of damage");
  params.addRequiredParam<MaterialPropertyName>("bulk_modulus", "The bulk modulus of the material");
  params.addRequiredParam<MaterialPropertyName>("shear_modulus",
                                                "The shear modulus of the material");
  params.addParam<MaterialPropertyName>(
      "fracture_toughness", "Gc", "The fracture toughness of the material");
  params.addParam<MaterialPropertyName>(
      "regularization_length", "l", "The regularization length for the damage model");
  return params;
}

ExplicitPFFSecondTimeDerivative::ExplicitPFFSecondTimeDerivative(const InputParameters & params)
  : MassMatrix(params),
    _K(getMaterialProperty<Real>("bulk_modulus")),
    _G(getMaterialProperty<Real>("shear_modulus")),
    _Gc(getMaterialProperty<Real>("fracture_toughness")),
    _l(getMaterialProperty<Real>("regularization_length"))
{
}

Real
ExplicitPFFSecondTimeDerivative::computeQpJacobian()
{
  auto c = std::sqrt((_K[_qp] + 4. / 3 * _G[_qp]) / _density[_qp]);
  return _test[_i][_qp] *_Gc[_qp] * _l[_qp] / c / c * _phi[_j][_qp];
}