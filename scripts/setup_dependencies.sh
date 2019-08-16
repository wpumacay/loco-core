#!/bin/sh

# --------------------------------------------------------------------- #
# script/setup: Sets up this repo by cloning all required dependencies  #
# --------------------------------------------------------------------- #

echo "======== Installing apt-dependencies ============"

echo "==> Installing build tools"
sudo apt-get install make cmake pkg-config

echo "==> Installing dependencies for renderer"
sudo apt-get install libassimp-dev libglfw3-dev libglew-dev

echo "======== Cloning dependencies -> ext/ ==========="

# rendering engine for visualizer
echo "==> Cloning wpumacay/cat1 @ github - master branch"
git clone --depth=1 https://github.com/wpumacay/cat1.git ext/cat1

# using own imgui version to add some extra cmake-files
echo "==> Cloning wpumacay/imgui @ github - master branch"
git clone --depth=1 https://github.com/wpumacay/imgui.git ext/imgui

# used for generating pytysoc|python bindings
echo "==> Cloning pybind/pybind11 @ github - master branch"
git clone --depth=1 https://github.com/pybind/pybind11.git ext/pybind11


