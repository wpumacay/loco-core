#pragma once

#include <tysoc_common.h>

namespace tysoc {

    const std::string ACTUATOR_TORQUE = "torque_actuator";
    const std::string ACTUATOR_POSITION = "position_actuator";
    const std::string ACTUATOR_VELOCITY = "velocity_actuator";

    /**
    *   Abstract actuator interface for SISO controllers
    *   
    *   This is the base class for all actuator types supported by our implementation. These 
    *   controllers are SISO (adopting the actuators-usage of MuJoCo as standard) in the sense 
    *   that these accept only Single Inputs as control command, and give a Single Output as 
    *   force at the transmission (joints for now).
    *
    *   These actuators can have internal dynamics, which can be handled by the specific backend (if
    *   they support the required actuation models) or by us in the appropriate backend wrappers (if
    *   only basic control mechanisms are provided)
    *
    *   Similar to other components, these are abstract and available to the user to interact with,
    *   but the heavy lifting is done by the wrappers themselves, by grabbing the information from
    *   this resource and using it in the backend in a bidirectional way (pass info from and to the 
    *   backend).
    */
    class TActuator
    {
        protected :

        /* For book keeping and default-name generation */
        static int m_numActuators;

        /* Unique name identifier of this actuator in the whole simulation */
        std::string m_name;

        /* Type of the actuator, which could be either "torque", "position" and "velocity" */
        std::string m_type;

        public :

        /* Control value to be applied to this actuator */
        TScalar ctrl;

        /* Range of possible values for ctrl input */
        TVec2 rangeCtrl;

        TActuator( const std::string& name,
                   const std::string& type );

        std::string name() { return m_name; }
        std::string type() { return m_type; }

    };

    /**
    *   Abstract actuator for SISO direct torque controllers
    */
    class TTorqueActuator : public TActuator
    {
        public :

        /* Control scaler for torque transmission */
        TScalar gear;

        /* Constructs a direct torque actuator */
        TTorqueActuator( const std::string& name,
                         const std::string& type ) 
            : TActuator( name, type ) {}
    };


    /**
    *   
    */
    class TPoositionActuator
    {

    };


    class TVelocityActuator
    {

    };




}