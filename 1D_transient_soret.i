# This is the most reasonable model
inactive = 'VectorPostprocessors'
[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 50
  xmax = 2
  elem_type = EDGE3
[]

[AuxVariables]
  [T]
  []
[]

[Variables]
  [C_d]
    order = SECOND
    family = LAGRANGE
  []
  [C_p]
    family = LAGRANGE
    order = FIRST
    inactive = 'InitialCondition'
    [InitialCondition]
      type = ConstantIC
      value = 0.5
      variable = C_p
    []
  []
[]

[Kernels]
  [C_souce]
    type = BodyForce
    value = 1
    variable = C_d
  []
  [C_d_dot]
    type = TimeDerivative
    variable = C_d
  []
  [C_d_diffusion]
    type = MatDiffusion
    variable = C_d
  []
  [C_d_precipitation_and_redissolution]
    type = ADPrecipitationAndRedissolution
    coupled_var_precipitation = 'C_p'
    solubility = solubility_solid
    variable = C_d
  []
  [C_p_dot]
    type = TimeDerivative
    variable = C_p
  []
  [C_p_precipitation_and_redissolution]
    type = ADPrecipitationAndRedissolution
    solubility = solubility_solid
    coupled_var_dissolution = 'C_d'
    variable = C_p
  []
  [soret]
    type = SoretDiffusion
    c = 'C_d'
    T = 'T'
    diff_name = D
    Q_name = Qstar
    variable = C_d
  []
[]

[AuxKernels]
  [Temp]
    type = FunctionAux
    variable = T
    function = 1000-25*x
  []
[]

[Materials]
  [Diffusivity]
    type = GenericConstantMaterial
    prop_names = 'D'
    prop_values = '1'
  []
  [Solubility]
    # Will use Arrhenius equation, now const for test
    type = ParsedMaterial
    f_name = solubility_solid
    function = 50
    outputs = 'exodus'
  []
  [Qstar]
    type = GenericConstantMaterial
    prop_values = '1'
    prop_names = 'Qstar'
  []
[]

[Executioner]
  type = Transient
  num_steps = 200
  dt = 1
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
  end_time = 100
  [TimeStepper]
    type = IterationAdaptiveDT
    dt = 0.1
  []
[]

[Debug]
  show_var_residual_norms = true
[]

[VectorPostprocessors]
  inactive = 'C_p Integeral_Ln_different_sum'
  [C_d]
    type = LineValueSampler
    num_points = 50
    end_point = '2 0 0'
    variable = 'C_d'
    sort_by = x
    start_point = '0 0 0'
  []
  [C_p]
    type = LineValueSampler
    num_points = 50
    end_point = '2 0 0'
    variable = 'C_p'
    sort_by = x
    start_point = '0 0 0'
  []
  [Integeral_Ln_different_sum]
    type = VectorOfPostprocessors
    postprocessors = 'sum_S_dissolve sum_S_precipitate Total_Ln_sum'
    outputs = 'CenterlineFinalValue'
  []
[]

[Postprocessors]
  inactive = 'dt'
  [dt]
    type = TimestepSize
    outputs = 'exodus console'
  []
  [Avg_dissolve]
    type = ElementAverageValue
    variable = 'C_d'
  []
  [Avg_precipitate]
    type = ElementAverageValue
    variable = 'C_p'
  []
  [sum_S_dissolve]
    type = ElementIntegralVariablePostprocessor
    variable = 'C_d'
    outputs = 'exodus console'
  []
  [sum_S_precipitate]
    type = ElementIntegralVariablePostprocessor
    variable = 'C_p'
    outputs = 'exodus console'
  []
  [Total_Ln_sum]
    type = FunctionValuePostprocessor
    function = Ln_sum_all_blocks
    outputs = 'exodus console'
  []
  [Avg_sum]
    type = FunctionValuePostprocessor
    function = Sum_avg_all_blocks
  []
[]

[Functions]
  # Postprocessor Functions
  [Sum_avg_all_blocks]
    type = ParsedFunction
    vars = 'Avg_dissolve Avg_precipitate'
    value = 'Avg_dissolve+Avg_precipitate'
    vals = 'Avg_dissolve Avg_precipitate'
  []
  [Ln_sum_all_blocks]
    type = ParsedFunction
    value = 'sum_S_dissolve + sum_S_precipitate'
    vals = 'sum_S_dissolve sum_S_precipitate'
    vars = 'sum_S_dissolve sum_S_precipitate'
  []
[]

[Outputs]
  exodus = true
  inactive = 'CenterlineFinalValue'
  [CenterlineFinalValue]
    type = CSV
  []
[]
