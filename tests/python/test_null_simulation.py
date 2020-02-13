#!/usr/bin/env python

import loco
import tinymath as tm
import numpy as np
import gc

def test_null_simulation_functionality() :
    vis_data = loco.sim.VisualData()
    vis_data.type = loco.sim.ShapeType.CAPSULE
    vis_data.size = [ 0.1, 0.2, 0.1 ]
    col_data = loco.sim.CollisionData()
    col_data.type = loco.sim.ShapeType.CAPSULE
    col_data.size = [ 0.1, 0.2, 0.1 ]

    body_data = loco.sim.BodyData()
    body_data.dyntype = loco.sim.DynamicsType.DYNAMIC
    body_data.collision = col_data
    body_data.visual = vis_data

    body_obj = loco.sim.SingleBody( 'body_0', body_data, [ 1.0, 1.0, 1.0 ], np.identity( 3 ) )
    scenario = loco.sim.Scenario()
    scenario.AddSingleBody( body_obj )

    simulation = loco.sim.NullSimulation( scenario )
    simulation.Initialize()
    simulation.Step()
    simulation.Reset()
    simulation.Pause()
    simulation.Resume()
    assert ( len( gc.get_referrers( scenario ) ) == 1 )
    assert ( simulation.backendId == 'null' )
    del simulation
    del scenario

if __name__ == '__main__' :
    _ = input( 'Press ENTER to start test : test_null_simulation_functionality' )
    test_null_simulation_functionality()

    _ = input( 'Press ENTER to continue ...' )