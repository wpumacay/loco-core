#!/usr/bin/env python

import sys
import loco
import tinymath as tm
import numpy as np

def create_body( name, shape, size, dyntype,
                 position, euler, color = [0.7, 0.5, 0.3] ) :
    col_data = loco.sim.CollisionData()
    col_data.type = shape
    col_data.size = size
    vis_data = loco.sim.VisualData()
    vis_data.type = shape
    vis_data.size = size
    vis_data.ambient = color
    vis_data.diffuse = color
    vis_data.specular = color
    body_data = loco.sim.BodyData()
    body_data.collision = col_data
    body_data.visual = vis_data
    body_data.dyntype = dyntype

    body_obj = loco.sim.SingleBody( name, body_data, position, tm.rotation( tm.Vector3f( euler ) ) )
    return body_obj

if __name__ == '__main__' :
    PHYSICS_BACKEND = loco.sim.PHYSICS_NONE
    if len( sys.argv ) > 1 :
        choice_backend = sys.argv[1]
        if choice_backend == 'mujoco' :
            PHYSICS_BACKEND = loco.sim.PHYSICS_MUJOCO
        elif choice_backend == 'bullet' :
            PHYSICS_BACKEND = loco.sim.PHYSICS_BULLET
        elif choice_backend == 'dart' :
            PHYSICS_BACKEND = loco.sim.PHYSICS_DART
        elif choice_backend == 'raisim' :
            PHYSICS_BACKEND = loco.sim.PHYSICS_RAISIM
    print( 'Backend: {}'.format( PHYSICS_BACKEND ) )

    orientation = [ np.pi / 3, np.pi / 4, np.pi / 6 ]
    #### orientation = [ np.pi / 2, 0.0, 0.0 ]
    #### orientation = [ 0.0, 0.0, 0.0 ]

    scenario = loco.sim.Scenario()
    scenario.AddSingleBody( create_body( 'floor', loco.sim.ShapeType.PLANE, [ 10.0, 10.0, 1.0 ],
                                          loco.sim.DynamicsType.STATIC, [ 0.0, 0.0, 0.0 ], [ 0.0, 0.0, 0.0 ], [ 0.3, 0.5, 0.7 ] ) )
    scenario.AddSingleBody( create_body( 'box', loco.sim.ShapeType.BOX, [ 0.2, 0.3, 0.4 ],
                                          loco.sim.DynamicsType.DYNAMIC, [ -1.0, -1.0, 2.0 ], orientation ) )
    scenario.AddSingleBody( create_body( 'sphere', loco.sim.ShapeType.SPHERE, [ 0.1, 0.1, 0.1 ],
                                          loco.sim.DynamicsType.DYNAMIC, [ 1.0, -1.0, 2.0 ], orientation ) )
    scenario.AddSingleBody( create_body( 'cylinder', loco.sim.ShapeType.CYLINDER, [ 0.2, 0.5, 1.0 ],
                                          loco.sim.DynamicsType.DYNAMIC, [ -1.0, 1.0, 2.0 ], orientation ) )
    scenario.AddSingleBody( create_body( 'capsule', loco.sim.ShapeType.CAPSULE, [ 0.2, 0.5, 1.0 ],
                                          loco.sim.DynamicsType.DYNAMIC, [ 1.0, 1.0, 2.0 ], orientation ) )
    scenario.AddSingleBody( create_body( 'ellipsoid', loco.sim.ShapeType.ELLIPSOID, [ 0.2, 0.3, 0.4 ],
                                          loco.sim.DynamicsType.DYNAMIC, [ 0.0, 0.0, 2.0 ], orientation ) )

    runtime = loco.sim.Runtime( PHYSICS_BACKEND, loco.sim.RENDERING_GLVIZ_GLFW )
    simulation = runtime.CreateSimulation( scenario )
    visualizer = runtime.CreateVisualizer( scenario )

    simulation.Initialize()
    visualizer.Initialize()

    while visualizer.IsActive() :
        if visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_ESCAPE ) :
            break
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_R ) :
            simulation.Reset()

        simulation.Step()
        visualizer.Update()

    runtime.DestroySimulation()
    runtime.DestroyVisualizer()