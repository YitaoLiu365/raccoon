#include "HistoryMaximum.h"

registerMooseObject("raccoonApp", HistoryMaximum);

InputParameters
HistoryMaximum::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Historical maximum of a material property.");
  params.addRequiredParam<MaterialPropertyName>("prop",
                                                "The name of the property to take maximum of.");
  params.addParam<MaterialPropertyName>("minimum",
                                        "The minimum value of the historical maximum property.");
  params.addParam<MaterialPropertyName>(
      "history_maximum", "hist_max", "Name of the historical maximum property to be computed.");
  return params;
}

HistoryMaximum::HistoryMaximum(const InputParameters & parameters)
  : Material(parameters),
    _hist_max(declareADProperty<Real>("history_maximum")),
    _hist_max_old(getMaterialPropertyOld<Real>("history_maximum")),
    _prop(getADMaterialProperty<Real>("prop")),
    _minimum(getADMaterialProperty<Real>("minimum"))
{
}

void
HistoryMaximum::initQpStatefulProperties()
{
  _hist_max[_qp] = 0.0;
}

void
HistoryMaximum::computeQpProperties()
{
  if (_t_step == 0)
  {
    _hist_max[_qp] = std::max(_prop[_qp], _minimum[_qp]);

    // _hist_max[_qp] = _prop[_qp];
    // _console << "psie_active: " << raw_value(_prop[_qp]) << std::endl;
    // _console << "psie_active threshold: " << raw_value(_minimum[_qp]) << std::endl;
    // _console << "history_max: " << raw_value(_hist_max[_qp]) << std::endl;
    return;
  }

  // Update the historical maximum property
  _hist_max[_qp] = std::max(_hist_max_old[_qp], _prop[_qp]);
  // Ensure it does not go below the minimum
  _hist_max[_qp] = std::max(_hist_max[_qp], _minimum[_qp]);
}