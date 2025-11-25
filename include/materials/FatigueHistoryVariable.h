#pragma once

#include "Material.h"
#include "MaterialProperty.h"

class FatigueHistoryVariable : public Material
{
public:
  static InputParameters validParams();

  FatigueHistoryVariable(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  void computeQpProperties() override;

  const MaterialPropertyName _alpha_bar_name;
  ADMaterialProperty<Real> & _alpha_bar;
  const MaterialProperty<Real> & _alpha_bar_old;
  const MaterialPropertyName _alpha_name;
  const ADMaterialProperty<Real> & _alpha;
  const MaterialProperty<Real> & _alpha_old;
  const VariableValue & _init;
  bool _fromZero;
};
