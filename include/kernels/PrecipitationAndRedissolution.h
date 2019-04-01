#ifndef PRECIPITATATIONANDREDISSOLUTION_H
#define PRECIPITATATIONANDREDISSOLUTION_H

#include "Kernel.h"

class PrecipitationAndRedissolution;

template <>
InputParameters validParams<PrecipitationAndRedissolution>();

class PrecipitationAndRedissolution : public Kernel
{
public:
  PrecipitationAndRedissolution(const InputParameters & parameters);

protected:

  virtual Real computeQpResidual() override;

  virtual Real computeQpJacobian() override;

  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;


  const bool _has_coupled_dissolution;
  const bool _has_coupled_precipitation;
  const int _sign;
  const VariableValue & _coupled_variable;
  unsigned int _coupled_variable_offJac;
  const MaterialProperty<Real> & _solubility;

};

#endif // PRECIPITATATIONANDREDISSOLUTION_H
