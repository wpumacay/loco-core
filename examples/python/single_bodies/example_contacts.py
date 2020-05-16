#!/usr/bin/env python

import sys
import loco
import tinymath as tm
import numpy as np

PHYSICS_BACKEND = loco.sim.PHYSICS_NONE
RENDERING_BACKEND = loco.sim.RENDERING_GLVIZ_GLFW

if __name__ == '__main__' :

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
    print( 'Physics backend: {}'.format( PHYSICS_BACKEND ) )
    print( 'Rendering backend: {}'.format( RENDERING_BACKEND ) )

    rotation = tm.rotation( tm.Vector3f( [ np.pi / 3, np.pi / 4, np.pi / 6 ] ) )
    #### rotation = tm.rotation( tm.Vector3f( [ np.pi / 2, 0.0, 0.0 ] ) )
    #### rotation = tm.rotation( tm.Vector3f( [ 0.0, 0.0, 0.0 ] ) )

    scenario = loco.sim.Scenario()
    floor = scenario.AddSingleBody( loco.sim.Plane( "floor", 10.0, 10.0, np.zeros(3), np.identity(3) ) )
    sphere = scenario.AddSingleBody( loco.sim.Sphere( "sphere", 0.1, [ 1.0, -1.0, 2.0 ], np.identity(3) ) )
    goal = scenario.AddSingleBody( loco.sim.Sphere( "goal", 0.4, [0.0, 0.0, 0.4], np.identity(3) ) )
    box = scenario.AddSingleBody( loco.sim.Box( "box", [ 0.2, 0.3, 0.4 ], [ -1.0, -1.0, 2.0 ], rotation ) )

    runtime = loco.sim.Runtime( PHYSICS_BACKEND, RENDERING_BACKEND )
    simulation = runtime.CreateSimulation( scenario )
    visualizer = runtime.CreateVisualizer( scenario )

    floor.drawable.texture = 'built_in_chessboard'
    floor.drawable.ambient = [ 0.3, 0.5, 0.7 ]
    floor.drawable.diffuse = [ 0.3, 0.5, 0.7 ]
    floor.drawable.specular = [ 0.3, 0.5, 0.7 ]

    goal.drawable.texture = 'built_in_chessboard'
    goal.drawable.ambient = [0.8, 0.1, 0.1]
    goal.drawable.diffuse = [0.8, 0.1, 0.1]
    goal.drawable.specular = [0.8, 0.1, 0.1]

    while visualizer.IsActive() :
        if visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_ESCAPE ) :
            break
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_R ) :
            simulation.Reset()
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_P ) :
            simulation.Pause() if simulation.running else simulation.Resume()
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_L ) :
            for single_body in scenario.GetSingleBodiesList() :
                single_body.linear_vel = [ 0.0, 0.0, 5.0 ]
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_A ) :
            for single_body in scenario.GetSingleBodiesList() :
                single_body.angular_vel = [ 0.0, 0.0, np.pi ]
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_SPACE ) :
            sphere.AddForceCOM( [ 0.0, 0.0, 1000.0 ] )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_UP ) :
            sphere.AddForceCOM( [ 0.0, 200.0, 0.0 ] )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_DOWN ) :
            sphere.AddForceCOM( [ 0.0, -200.0, 0.0 ] )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_RIGHT ) :
            sphere.AddForceCOM( [ 200.0, 0.0, 0.0 ] )
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_LEFT ) :
            sphere.AddForceCOM( [ -200.0, 0.0, 0.0 ] )

        simulation.Step( 1. / 60. )
        visualizer.Render()

        for contact in sphere.collider.contacts :
            if goal.collider.name == contact.name :
                print( 'Reached goal, restarting demo' )
                simulation.Reset()
            elif box.collider.name in contact.name :
                print( 'Reached box, restarting only the position of the sphere' )
                sphere.pos = [1.0, -1.0, 2.0]

    runtime.DestroySimulation()
    runtime.DestroyVisualizer()