
import os
import pytysoc
import tysoc_bindings

_humanoid   = tysoc_bindings.PyCoreAgent( 'humanoid1', [-2,-2,1], 'mjcf', 'humanoid' )
_walker     = tysoc_bindings.PyCoreAgent( 'walker1', [0,-2,1], 'mjcf', 'walker' )
_baxter     = tysoc_bindings.PyCoreAgent( 'baxter1', [2,-2,1], 'mjcf', 'baxter' )

_laikago    = tysoc_bindings.PyCoreAgent( 'laikago1', [-1,0,1], 'urdf', 'laikago' )
_doublePend = tysoc_bindings.PyCoreAgent( 'doublePend1', [1,0,1], 'urdf', 'double_pendulum' )

_dog        = tysoc_bindings.PyCoreAgent( 'dog1', [-3,2,1], 'rlsim', 'dog3d' )
_raptor     = tysoc_bindings.PyCoreAgent( 'raptor1', [-1,2,1], 'rlsim', 'raptor3d' )
_goat       = tysoc_bindings.PyCoreAgent( 'goat1', [1,2,1], 'rlsim', 'goat3d' )
_biped      = tysoc_bindings.PyCoreAgent( 'biped1', [3,2,1], 'rlsim', 'biped3d' )

_scenario = tysoc_bindings.PyScenario()
_scenario.addAgent( _humanoid )
_scenario.addAgent( _walker )
_scenario.addAgent( _baxter )
_scenario.addAgent( _laikago )
_scenario.addAgent( _doublePend )
_scenario.addAgent( _dog )
_scenario.addAgent( _raptor )
_scenario.addAgent( _goat )
_scenario.addAgent( _biped )

# can choose between GLVIZ and MJCVIZ rendering backends, as long as MUJOCO is ...
# enabled as physics backend (need some internals from mujoco to be intialized)
_runtime = pytysoc.createRuntime( physicsBackend = pytysoc.BACKENDS.PHYSICS.MUJOCO,
                                  renderingBackend = pytysoc.BACKENDS.RENDERING.GLVIZ )

_simulation = _runtime.createSimulation( _scenario )
_simulation.initialize()

_visualizer = _runtime.createVisualizer( _scenario )
_visualizer.initialize()

while _visualizer.isActive() :

    # for MJCVIZ run step before render (first step initializes some data ...
    # in mujoco, it seems). Otherwise, you would get a mjr_render error related to the frustum
    _simulation.step()
    _visualizer.render()