#pragma once

#include "Material.h"
#include "MaterialProperty.h"

class HistoryMaximum : public Material
{
public:
  static InputParameters validParams();

  HistoryMaximum(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  void computeQpProperties() override;

  ADMaterialProperty<Real> & _hist_max;
  const MaterialProperty<Real> & _hist_max_old;
  const ADMaterialProperty<Real> & _prop;
  const ADMaterialProperty<Real> & _minimum;
};