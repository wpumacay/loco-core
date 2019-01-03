# tysocCore: Core functionality for my robot locomotion framework

This is the core functionality used for a robot locomotion framework I'm making
as part of my Msc. thesis. This is physics-backend agnostic, and the concrete
implementations are found in some other repositories (mujoco in [this](https://github.com/wpumacay/tysocMjc)
repository, bullet and physX are on the way).

## Objective

The main goal of this project is to serve as a framework for DeepRL research in robot 
locomotion. It's inspired in [dm_control](https://github.com/deepmind/dm_control), but 
I followed a different approach. Most of the benchmarks available use bindings like 
mujoco-py or direct bindings from ctypes to allow the creation of simulated environments using 
mujoco as physics backend, and start building from that python interface. This project instead
abstracts all required functionality into a core C++ library (this repo) and then it "instantiates"
(makes use of) a specific backend as needed (like mujoco, bullet or physX). Then the core
functionality is exposed to the user through a python API via bindings, which is agnostic of the
backend used.

In short: **you should be able to swap between physics backends as you please.**

I'm working on the core functionality, which should include :

* **A terrain generation API** : allows you to create terrains procedurally via config params, .json config files, or programatically.
* **A sensors API** : allows you to create and query various kind of virtual sensors, like agent intrinsic information (joint angles, velocities, etc.) or extrinsic sensors (heightmaps, camera readings, etc.)
* **An agent API** : allows you to create an agent, from various formats (mjcf, urdf, json-terrainrlsim, etc.) and interact with it via control actions.

## Features

I'm currently working on this library and will be on heavy development for the next 8 months. 
The features will be updated in the corresponding dependency repos, and updated here as well. 
The updates will be documented in the [UPDATES.md](https://github.com/wpumacay/tysocMjc/blob/master/doc/UPDATES.md) file.

I will be adding more documentation as I develop the library, and sorry in advance as 
I might forget to update the docs from time to time. However, one main objective is 
to write comprehensive documentation, and I will be doing it on the go. If you have any 
suggestions/issues, just post an issue or contact me at wpumacay@gmail.com .

## Roadmap

The current roadmap and features being in my todolist can be found in [this](https://trello.com/b/pLkqprwU) trello board.