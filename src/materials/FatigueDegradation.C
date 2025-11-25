#include "FatigueDegradation.h"

registerMooseObject("raccoonApp", FatigueDegradation);

InputParameters
FatigueDegradation::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("Degradation for fatigue calculation.");
  params.addParam<MaterialPropertyName>(
      "property_name", "f", "Name of the fatigue degradation function.");
  params.addParam<MaterialPropertyName>(
      "fracture_toughness", "Gc", "energy release rate or fracture toughness");
  params.addParam<MaterialPropertyName>(
      "regularization_length", "l", "the phase field regularization length");
  params.addParam<MaterialPropertyName>(
      "history_variable", "History variable to take as the variable of the degradation function.");
  params.addParam<MooseEnum>("crack_geometric_model",
                             MooseEnum("AT1 AT2", "AT1"),
                             "The crack geometric model (AT1 or AT2)");

  return params;
}

FatigueDegradation::FatigueDegradation(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _f(declareADProperty<Real>("property_name")),
    _Gc(getADMaterialProperty<Real>(prependBaseName("fracture_toughness", true))),
    _l(getADMaterialProperty<Real>(prependBaseName("regularization_length", true))),
    _alpha_bar(getADMaterialProperty<Real>("history_variable")),
    _crack_geometric_func(
        getParam<MooseEnum>("crack_geometric_model").getEnum<CrackGeometricModel>())
{
}

void
FatigueDegradation::computeQpProperties()
{
  ADReal alpha_T = _Gc[_qp] / _l[_qp];
  if (_crack_geometric_func == CrackGeometricModel::AT2)
    alpha_T *= 1.0 / 12.0;
  else
    alpha_T *= 3.0 / 32.0;

  _f[_qp] = _alpha_bar[_qp] < alpha_T ? 1 : pow(2 * alpha_T / (_alpha_bar[_qp] + alpha_T), 2);
}
