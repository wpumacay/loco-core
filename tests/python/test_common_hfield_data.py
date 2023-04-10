import pytest

import numpy as np

import loco


def test_hfield_attributes() -> None:
    assert hasattr(loco.HeightfieldData, "n_width_samples")
    assert hasattr(loco.HeightfieldData, "n_depth_samples")
    assert hasattr(loco.HeightfieldData, "heights")


class TestHeightfield:
    def test_default_constructor(self) -> None:
        hfield_data = loco.HeightfieldData()
        assert hfield_data.n_width_samples == 0
        assert hfield_data.n_depth_samples == 0
        assert type(
            hfield_data.heights
        ) == np.ndarray and hfield_data.heights.shape == (0,)

    def test_hfield_grid_size(self) -> None:
        hfield_data = loco.HeightfieldData()
        hfield_data.n_width_samples = 10
        hfield_data.n_depth_samples = 10
        assert hfield_data.heights.shape == (10, 10)
        assert np.allclose(
            hfield_data.heights, np.zeros((10, 10)).astype(np.float32)
        )

    def test_hfield_heights(self) -> None:
        hfield_data = loco.HeightfieldData()
        array_np = np.random.randn(10, 10).astype(np.float32)
        hfield_data.heights = array_np
        assert hfield_data.n_width_samples == 10
        assert hfield_data.n_depth_samples == 10
        assert hfield_data.heights.shape == (10, 10)
        assert np.allclose(hfield_data.heights, array_np)
