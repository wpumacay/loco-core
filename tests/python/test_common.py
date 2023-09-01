import math3d as m3d
import numpy as np

import loco


def test_constants() -> None:
    assert loco.MAX_NUM_QPOS == 7
    assert loco.MAX_NUM_QVEL == 6
    assert loco.NUM_QPOS_JOINT_PRISMATIC == 1
    assert loco.NUM_QPOS_JOINT_REVOLUTE == 1
    assert loco.NUM_QPOS_JOINT_SPHERICAL == 4
    assert loco.NUM_QPOS_JOINT_FREE == 7
    assert np.abs(loco.DEFAULT_DENSITY - 1e3) < 1e-5


def test_shape_data() -> None:
    shape_data = loco.ShapeData()
    assert shape_data.type == loco.ShapeType.SPHERE
    # Size element is exposed as a math3d::Vector3X
    assert shape_data.size is m3d.Vector3f or shape_data.size is m3d.Vector3d
    assert np.abs(shape_data.size.x - 0.0) < 1e-6
    assert np.abs(shape_data.size.y - 0.0) < 1e-6
    assert np.abs(shape_data.size.z - 0.0) < 1e-6
    assert np.allclose(
        shape_data.size, np.array([0.0, 0.0, 0.0], dtype=np.float32)
    )
    # Relative pose is exposed as a math3d::Pose3d_X
    assert (
        shape_data.local_tf is m3d.Pose3d_f
        or shape_data.local_tf is m3d.Pose3d_d
    )
    assert np.abs(shape_data.local_tf.position.x - 0.0) < 1e-6
    assert np.abs(shape_data.local_tf.position.y - 0.0) < 1e-6
    assert np.abs(shape_data.local_tf.position.z - 0.0) < 1e-6
    assert np.allclose(
        shape_data.local_tf.position,
        np.array([0.0, 0.0, 0.0], dtype=np.float32),
    )
    assert np.abs(shape_data.local_tf.orientation.w - 1.0) < 1e-6
    assert np.abs(shape_data.local_tf.orientation.x - 0.0) < 1e-6
    assert np.abs(shape_data.local_tf.orientation.y - 0.0) < 1e-6
    assert np.abs(shape_data.local_tf.orientation.z - 0.0) < 1e-6
    assert np.allclose(
        shape_data.local_tf.orientation,
        np.array([1.0, 0.0, 0.0, 0.0], dtype=np.float32),
    )
    # MeshData and HeightfieldData are both empty by default
    assert shape_data.mesh_data.filepath == ""
    assert shape_data.mesh_data.n_vertices == 0
    assert shape_data.mesh_data.vertices.shape == (0,)
    assert shape_data.mesh_data.n_faces == 0
    assert shape_data.mesh_data.faces.shape == (0,)
    assert shape_data.hfield_data.n_width_samples == 0
    assert shape_data.hfield_data.n_depth_samples == 0
    assert shape_data.hfield_data.heights.shape == (0,)


def test_collider_data() -> None:
    collider_data = loco.ColliderData()
    assert collider_data.collision_group == 1
    assert collider_data.collision_mask == 1
    assert np.allclose(
        collider_data.friction, np.array([1.0, 0.005, 0.0001], dtype=np.float32)
    )
    assert len(collider_data.children) == 0
    # Make sure it keeps the same defaults as its parent
    assert collider_data.type == loco.ShapeType.SPHERE
    assert np.allclose(
        collider_data.size, np.array([0.0, 0.0, 0.0], dtype=np.float32)
    )
    assert collider_data.mesh_data.filepath == ""
    assert collider_data.mesh_data.n_vertices == 0
    assert collider_data.mesh_data.vertices.shape == (0,)
    assert collider_data.mesh_data.n_faces == 0
    assert collider_data.mesh_data.faces.shape == (0,)
    assert collider_data.hfield_data.n_width_samples == 0
    assert collider_data.hfield_data.n_depth_samples == 0
    assert collider_data.hfield_data.heights.shape == (0,)
    assert np.allclose(
        collider_data.local_tf.position,
        np.array([0.0, 0.0, 0.0], dtype=np.float32),
    )
    assert np.allclose(
        collider_data.local_tf.orientation,
        np.array([1.0, 0.0, 0.0, 0.0], dtype=np.float32),
    )


def test_drawable_data() -> None:
    drawable_data = loco.DrawableData()
    assert np.allclose(
        drawable_data.color, np.array([0.7, 0.5, 0.3], dtype=np.float32)
    )
    assert drawable_data.texture == ""
    assert len(drawable_data.children) == 0
    # Make sure it keeps the same defaults as its parent
    assert drawable_data.type == loco.ShapeType.SPHERE
    assert np.allclose(
        drawable_data.size, np.array([0.0, 0.0, 0.0], dtype=np.float32)
    )
    assert drawable_data.mesh_data.filepath == ""
    assert drawable_data.mesh_data.n_vertices == 0
    assert drawable_data.mesh_data.vertices.shape == (0,)
    assert drawable_data.mesh_data.n_faces == 0
    assert drawable_data.mesh_data.faces.shape == (0,)
    assert drawable_data.hfield_data.n_width_samples == 0
    assert drawable_data.hfield_data.n_depth_samples == 0
    assert drawable_data.hfield_data.heights.shape == (0,)
    assert np.allclose(
        drawable_data.local_tf.position,
        np.array([0.0, 0.0, 0.0], dtype=np.float32),
    )
    assert np.allclose(
        drawable_data.local_tf.orientation,
        np.array([1.0, 0.0, 0.0, 0.0], dtype=np.float32),
    )


def test_inertial_data() -> None:
    inertial_data = loco.InertialData()
    assert np.abs(inertial_data.mass - 1.0) < 1e-5
    assert np.allclose(
        inertial_data.inertia, np.zeros((3, 3), dtype=np.float32)
    )
    assert np.allclose(
        inertial_data.local_tf.position,
        np.array([0.0, 0.0, 0.0], dtype=np.float32),
    )
    assert np.allclose(
        inertial_data.local_tf.orientation,
        np.array([1.0, 0.0, 0.0, 0.0], dtype=np.float32),
    )


def test_body_data() -> None:
    body_data = loco.BodyData()
    assert body_data.dyntype == loco.DynamicsType.DYNAMIC
    assert type(body_data.inertia) is loco.InertialData
    assert type(body_data.collider) is loco.ColliderData
    assert type(body_data.drawable) is loco.DrawableData
