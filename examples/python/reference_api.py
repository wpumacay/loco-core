
import numpy as np
import pytysoc

if __name__ == '__main__' :

    # ... (mass and inertia are defaults)

    box = pytysoc.createSingleBody( "box_0", pytysoc.Shape.BOX, size = [ 0.2, 0.2, 0.2 ] )
    sphere = pytysoc.createSingleBody( "sphere_0", pytysoc.Shape.SPHERE, radius = 0.1 )
    monkey = pytysoc.createSingleBody( "mesh_0", pytysoc.Shape.MESH, path = pytysoc.Resources.Meshes.MONKEY, scale = [ 0.1, 0.1, 0.1 ] )

    # ...

    ### (addbody with no joint gives Fixed joint, just uses tf)
    table = pytysoc.createCompound( "table_0" )
    table_base, _ = table.addBody( "base", pytysoc.Shape.BOX, size = [ 1.0, 1.5, 0.05 ], mass = 0.1 )
    table_leg1, _ = table_base.addBody( "leg1", pytysoc.Shape.BOX, size = [ 0.1, 0.1, 0.75 ], mass = 0.1, tf = ... )
    table_leg2, _ = table_base.addBody( "leg2", pytysoc.Shape.BOX, size = [ 0.1, 0.1, 0.75 ], mass = 0.1, tf = ... )
    table_leg3, _ = table_base.addBody( "leg3", pytysoc.Shape.BOX, size = [ 0.1, 0.1, 0.75 ], mass = 0.1, tf = ... )
    table_leg4, _ = table_base.addBody( "leg4", pytysoc.Shape.BOX, size = [ 0.1, 0.1, 0.75 ], mass = 0.1, tf = ... )

    ### (addBody with joint expects both body-info and joint-info)
    door = pytysoc.createCompound( "door_0" )
    door_support, _ = door.addBody( "support", pytysoc.Shape.CYLINDER, radius = 0.1, height = 0.8 )
    door_block, door_joint_pivot = door_support.addBody( "block", pytysoc.Shape.BOX, size = ..., mass = ..., tf = ..., joint_type = pytysoc.Joints.REVOLUTE, joint_tf = ... )
    door_joint_pivot.setAngle( np.pi / 6 )

    # ...

    ### (when created, the minimal sensors are added: intrinsics)
    jaco = pytysoc.createAgentFromFile( "jaco_0", pytysoc.Agents.Format.MJCF, pytysoc.Resources.Mjcf.JACO )
    anymal = pytysoc.createAgentFromFile( "anymal_0", pytysoc.Agents.Format.URDF, pytysoc.Resources.Urdf.ANYMAL )

    # ...

    jaco_controller = pytysoc.createManualController( pytysoc.Actuators.TORQUE )
    anymal_controller = pytysoc.createManualController( pytysoc.Actuators.PD_CONTROLLER )

    # ...

    ### We might not require this if a default scenario is created (like a default tf-graph)
    scenario = pytysoc.createScenario()
    scenario.addSingleBody( box )
    scenario.addSingleBody( sphere )
    scenario.addSingleBody( monkey )
    scenario.addCompound( table )
    scenario.addCompound( door )
    scenario.addAgent( jaco )
    scenario.addAgent( anymal )

    # ...

    sim, viz = pytysoc.createRuntime( scenario, pytysoc.Backends.Physics.MUJOCO, pytysoc.Backends.Graphics.GLVIZ )

    while viz.active() :
        sim.step()
        viz.render()