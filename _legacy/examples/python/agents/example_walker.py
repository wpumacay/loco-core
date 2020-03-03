
import numpy as np
import pytysoc

_walker = pytysoc.createAgentFromModel( name = 'walker',
                                        position = [ 0.0, 0.0, 3.0 ],
                                        orientation = np.identity( 3 ),
                                        format = 'mjcf',
                                        model = 'ant' )

_runtime = pytysoc.createRuntime( physicsBackend = pytysoc.BACKENDS.PHYSICS.MUJOCO,
                                  renderingBackend = pytysoc.BACKENDS.RENDERING.GLVIZ )

_simulation = _runtime.createSimulation()
_visualizer = _runtime.createVisualizer()

_simulation.addAgent( _walker )

_walker_torso = _walker.getBody( 'b:torso' )
_walker_ankle = _walker.getJoint( 'j:ankle' )

_walker_ankle.setQpos( np.pi / 10. )
_walker_ankle.setQvel( 0.0 )

_walker_ankle_actuator = _walker.getActuator( 'a:ankle_controller' )
_walker_ankle_actuator.setKp( 50.0 )
_walker_ankle_actuator.setKv( 20.0 )

_walker_knee_actuator = _walker.getActuator( 'a:knee_controller' )
_walker_knee_actuator.deactivate() # simulate that this joint stopped working

while _visualizer.isActive() :

    if _walker_torso.inContact( pytysoc.flags.TERRAIN ) :
        break

    _walker.setActions( np.random.rand( _walker.nu() ) )

    _simulation.step()
    _visualizer.render()