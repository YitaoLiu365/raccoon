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

  virtual void initPF() override;
  virtual void irreversibilityCheck() override;
  virtual void upperboundCheck() override;

protected:
  // Variables for phase-field
  std::unordered_set<unsigned int> & _vars_d;
  // Local dofs representing phase-field
  std::vector<dof_id_type> & _local_d_indices;
};
