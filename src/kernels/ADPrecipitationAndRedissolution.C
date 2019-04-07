// PrecipitationAndRedissolution AD

#include "ADPrecipitationAndRedissolution.h"

registerADMooseObject("Precipitation5App", ADPrecipitationAndRedissolution);

defineADValidParams(ADPrecipitationAndRedissolution,
                    ADKernel,
                    params.addCoupledVar("coupled_var_dissolution", "Coupled the dissolution variable for judgement.");
                    params.addCoupledVar("coupled_var_precipitation", "Coupled the precipitation variable for judgement.");
                    params.addParam<MaterialPropertyName>(
	  "solubility", "solubility", "Solubility of the solute.");
                    params.addRequiredParam<Real>("unit_scalor", "unit_scalor for the precipitate, C_Ln_pure");
                    );

template <ComputeStage compute_stage>
ADPrecipitationAndRedissolution<compute_stage>::ADPrecipitationAndRedissolution(const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters),

//Boolean type. If the variable detected here means it is applied in precipitaion equation
    _has_coupled_dissolution(isCoupled("coupled_var_dissolution")),

//Boolean type. If the variable detected here means it is applied in diffusion equation
    _has_coupled_precipitation(isCoupled("coupled_var_precipitation")),

//If coupled with c_dissolution, means it is applied in precipitaion equation, then it's -1; if not means it is in diffusion equation, then it's 1
    _sign((_has_coupled_dissolution) ? -1 : 1),

//Pass whatever it is coupled to _coupled_variable;
//If coupled with c_dissolution, then _coupled_variable is c_dissolution; 
//If coupled with c_precipitation, then _coupled_variable is c_precipitation;
    _coupled_variable((_has_coupled_dissolution) ? adCoupledValue("coupled_var_dissolution") : adCoupledValue("coupled_var_precipitation")),

    _solubility(adGetADMaterialProperty<Real>("solubility")),
    _unit_scalor(adGetParam<Real>("unit_scalor"))

{
}

template <ComputeStage compute_stage>
ADResidual
ADPrecipitationAndRedissolution<compute_stage>::computeQpResidual()
{
  ADReal r = 0.0;
  ADReal prefactor = _sign;

// this will run when applied in diffusion eqution
  if (_has_coupled_precipitation)
  {  
    if (_u[_qp] > _solubility[_qp])
      r = prefactor * (_u[_qp] - _solubility[_qp]);
    else if (_u[_qp] < _solubility[_qp] && _coupled_variable[_qp] > 0.0)
      r = prefactor * (_u[_qp] - _solubility[_qp]) * _coupled_variable[_qp]/_unit_scalor;
  }

// this will run when applied in precipitation eqution
  else if (_has_coupled_dissolution)
  {
    if (_coupled_variable[_qp] > _solubility[_qp])
      r = prefactor * (_coupled_variable[_qp] - _solubility[_qp]);  
    else if (_coupled_variable[_qp] < _solubility[_qp] && _u[_qp] > 0.0)
      r = prefactor * (_coupled_variable[_qp] - _solubility[_qp]) * _u[_qp]/_unit_scalor;
  }

  else
    mooseError("Either precipitate_variable or dissolve_variable must be provided");

  return _test[_i][_qp] * r; 
}



