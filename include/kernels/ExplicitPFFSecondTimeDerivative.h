#pragma once

#include "MassMatrix.h"

class ExplicitPFFSecondTimeDerivative : public MassMatrix
{
public:
    static InputParameters  validParams();
    ExplicitPFFSecondTimeDerivative(const InputParameters & parameters);

protected:
    Real computeQpJacobian() override;

    const MaterialProperty<Real> & _K;
    const MaterialProperty<Real> & _G;
    const MaterialProperty<Real> & _Gc;
    const MaterialProperty<Real> & _l;
};