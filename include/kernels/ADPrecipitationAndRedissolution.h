#ifndef ADPRECIPITATATIONANDREDISSOLUTION_H
#define ADPRECIPITATATIONANDREDISSOLUTION_H

#include "ADKernel.h"

template <ComputeStage compute_stage>
class ADPrecipitationAndRedissolution;

declareADValidParams(ADPrecipitationAndRedissolution);

template <ComputeStage compute_stage>
class ADPrecipitationAndRedissolution : public ADKernel<compute_stage>
{
public:
  ADPrecipitationAndRedissolution(const InputParameters & parameters);

protected:

  virtual ADResidual computeQpResidual() override;

  const bool _has_coupled_dissolution;
  const bool _has_coupled_precipitation;
  const int _sign;
  const ADVariableValue & _coupled_variable;
 
  const ADMaterialProperty(Real) & _solubility;
  const Real _unit_scalor;

  usingKernelMembers;

};

#endif // PRECIPITATATIONANDREDISSOLUTION_H
