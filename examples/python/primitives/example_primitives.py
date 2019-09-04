
import numpy as np
import pytysoc

# 1-a) General way of creating a primitive ######################################

_cubeShape = pytysoc.Shape( pytysoc.Shape.BOX,
                            size = [0.1, 0.1, 0.1],
                            localPos = np.zeros( 3 ),
                            localRot = np.identity( 3 ),
                            color = [0.7, 0.5, 0.3] )

_cubeCollision = pytysoc.Collision( _cubeShape )
_cubeVisual = pytysoc.Visual( _cubeShape )

_cubeBody = pytysoc.Body( "cuby" )
_cubeBody.addCollision( _cubeCollision )
_cubeBody.addVisual( _cubeVisual )

## # 1-b) Custom|Fast way of creating a primitive ##################################
## _cubeBody = pytysoc.createBody( type = pytysoc.Shape.BOX,
##                                 size = [0.1, 0.1, 0.1] )

# 2-a-1) Create a scenario to add the primitive #################################
_scenario = pytysoc.Scenario()
_scenario.addBody( _cubeBody )

# 2-a-2) Create a runtime, simulation, and visulaizer ###########################
_runtime = pytysoc.createRuntime( physicsBackend = pytysoc.BACKENDS.PHYSICS.MUJOCO,
                                  renderingBackend = pytysoc.BACKENDS.RENDERING.GLVIZ )
_simulation = _runtime.createSimulation( _scenario )
_visualizer = _runtime.createVisualizer( _scenario )

## # 2-b-1) Create a runtime, simulation, and visualizer directly ##################
## _runtime = pytysoc.createRuntime( physicsBackend = pytysoc.BACKENDS.PHYSICS.MUJOCO,
##                                   renderingBackend = pytysoc.BACKENDS.RENDERING.GLVIZ )
## _simulation = _runtime.createSimulation() # creates a default empty scenario if none given
## _visualizer = _runtime.createVisualizer() # creates a default empty scenario if none given
## 
## # 2-b-2) Add the body to the simulation and visualizer ##########################
## _simulation.addBody( _cubeBody )
## _visualizer.addBody( _cubeBody )

# 3) initialize and enjoy
_simulation.initialize()
_visualizer.initialize()

while _visualizer.isActive() :
    _simulation.step()
    _visualizer.render()