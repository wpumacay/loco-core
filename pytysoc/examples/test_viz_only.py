
import os
import pytysoc
import tysoc_bindings

_humanoid   = tysoc_bindings.PyCoreAgent( 'humanoid1', [-2,-2,1.5], 'mjcf', 'humanoid' )
_walker     = tysoc_bindings.PyCoreAgent( 'walker1', [0,-2,1.5], 'mjcf', 'walker' )
_baxter     = tysoc_bindings.PyCoreAgent( 'baxter1', [2,-2,1.5], 'mjcf', 'baxter' )

_laikago    = tysoc_bindings.PyCoreAgent( 'laikago1', [0,0,1.5], 'urdf', 'laikago' )

_dog        = tysoc_bindings.PyCoreAgent( 'dog1', [-3,2,1.5], 'rlsim', 'dog3d' )
_raptor     = tysoc_bindings.PyCoreAgent( 'raptor1', [-1,2,1.5], 'rlsim', 'raptor3d' )
_goat       = tysoc_bindings.PyCoreAgent( 'goat1', [1,2,1.5], 'rlsim', 'goat3d' )
_biped      = tysoc_bindings.PyCoreAgent( 'biped1', [3,2,1.5], 'rlsim', 'biped3d' )

_terrainGen = tysoc_bindings.PyStaticTerrainGen( 'terrainGen0' ) 
_terrainGen.createPrimitive( 'plane', [10,10,0.1], [0,0,0], [0,0,0], [.2,.3,.4], 'chessboard' )

_scenario = tysoc_bindings.PyScenario()
_scenario.addAgent( _humanoid )
_scenario.addAgent( _walker )
_scenario.addAgent( _baxter )
_scenario.addAgent( _laikago )
_scenario.addAgent( _dog )
_scenario.addAgent( _raptor )
_scenario.addAgent( _goat )
_scenario.addAgent( _biped )
_scenario.addTerrainGen( _terrainGen )

# without a physics backend, only use GLVIZ for now. To use MJCVIZ see the ...
# test_simulation.py example in this same folder, as it initializes MUJOCO as ...
# physics backend and initializes some internals (mjData* and mjModel) which ...
# are needed for the visualizer as well :() . Will check later to initialze this ...
# structures if no simulation is given, which should work Ok.
_runtime = pytysoc.createRuntime( renderingBackend = pytysoc.BACKENDS.RENDERING.GLVIZ,
                                  workingDir = pytysoc.PATHS.WORKING_DIR )

_visualizer = _runtime.createVisualizer( _scenario )
_visualizer.initialize()

while _visualizer.isActive() :

    _visualizer.render()