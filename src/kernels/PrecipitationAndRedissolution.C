// PrecipitationAndRedissolution 

#include "PrecipitationAndRedissolution.h"

registerMooseObject("Precipitation5App", PrecipitationAndRedissolution);

template <>
InputParameters
validParams<PrecipitationAndRedissolution>()
{
  InputParameters params = validParams<Kernel>();

// Because this kernel suppose to work on two varibales (c_dissolution and c_precipitation), so we'll need to use
// coupled_var_dissolution/couple_var_precipitation to see which siutaion it is. 
// If it is applied in diffusion equation, it uses its own value (_u) and coupled precipitation_var;
// If it is applied in precipitaion equation, it uses its own value (_u) and coupled dissolution_var;
 
  params.addCoupledVar(
      "coupled_var_dissolution", "Coupled the dissolution variable for judgement.");
  params.addCoupledVar(
      "coupled_var_precipitation", "Coupled the precipitation variable for judgement.");
  params.addParam<MaterialPropertyName>(
	  "solubility", 0.0, "Solubility of the solute.");
  return params;
}

PrecipitationAndRedissolution::PrecipitationAndRedissolution(const InputParameters & parameters)
  : Kernel(parameters),

//Boolean type. If the variable detected here means it is applied in precipitaion equation
    _has_coupled_dissolution(isCoupled("coupled_var_dissolution")),

//Boolean type. If the variable detected here means it is applied in diffusion equation
    _has_coupled_precipitation(isCoupled("coupled_var_precipitation")),

//If coupled with c_dissolution, means it is applied in precipitaion equation, then it's -1; if not means it is in diffusion equation, then it's 1
    _sign((_has_coupled_dissolution) ? -1 : 1),

//Pass whatever it is coupled to _coupled_variable;
//If coupled with c_dissolution, then _coupled_variable is c_dissolution; 
//If coupled with c_precipitation, then _coupled_variable is c_precipitation;
    _coupled_variable((_has_coupled_dissolution) ? coupledValue("coupled_var_dissolution") : coupledValue("coupled_var_precipitation")),

// Save off the coupled variable identifier for use in
// computeQpOffDiagJacobian
    _coupled_variable_offJac((_has_coupled_dissolution) ? coupled("coupled_var_dissolution") : coupled("coupled_var_precipitation")),

    _solubility(getMaterialProperty<Real>(getParam<MaterialPropertyName>("solubility")))
{
}

Real
PrecipitationAndRedissolution::computeQpResidual()
{
  Real r = 0.0;
  Real prefactor = _sign;

// this will run when applied in diffusion eqution
  if (_has_coupled_precipitation)
  {  
    if (_u[_qp] > _solubility[_qp])
      r = prefactor * (_u[_qp] - _solubility[_qp]);
    else if (_u[_qp] < _solubility[_qp] && _coupled_variable[_qp] > 0.0)
      r = prefactor * (_u[_qp] - _solubility[_qp]) * _coupled_variable[_qp];
  }

// this will run when applied in precipitation eqution
  else if (_has_coupled_dissolution)
  {
    if (_coupled_variable[_qp] > _solubility[_qp])
      r = prefactor * (_coupled_variable[_qp] - _solubility[_qp]);  
    else if (_coupled_variable[_qp] < _solubility[_qp] && _u[_qp] > 0.0)
      r = prefactor * (_coupled_variable[_qp] - _solubility[_qp]) * _u[_qp];
  }

  else
    mooseError("Either precipitate_variable or dissolve_variable must be provided");

  return _test[_i][_qp] * r; 
}


Real
PrecipitationAndRedissolution::computeQpJacobian()
{
  Real jac = 0.0;
  Real prefactor = _sign;

// this is for diffusion eqution
  if (_has_coupled_precipitation)
  {  
    if (_u[_qp] > _solubility[_qp])
      jac = prefactor * _phi[_j][_qp];
    else if (_u[_qp] < _solubility[_qp] && _coupled_variable[_qp] > 0.0)
      jac = prefactor * _phi[_j][_qp] * _coupled_variable[_qp];
  }

// this is for precipitation eqution
  else if (_has_coupled_dissolution)
  {
    if (_coupled_variable[_qp] > _solubility[_qp])
      jac = 0.0;  
    else if (_coupled_variable[_qp] < _solubility[_qp] && _u[_qp] > 0.0)
      jac = prefactor * (_coupled_variable[_qp] - _solubility[_qp]) * _phi[_j][_qp];
  }

  return -_test[_i][_qp] * jac;//-sign creat bad jac
}


Real
PrecipitationAndRedissolution::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _coupled_variable_offJac)
  {
  Real offjac = 0.0;
  Real prefactor = _sign;

// this is for diffusion eqution
  if (_has_coupled_precipitation)
  {  
    if (_u[_qp] > _solubility[_qp])
      offjac = 0.0;
    else if (_u[_qp] < _solubility[_qp] && _coupled_variable[_qp] > 0.0)
      offjac = prefactor * _phi[_j][_qp] * ( _u[_qp] - _solubility[_qp]);
  }

// this is for precipitation eqution
  else if (_has_coupled_dissolution)
  {
    if (_coupled_variable[_qp] > _solubility[_qp])
      offjac = prefactor * _phi[_j][_qp];  
    else if (_coupled_variable[_qp] < _solubility[_qp] && _u[_qp] > 0.0)
      offjac = prefactor * _phi[_j][_qp]* _u[_qp];
  }

  return -_test[_i][_qp] * offjac;//-sign creat bad jac
  
  }

  else return 0.0;
}

