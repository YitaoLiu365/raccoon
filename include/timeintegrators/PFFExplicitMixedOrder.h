//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ExplicitMixedOrder.h"

class PFFExplicitMixedOrder : public ExplicitMixedOrder
{
public:
  static InputParameters validParams();
  PFFExplicitMixedOrder(const InputParameters & parameters);

  virtual void init() override { ExplicitMixedOrder::init(); }
  virtual void solve() override { ExplicitMixedOrder::solve(); }

protected:
  /// Calculate acceleration and velocity using the central difference method
  virtual void discretize() override { ExplicitMixedOrder::discretize(); }

  /// Helper functions of phase-field
  virtual void initPF() override;
  virtual void upperboundCheck() override;
  virtual void irreversibilityCheck(NumericVector<Number> * accel,
                                    NumericVector<Number> * vel) override;

  /// Variables for phase-field
  std::unordered_set<unsigned int> & _vars_d;

  /// Local dofs representing phase-field
  std::vector<dof_id_type> & _local_d_indices;
};
