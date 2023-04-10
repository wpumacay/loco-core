import pytest

import numpy as np

import loco


def test_mesh_attributes() -> None:
    assert hasattr(loco.MeshData, "filepath")
    assert hasattr(loco.MeshData, "n_vertices")
    assert hasattr(loco.MeshData, "vertices")
    assert hasattr(loco.MeshData, "n_faces")
    assert hasattr(loco.MeshData, "faces")

class TestMeshData:
    def test_default_constructor(self) -> None:
        mesh_data = loco.MeshData()
        assert mesh_data.filepath == ""
        assert mesh_data.n_vertices == 0
        assert mesh_data.n_faces == 0
        assert type(mesh_data.vertices) == np.ndarray and mesh_data.vertices.shape == (0,)
        assert type(mesh_data.faces) == np.ndarray and mesh_data.faces.shape == (0,)

    def test_vertices_and_faces(self) -> None:
        mesh_data = loco.MeshData()
        vertices = np.random.randn(10, 3).astype(np.float32)
        mesh_data.vertices = vertices
        assert mesh_data.n_vertices == 10
        assert mesh_data.vertices.shape == (30,)
        assert np.allclose(mesh_data.vertices, vertices.flatten())

        faces = np.random.randint(0, 10, (4, 3)).astype(np.uint32)
        mesh_data.faces = faces
        assert mesh_data.n_faces == 4
        assert mesh_data.faces.shape == (12,)
        assert np.allclose(mesh_data.faces, faces.flatten())
