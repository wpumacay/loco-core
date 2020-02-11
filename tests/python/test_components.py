#!/usr/bin/env python

import loco
import tinymath as tm
import numpy as np
import gc

def rotationMatrix_axisX( angle ) :
    mat = np.identity( 3 )
    mat[1, 1] = np.cos( angle ); mat[1 ,2] = -np.sin( angle );
    mat[2, 1] = np.sin( angle ); mat[2 ,2] = np.cos( angle );
    return mat

def createTestTransform_local() :
    rotation = rotationMatrix_axisX( np.pi / 4.0 )
    position = [ -0.0, -0.1, -0.2 ]
    return tm.Matrix4f( rotation, position )

def createTestTransform_world() :
    rotation = rotationMatrix_axisX( np.pi / 6.0 )
    position = [ 0.0, 0.0, 2.0 ]
    return tm.Matrix4f( rotation, position )

def test_collision_object() :
    col_data = loco.sim.CollisionData()
    col_data.type = loco.sim.ShapeType.BOX
    col_data.size = [ 0.1, 0.2, 0.3 ]

    col_obj = loco.sim.Collision( 'col_0', col_data )
    # Call methods (nothing should happen, as the collider is not in a simulation)
    col_obj.Initialize()
    col_obj.PreStep()
    col_obj.PostStep()
    col_obj.Reset()
    # Check that all read_write properties work (visible and wireframe props require adapters to work)
    col_obj.visible = True
    col_obj.wireframe = True
    col_obj.size = [ 0.2, 0.2, 0.2 ]
    col_obj.collisionGroup = 1
    col_obj.collisionMask = 2
    col_obj.localTf = createTestTransform_local()
    assert ( col_obj.visible == True ), 'test_collision_object >>> Couldn\t set \'visible\' property'
    assert ( col_obj.wireframe == True ), 'test_collision_object >>> Couldn\t set \'wireframe\' property'
    assert np.allclose( col_obj.size, [ 0.2, 0.2, 0.2 ] ), 'test_collision_object >>> Couldn\'t set \'size\' property'
    assert ( col_obj.collisionGroup == 1 ), 'test_collision_object >>> Couldn\'t set \'collisionGroup\' property'
    assert ( col_obj.collisionMask == 2 ), 'test_collision_object >>> Couldn\'t set \'collisionMask\' property'
    assert np.allclose( col_obj.localTf, createTestTransform_local() ), 'test_collision_object >>> Couldn\'t set \'localTf\' property'
    assert np.allclose( col_obj.localRot, rotationMatrix_axisX( np.pi / 4.0 ) ), 'test_collision_object >>> localRot getter doesn\'t return correct value'
    assert np.allclose( col_obj.localPos, [ -0.0, -0.1, -0.2 ] ), 'test_collision_object >>> localPos getter doesn\'t return correct value'
    # Finally, just print the object (handy if not using pytest)
    print( col_obj )

def test_visual_object() :
    vis_data = loco.sim.VisualData()
    vis_data.type = loco.sim.ShapeType.SPHERE
    vis_data.size = [ 0.1, 0.1, 0.1 ]

    vis_obj = loco.sim.Visual( 'vis_0', vis_data )
    # Call methods (nothing should happen, as the visual is not part of a visualizer)
    vis_obj.Initialize()
    vis_obj.PreStep()
    vis_obj.PostStep()
    vis_obj.Reset()
    # Check that all read_write properties work (visible and wireframe props )
    vis_obj.visible = True
    vis_obj.wireframe = True
    vis_obj.size = [ 0.2, 0.2, 0.2 ]
    vis_obj.ambient = [ 0.725, 0.525, 0.325 ]
    vis_obj.diffuse = [ 0.750, 0.550, 0.350 ]
    vis_obj.specular = [ 0.775, 0.575, 0.375 ]
    vis_obj.shininess = 128.0
    vis_obj.localTf = createTestTransform_local()
    assert ( vis_obj.visible == True ), 'test_visual_object >>> issue with set|get of \'visible\' property'
    assert ( vis_obj.wireframe == True ), 'test_visual_object >>> issue with set|get of \'wireframe\' property'
    assert np.allclose( vis_obj.size, [ 0.2, 0.2, 0.2 ] ), 'test_visual_object >>> issue with set|get of \'size\' property'
    assert np.allclose( vis_obj.ambient, [ 0.725, 0.525, 0.325 ] ), 'test_visual_object >>> issue with set|get of \'ambient\' property'
    assert np.allclose( vis_obj.diffuse, [ 0.750, 0.550, 0.350 ] ), 'test_visual_object >>> issue with set|get of \'diffuse\' property'
    assert np.allclose( vis_obj.specular, [ 0.775, 0.575, 0.375 ] ), 'test_visual_object >>> issue with set|get of \'specular\' property'
    assert np.allclose( vis_obj.shininess, 128.0 ), 'test_visual_object >>> issue with set|get of \'shininess\' property'
    assert np.allclose( vis_obj.localTf, createTestTransform_local() ), 'test_visual_object >>> issue with set|get of \'localTf\' property'
    assert np.allclose( vis_obj.localRot, rotationMatrix_axisX( np.pi / 4.0 ) ), 'test_visual_object >>> issue with set|get of \'localRot\' property'
    assert np.allclose( vis_obj.localPos, [ -0.0, -0.1, -0.2 ] ), 'test_visual_object >>> issue with set|get of \'localPos\' property'
    # Finally, just print the object (handy if not using pytest)
    print( vis_obj )

