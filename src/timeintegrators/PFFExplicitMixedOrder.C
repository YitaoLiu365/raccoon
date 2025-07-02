//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "InputParameters.h"
#include "PFFExplicitMixedOrder.h"

registerMoodeObject("raccoonApp", PFFExplicitMixedOrder);

InputParameters
PFFExplicitMixedOrder::validParams()
{
  InputParameters params = ExplicitMixedOrder::validParams();
  params.addParam<std::vector<VariableName>>(
      "phase_field_variables",
      {},
      "A subset of variables that require phase field to be applied by "
      "this time integrator.");

  return params;
}

PFFExplicitMixedOrder::PFFExplicitMixedOrder(const InputParameters & parameters)
  : ExplicitMixedOrder(parameters),
    _vars_d(declareRestartableData<std::unordered_set<unsigned int>>("phase_field_variables")),
    _local_d_indices(declareRestartableData<std::vector<dof_id_type>>("local_phase_field_indices"))
{
  _fe_problem.setUDotRequested(true);
  _fe_problem.setUDotOldRequested(true);
  _fe_problem.setUDotDotRequested(true);

  // This effectively changes the default solve_type to LINEAR instead of PJFNK,
  // so that it is valid to not supply solve_type in the Executioner block:
  if (_nl)
    _fe_problem.solverParams(_nl->number())._type = Moose::ST_LINEAR;

  _ones = addVector("ones", true, PARALLEL);

  // don't set any of the common SNES-related petsc options to prevent unused option warnings
  Moose::PetscSupport::dontAddCommonSNESOptions(_fe_problem);
}
