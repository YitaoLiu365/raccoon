#pragma once

#include "Material.h"
#include "MaterialProperty.h"
#include "BaseNameInterface.h"

class FatigueDegradation : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();
  FatigueDegradation(const InputParameters & parameters);

protected:
  void computeQpProperties() override;

  ADMaterialProperty<Real> & _f; // the value of the fatigue degradation function
  const ADMaterialProperty<Real> & _Gc;
  const ADMaterialProperty<Real> & _l;
  const ADMaterialProperty<Real> & _alpha_bar;
  const enum class CrackGeometricModel { AT1, AT2 } _crack_geometric_func;
};