def test_single_body_object() :
    vis_data = loco.sim.VisualData()
    vis_data.type = loco.sim.ShapeType.CAPSULE
    vis_data.size = [ 0.1, 0.2, 0.1 ]
    vis_data.localTf = createTestTransform_local()
    col_data = loco.sim.CollisionData()
    col_data.type = loco.sim.ShapeType.CAPSULE
    col_data.size = [ 0.1, 0.2, 0.1 ]
    col_data.localTf = createTestTransform_local()

    body_data = loco.sim.BodyData()
    body_data.dyntype = loco.sim.DynamicsType.DYNAMIC
    body_data.collision = col_data
    body_data.visual = vis_data

    # @firsttodo: complete tests (ownership of visual|collision, etc.)
    body_obj = loco.sim.SingleBody( "body_0", body_data, [ 1.0, 1.0, 1.0 ], np.identity( 3 ) )
    # Call methods (nothing should happen, as the body is not in a simulation)
    body_obj.Initialize()
    body_obj.PreStep()
    body_obj.PostStep()
    body_obj.Reset()
    # Check that all read_write properties work
    body_obj.tf = createTestTransform_world()
    assert ( body_obj.dyntype == loco.sim.DynamicsType.DYNAMIC ), 'test_single_body_object >>> issue with set|get of \'dyntype\' property'
    assert ( body_obj.classType == loco.sim.BodyClassType.SINGLE_BODY ), 'test_single_body_object >>> issue with set|get of \'classType\' property'
    assert np.allclose( body_obj.tf, createTestTransform_world() ), 'test_single_body_object >>> issue with set|get of \'tf\' property'
    assert np.allclose( body_obj.rot, rotationMatrix_axisX( np.pi / 6.0 ) ), 'test_single_body_object >>> issue with set|get of \'rot\' property'
    assert np.allclose( body_obj.pos, [ 0.0, 0.0, 2.0 ] ), 'test_single_body_object >>> issue with set|get of \'pos\' property'
    vis_ref = body_obj.visual
    col_ref = body_obj.collision
    expected_tf = np.dot( body_obj.tf, createTestTransform_local() )
    assert np.allclose( vis_ref.tf, expected_tf ), 'test_single_body_object >>> expected component(visual) \'tf\' is not correct'
    assert np.allclose( vis_ref.pos, expected_tf[0:3,3] ), 'test_single_body_object >>> expected component(visual) \'rot\' is not correct'
    assert np.allclose( vis_ref.rot, expected_tf[0:3,0:3] ), 'test_single_body_object >>> expected component(visual) \'pos\' is not correct'
    assert np.allclose( col_ref.tf, expected_tf ), 'test_single_body_object >>> expected component(collision) \'tf\' is not correct'
    assert np.allclose( col_ref.pos, expected_tf[0:3,3] ), 'test_single_body_object >>> expected component(collision) \'tf\' is not correct'
    assert np.allclose( col_ref.rot, expected_tf[0:3,0:3] ), 'test_single_body_object >>> expected component(collision) \'tf\' is not correct'
    print( body_obj )
    # Check that changing visuals|collisions works fine
    vis_data_2 = loco.sim.VisualData()
    vis_data_2.type = loco.sim.ShapeType.CYLINDER
    vis_data_2.size = [ 0.1, 0.2, 0.1 ]
    col_data_2 = loco.sim.CollisionData()
    col_data_2.type = loco.sim.ShapeType.CYLINDER
    col_data_2.size = [ 0.1, 0.2, 0.1 ]

    vis_obj_2 = loco.sim.Visual( 'vis_2_replacement', vis_data_2 )
    col_obj_2 = loco.sim.Collision( 'col_2_replacement', col_data_2 )
    body_obj.visual = vis_obj_2
    body_obj.collision = col_obj_2
    assert ( body_obj.visual.name == 'vis_2_replacement' ), 'test_single_body_object >>> expected vis-replacement to have correct name'
    assert ( body_obj.collision.name == 'col_2_replacement' ), 'test_single_body_object >>> expected col-replacement to have correct name'
    # Local transforms for these visual|collision are identity, so their 'tf' should be the same as their parents'
    assert np.allclose( body_obj.visual.tf, createTestTransform_world() ), 'test_single_body_object >>> expected vis-replacement to have correct world-transform'
    assert np.allclose( body_obj.collision.tf, createTestTransform_world() ), 'test_single_body_object >>> expected col-replacement to have correct world-transform'
    print( body_obj )

if __name__ == '__main__' :
    _ = input( 'Press ENTER to start test : test_collision_object' )
    test_collision_object()

    _ = input( 'Press ENTER to start test : test_visual_object' )
    test_visual_object()

    _ = input( 'Press ENTER to start test : test_single_body_object' )
    test_single_body_object()

    _ = input( 'Press ENTER to continue ...' )