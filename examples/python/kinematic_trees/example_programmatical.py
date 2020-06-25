#!/usr/bin/env python

import sys
import loco
import tinymath as tm
import numpy as np

PHYSICS_BACKEND = loco.sim.PHYSICS_NONE
RENDERING_BACKEND = loco.sim.RENDERING_GLVIZ_GLFW

class DoublePendulum(object):

    def __init__(self, name='double_pendulum', pos=[0.0, 0.0, 2.0], l1=0.5, l2=0.5):
        super(DoublePendulum, self).__init__()

        self.kintree = loco.sim.kintree.KinematicTree(name, pos, tm.Matrix3f())
        self.base = loco.sim.kintree.Box(name + '_base', [0.1, 0.1, 0.1])
        self.link_1 = loco.sim.kintree.Capsule(name + '_link_1', 0.05, l1)
        self.link_2 = loco.sim.kintree.Capsule(name + '_link_2', 0.04, l2)

        jnt_1_axis = [1.0, 0.0, 0.0]
        jnt_2_axis = [1.0, 0.0, 0.0]
        self.jnt_base = loco.sim.kintree.FixedJoint(name + '_jnt_fixed')
        self.jnt_1 = loco.sim.kintree.RevoluteJoint(name + '_jnt_rev_1', jnt_1_axis, [1.0, -1.0])
        self.jnt_2 = loco.sim.kintree.RevoluteJoint(name + '_jnt_rev_2', jnt_2_axis, [1.0, -1.0])

        tf_link_1_to_base = tm.Matrix4f(np.identity(3), [0.0, 0.0, -0.5 * l1])
        tf_link_2_to_link_1 = tm.Matrix4f(np.identity(3), [0.0, 0.0, -0.5 * l1 - 0.5 * l2] )
        tf_jnt_1_to_link_1 = tm.Matrix4f(np.identity(3), [0.0, 0.0, 0.5 * l1])
        tf_jnt_2_to_link_2 = tm.Matrix4f(np.identity(3), [0.0, 0.0, 0.5 * l2])

        self.kintree.SetRoot(self.base)
        self.base.AddJoint(self.jnt_base, np.identity(4))
        self.base.AddChild(self.link_1, tf_link_1_to_base)
        self.link_1.AddJoint(self.jnt_1, tf_jnt_1_to_link_1)
        self.link_1.AddChild(self.link_2, tf_link_2_to_link_1)
        self.link_2.AddJoint(self.jnt_2, tf_jnt_2_to_link_2)

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
    floor = scenario.AddSingleBody( loco.sim.primitives.Plane( "floor", 10.0, 10.0, tm.Vector3f(), tm.Matrix3f() ) )
    double_pendulum = DoublePendulum()
    scenario.AddKinematicTree( double_pendulum.kintree )

    runtime = loco.sim.Runtime( PHYSICS_BACKEND, RENDERING_BACKEND )
    simulation = runtime.CreateSimulation( scenario )
    visualizer = runtime.CreateVisualizer( scenario )

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

        double_pendulum.jnt_1.angle = double_pendulum.jnt_1.angle + 0.01
        double_pendulum.jnt_2.angle = double_pendulum.jnt_2.angle + 0.005

        simulation.Step( 1. / 60. )
        visualizer.Render()

    runtime.DestroySimulation()
    runtime.DestroyVisualizer()