
#pragma once

#include <pytysoc_common.h>
#include <pytysoc_tests.h>
#include <pytysoc_data.h>
#include <pytysoc_collision.h>
#include <pytysoc_visual.h>
#include <pytysoc_joint.h>
#include <pytysoc_body.h>
#include <pytysoc_single_body.h>
#include <pytysoc_compound_body.h>
#include <pytysoc_compound.h>
#include <pytysoc_agent_core.h>
#include <pytysoc_sensors.h>
#include <pytysoc_terrainGen.h>
#include <pytysoc_scenario.h>
#include <pytysoc_runtime.h>

// @todo: REMOVE usage of double wrapping, as we're currently creating wrappers for c++ resources,
//        but pybind11 already wraps the classes we make as wrappers (double wrapping). Instead, 
//        just wrap directly, as in the following example:
//
//  current:
//          /* define some wrapping class */
//          class PyBody{}
//          /* define wrappers using pybind for this """wrapping""" class */
//          py::class_<pytysoc::PyBody>(m,"PyBody") \...
//
//  desired:
//          /* just wrap directly the tysoc::TIBody object */
//          py::class_<tysoc::TIBody>(m,"TIBody") \...
//
// for this to work, we need to expose every element to pybind, and this means EVERY elements on
// the c++ side: enums (e.g. eJointType), structs (e.g. TJointData), math-objects (e.g. TMat4), 
// core tysoc objects (e.g. TIBody, TCompound), etc.
//
// @note:
//  :( :( :( T_T, I've already look more in depth in pybind11, and how others use it, and I'm definitely
//  using it in the wrong way (wrapping a wrappable, and other annoying issues :/). I'm going to rewrite
//  everything from scratch (the bindings), but to do that I'll first make sure everything is working
//  flawlessly on the c++ side, and the ownership model is the correct one (or at least not so tightly
//  coupled to the design that it can't be changed) as I'm designing it to make sure ownership belongs
//  to the scenario, but I'll have to consider the case in which pybind won't play nicely with unique_ptr
//  usage. There we might have to change everything to shared_ptr usage.

namespace pytysoc
{
    void initializeMjcfSchemaPath( const std::string& schemaPath );

    void initializeLoader( const std::string& pathTemplatesMjcf,
                           const std::string& pathTemplatesUrdf,
                           const std::string& pathTemplatesRlsim );
}