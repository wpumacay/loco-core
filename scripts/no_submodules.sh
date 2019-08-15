#!/bin/sh

# --------------------------------------------------------------------- #
# script/no_submodules: Removes any submodules from this repo           #
# --------------------------------------------------------------------- #

for submod in pybind11 ext/cat1 ext/imgui
do
    echo "Deleting: ${submod}"
    mv "${submod}" "${submod}_tmp"
    git submodule deinit -f -- "${submod}"
    rm -rf ".git/modules/${submod}"
    git rm -f "${submod}"
done