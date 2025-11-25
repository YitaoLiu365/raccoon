#include "FatigueHistoryVariable.h"

registerMooseObject("raccoonApp", FatigueHistoryVariable);

InputParameters
FatigueHistoryVariable::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("History variable for fatigue calculation.");
  params.addParam<MaterialPropertyName>(
      "property_name", "alpha_bar", "Name of the fatigue history variable to be computed.");
  params.addRequiredParam<MaterialPropertyName>(
      "history_quantity", "The name of the property to take as the history quantity.");
  params.addRequiredCoupledVar("initial", "The initial value of the history variable.");
  params.addParam<bool>(
      "start_from_zero", true, "Whether the history variable starts to accumulate from zero.");
  return params;
}

FatigueHistoryVariable::FatigueHistoryVariable(const InputParameters & parameters)
  : Material(parameters),
    _alpha_bar_name(getParam<MaterialPropertyName>("property_name")),
    _alpha_bar(declareADProperty<Real>(_alpha_bar_name)),
    _alpha_bar_old(getMaterialPropertyOld<Real>(_alpha_bar_name)),
    _alpha_name(getParam<MaterialPropertyName>("history_quantity")),
    _alpha(getADMaterialProperty<Real>(_alpha_name)),
    _alpha_old(getMaterialPropertyOld<Real>(_alpha_name)),
    _init(coupledValue("initial")),
    _fromZero(getParam<bool>("start_from_zero"))
{
}

void
FatigueHistoryVariable::initQpStatefulProperties()
{
  _alpha_bar[_qp] = _fromZero ? 0 : _init[_qp];
}

void
FatigueHistoryVariable::computeQpProperties()
{
  // _alpha_bar[_qp] = _alpha_bar_old[_qp];

  if (_alpha[_qp] > _alpha_old[_qp])
  {
    // _console << "TRUE: "
    //          << MetaPhysicL::raw_value(_alpha_bar[_qp] -
    //                                    (_alpha_bar_old[_qp] + _alpha[_qp] - _alpha_old[_qp]))
    //          << std::endl;
    // if (_alpha_bar[_qp] - (_alpha_bar_old[_qp] + _alpha[_qp] - _alpha_old[_qp]) > 0)
    // {
    //   std::cout << "ISSUE " << MetaPhysicL::raw_value(_alpha[_qp] - _alpha_old[_qp]) <<
    //   std::endl; std::cout << "_alpha " << MetaPhysicL::raw_value(_alpha[_qp]) << std::endl;
    //   std::cout << "_alpha_old " << MetaPhysicL::raw_value(_alpha_old[_qp]) << std::endl;
    //   std::cout << "alpha_bar_old " << MetaPhysicL::raw_value(_alpha_bar_old[_qp]) << std::endl;
    //   std::cout << "alpha_bar_before_updated " << MetaPhysicL::raw_value(_alpha_bar[_qp])
    //             << std::endl;
    //   std::cout << "alpha_bar_real "
    //             << MetaPhysicL::raw_value(_alpha_bar_old[_qp] + _alpha[_qp] - _alpha_old[_qp])
    //             << std::endl;
    // }
    _alpha_bar[_qp] = _alpha_bar_old[_qp] + _alpha[_qp] - _alpha_old[_qp];
  }

  else
  {
    // if (_qp == 0)
    // {
    //   std::cout << "alpha_bar_old " << MetaPhysicL::raw_value(_alpha_bar_old[_qp]) << std::endl;
    //   std::cout << "alpha_bar_before_updated " << MetaPhysicL::raw_value(_alpha_bar[_qp])
    //             << std::endl;
    // }
    _alpha_bar[_qp] = _alpha_bar_old[_qp];
  }

  // std::cout << "alpha_bar_final " << MetaPhysicL::raw_value(_alpha_bar[_qp]) << std::endl;

  // if (_t_step == 5 && int(_current_elem->id()) == 32035)
  // {
  // _console << "element id: " << _current_elem->id() << std::endl;
  // if (_qp == 0)
  //   _console << "_alpha_bar " << MetaPhysicL::raw_value(_alpha_bar[_qp]) << "; " << std::endl;

  //          << "alpha_bar: " << MetaPhysicL::raw_value(_alpha_bar[_qp]) << "; " << std::endl;
  //  << "alpha: " << MetaPhysicL::raw_value(_alpha[_qp]) << "; "
  //  << "alpha_bar: " << MetaPhysicL::raw_value(_alpha_bar[_qp]) << std::endl;
  // }
}
