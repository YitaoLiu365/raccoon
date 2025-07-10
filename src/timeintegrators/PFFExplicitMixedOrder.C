//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

// MOOSE includes
#include "Assembly.h"
#include "ExplicitMixedOrder.h"
#include "MeshChangedInterface.h"
#include "Moose.h"
#include "MooseError.h"
#include "MooseTypes.h"
#include "MooseVariableFieldBase.h"
#include "NonlinearSystem.h"

// libMesh includes
#include "TransientBase.h"
#include "libmesh/nonlinear_solver.h"
#include "libmesh/sparse_matrix.h"
#include <algorithm>
#include <iterator>
#include <utility>

#include "InputParameters.h"
#include "PFFExplicitMixedOrder.h"

registerMooseObject("raccoonApp", PFFExplicitMixedOrder);

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
}

void
PFFExplicitMixedOrder::initPF()
{
  std::vector<unsigned int> var_num_vec;
  auto & lm_sys = _sys.system();
  lm_sys.get_all_variable_numbers(var_num_vec);
  const auto & var_names_d = getParam<std::vector<VariableName>>("phase_field_variables");
  std::unordered_set<unsigned int> var_nums_d(var_num_vec.begin(), var_num_vec.end());
  for (const auto & var_name : var_names_d)
    if (lm_sys.has_variable(var_name))
    {
      const auto var_num = lm_sys.variable_number(var_name);
      _vars_d.insert(var_num);
      var_nums_d.erase(var_num);
    }
  std::vector<dof_id_type> var_d_indices, d_vec;
  for (const auto var_num : _vars_d)
  {
    d_vec = _local_d_indices;
    _local_d_indices.clear();
    lm_sys.get_dof_map().local_variable_indices(var_d_indices, lm_sys.get_mesh(), var_num);
    std::merge(d_vec.begin(),
               d_vec.end(),
               var_d_indices.begin(),
               var_d_indices.end(),
               std::back_inserter(_local_d_indices));
  }
}

void
PFFExplicitMixedOrder::upperboundCheck()
{
  auto & nlSol = *_nonlinear_implicit_system->solution;
  auto & nlSolOld = _nl->solutionOld();
  auto nlSol_d = nlSol.get_subvector(_local_d_indices);
  auto nlSolOld_d = nlSolOld.get_subvector(_local_d_indices);
  auto vel = _sys.solutionUDot();
  auto v_d = vel->get_subvector(_local_d_indices);
  for (auto i = nlSol_d->first_local_index(); i < nlSol_d->last_local_index(); ++i)
  {
    if ((*nlSol_d)(i) > 1.0)
    {
      nlSol_d->set(i, 1.0);
      v_d->set(i, (1 - (*nlSolOld_d)(i)) / _dt);
    }
  }
  nlSol.restore_subvector(std::move(nlSol_d), _local_d_indices);
  vel->restore_subvector(std::move(v_d), _local_d_indices);
  nlSol.close();
  vel->close();
}

void
PFFExplicitMixedOrder::irreversibilityCheck(NumericVector<Number> * accel,
                                            NumericVector<Number> * vel)
{
  auto accel_d = accel->get_subvector(_local_d_indices);
  auto vel_d = vel->get_subvector(_local_d_indices);
  for (auto i = vel_d->first_local_index(); i < vel_d->last_local_index(); ++i)
  {
    if ((*vel_d)(i) < 0.0)
    {
      accel_d->set(i, (*accel_d)(i) - (*vel_d)(i) * 2 / (_dt + _dt_old));
      vel_d->set(i, 0.0);
    }
  }
  accel->restore_subvector(std::move(accel_d), _local_d_indices);
  vel->restore_subvector(std::move(vel_d), _local_d_indices);
}
