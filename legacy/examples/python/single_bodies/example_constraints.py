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

    scenario = loco.sim.Scenario()
    floor = scenario.AddSingleBody( loco.sim.Plane( "floor", 20.0, 20.0, tm.Vector3f( [0.0, 0.0, -0.001] ), np.identity(3) ) )

    sphere = scenario.AddSingleBody( loco.sim.Sphere( "sphere", 0.1, [ 0.0, 0.0, 5.0 ], np.identity(3) ) )
    sphere.constraint = loco.sim.SingleBodyTranslational3dConstraint( "sphere_constraint" )

    block = scenario.AddSingleBody( loco.sim.Box( "block", [ 0.2, 0.2, 1.0 ], [ 0.0, 0.0, 2.0 ], tm.rotationXf( np.pi / 3.0 ) ) )
    block.constraint = loco.sim.SingleBodyRevoluteConstraint( "block_constraint",
                                                              tm.Matrix4f(),
                                                              [ 1.0, 0.0, 0.0 ] )

    door = scenario.AddSingleBody( loco.sim.Box( "door", [ 0.8, 0.2, 1.0 ], [ -0.5, 1.0, 0.4 ], tm.rotationYf( np.pi / 2.0 ) ) )
    door.constraint = loco.sim.SingleBodyRevoluteConstraint( "door_constraint",
                                                             tm.Matrix4f( np.identity(3), [ 0.0, 0.0, 0.5 ] ),
                                                             [ 1.0, 0.0, 0.0 ] )

    platform = scenario.AddSingleBody( loco.sim.Box( "platform", [ 1.0, 1.0, 0.2 ], [ 0.0, -2.0, 2.0 ], tm.rotationYf( np.pi / 3.0 ) ) )
    platform.constraint = loco.sim.SingleBodyPrismaticConstraint( "platform_constraint",
                                                                  tm.Matrix4f(), [ 0.0, 0.0, 1.0 ] )

    rod = scenario.AddSingleBody( loco.sim.Capsule( "rod", 0.1, 1.0, [ 1.0, 0.0, 1.0 ], np.identity(3) ) )
    rod.constraint = loco.sim.SingleBodySphericalConstraint( "rod_constraint",
                                                             tm.Matrix4f( np.identity(3), [ 0.0, 0.0, 0.5 ] ) )

    lego_1 = scenario.AddSingleBody( loco.sim.Box( "lego_1", [ 0.4, 0.2, 0.2 ], [ -1.0, 0.0, 0.1 ], np.identity(3) ) )
    lego_1.constraint = loco.sim.SingleBodyPlanarConstraint( "lego_1_constraint" )

    lego_2 = scenario.AddSingleBody( loco.sim.Box( "lego_2", [ 0.4, 0.2, 0.2 ], [ -0.8, 0.0, 0.301 ], np.identity(3) ) )
    lego_2.constraint = loco.sim.SingleBodyPlanarConstraint( "lego_2_constraint" )

    lego_3 = scenario.AddSingleBody( loco.sim.Box( "lego_3", [ 0.4, 0.2, 0.2 ], [ -1.2, 0.0, 0.301 ], np.identity(3) ) )
    lego_3.constraint = loco.sim.SingleBodyPlanarConstraint( "lego_3_constraint" )

    lego_4 = scenario.AddSingleBody( loco.sim.Box( "lego_4", [ 0.4, 0.2, 0.2 ], [ -1.0, 0.0, 0.501 ], np.identity(3) ) )
    lego_4.constraint = loco.sim.SingleBodyPlanarConstraint( "lego_4_constraint" )

    runtime = loco.sim.Runtime( PHYSICS_BACKEND, RENDERING_BACKEND )
    simulation = runtime.CreateSimulation( scenario )
    visualizer = runtime.CreateVisualizer( scenario )

    floor = scenario.GetSingleBodyByName( "floor" )
    floor.drawable.texture = 'built_in_chessboard'
    floor.drawable.ambient = [ 0.3, 0.5, 0.7 ]
    floor.drawable.diffuse = [ 0.3, 0.5, 0.7 ]
    floor.drawable.specular = [ 0.3, 0.5, 0.7 ]

    while visualizer.IsActive() :
        if visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_ESCAPE ) :
            break
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_R ) :
            simulation.Reset()
        elif visualizer.CheckSingleKeyPress( loco.sim.Keys.KEY_P ) :
            simulation.Pause() if simulation.running else simulation.Resume()
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

        simulation.Step()
        visualizer.Update()

    runtime.DestroySimulation()
    runtime.DestroyVisualizer()