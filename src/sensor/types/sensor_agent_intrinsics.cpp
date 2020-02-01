
#include <sensor/types/sensor_agent_intrinsics.h>

namespace tysoc {

        TAgentIntrinsicsSensor::TAgentIntrinsicsSensor( const std::string& name,
                                                        TAgent* agentPtr )
            : TISensor( name )
        {
            m_type                      = eSensorType::PROP_JOINT;
            m_agentPtr                  = agentPtr;
            m_sensorMeasurement         = new TAgentIntrinsicsSensorMeasurement();
            m_sensorMeasurement->type   = eSensorType::PROP_JOINT;

            // Take an initial measurement
            update();
        }

        TAgentIntrinsicsSensor::~TAgentIntrinsicsSensor()
        {
            TYSOC_CORE_TRACE( "Destroying(AgentIntrinsics) sensor: ", m_name );

            m_agentPtr = NULL;

            if ( m_sensorMeasurement )
            {  
                delete m_sensorMeasurement;
                m_sensorMeasurement = NULL;
            }
        }

        void TAgentIntrinsicsSensor::update()
        {
            if ( !m_agentPtr )
            {
                std::cout << "ERROR> no agentPtr given to this sensor" << std::endl;
                return;
            }

            m_sensorMeasurement->bodiesRelativePosition.clear();
            
            m_sensorMeasurement->bodiesLinVelocities.clear();
            m_sensorMeasurement->bodiesLinAccelerations.clear();
            m_sensorMeasurement->comForces.clear();
            m_sensorMeasurement->comTorques.clear();

            m_sensorMeasurement->thetas.clear();
            m_sensorMeasurement->thetadots.clear();

            auto _agentRootPosition = m_agentPtr->getPosition();

            m_sensorMeasurement->rootPosition.x = _agentRootPosition.x;
            m_sensorMeasurement->rootPosition.y = _agentRootPosition.y;
            m_sensorMeasurement->rootPosition.z = _agentRootPosition.z;

            // Compute the relative positions and velocities of the bodies/geometries to the root body
            auto _kinBodies = m_agentPtr->bodies;
            for ( size_t i = 0; i < _kinBodies.size(); i++ )
            {
                auto _bodyPosition = _kinBodies[i]->worldTransform.getPosition();

                float _dx = _bodyPosition.x - _agentRootPosition.x;
                float _dy = _bodyPosition.y - _agentRootPosition.y;
                float _dz = _bodyPosition.z - _agentRootPosition.z;

                m_sensorMeasurement->bodiesRelativePosition.push_back( _dx );
                m_sensorMeasurement->bodiesRelativePosition.push_back( _dy );
                m_sensorMeasurement->bodiesRelativePosition.push_back( _dz );
            }

            // @TODO|@CHECK: There is a slight issue here, because the order of the joint sensors and body sensosrs ...
            // can be changed if someone messes up with the xml template for the agent model. Perhaps could add an ordering ...
            // to the sensors (or even joints), or traverse them in order of the bodies and joints that they belong to
            // So far this is implicit, as they are created and stored once in the order of the joints and bodies come by
            // Check the "_constructDefaultSensors" method in the agent_kintree.cpp file.

            // get sensor readings and wrap them into the measurement
            auto _kinSensors = m_agentPtr->sensors;
            for ( size_t i = 0; i < _kinSensors.size(); i++ )
            {
                if ( _kinSensors[i]->data.type == eSensorType::PROP_JOINT )
                {
                    auto _kinJointSensor = reinterpret_cast< TKinTreeJointSensor* >( _kinSensors[i] );

                    m_sensorMeasurement->thetas.push_back( _kinJointSensor->theta );
                    m_sensorMeasurement->thetadots.push_back( _kinJointSensor->thetadot );
                }
                else if ( _kinSensors[i]->data.type == eSensorType::PROP_BODY )
                {
                    auto _kinBodySensor = reinterpret_cast< TKinTreeBodySensor* >( _kinSensors[i] );

                    m_sensorMeasurement->bodiesLinVelocities.push_back( _kinBodySensor->linVelocity );
                    m_sensorMeasurement->bodiesLinAccelerations.push_back( _kinBodySensor->linAcceleration );

                    m_sensorMeasurement->comForces.push_back( _kinBodySensor->comForce );
                    m_sensorMeasurement->comTorques.push_back( _kinBodySensor->comTorque );
                }
            }

            // @DEBUG: print the contents of the sensor
            // print();
        }

        void TAgentIntrinsicsSensor::print()
        {
            std::cout << "*******************************************" << std::endl;
            std::cout << "---------------" << std::endl;
            std::cout << "sensor name: " << m_name << std::endl;
            std::cout << "sensor type: " << tysoc::toString( m_type ) << std::endl;
            std::cout << "---------------" << std::endl;
            std::cout << "sensor data: relative positions to COM" << std::endl;

            for ( size_t i = 0; i < m_sensorMeasurement->bodiesRelativePosition.size() / 3; i++ )
            {
                std::cout << "x: " << m_sensorMeasurement->bodiesRelativePosition[3 * i + 0] << " , "
                          << "y: " << m_sensorMeasurement->bodiesRelativePosition[3 * i + 1] << " , "
                          << "z: " << m_sensorMeasurement->bodiesRelativePosition[3 * i + 2] << std::endl;
            }

            std::cout << "---------------" << std::endl;
            std::cout << "sensor data: joint angles and velocities" << std::endl;

            for ( size_t i = 0; i < m_sensorMeasurement->thetas.size(); i++ )
            {
                std::cout << "theta(" << i << "): " << m_sensorMeasurement->thetas[i] << std::endl;
                std::cout << "thetadot(" << i << "): " << m_sensorMeasurement->thetadots[i] << std::endl;
            }

            std::cout << "---------------" << std::endl;
            std::cout << "sensor data: bodies linear velocities and accelerations" << std::endl;

            for ( size_t i = 0; i < m_sensorMeasurement->bodiesLinVelocities.size(); i++ )
            {
                std::cout << "linvel(" << i << "): " << tysoc::toString( m_sensorMeasurement->bodiesLinVelocities[i] ) << std::endl;
                std::cout << "linacc(" << i << "): " << tysoc::toString( m_sensorMeasurement->bodiesLinAccelerations[i] ) << std::endl;
            }

            std::cout << "*******************************************" << std::endl;
        }

        // @CHECK: Seems that we area always returning the same, and the ...
        // object already is polymorphic, so perhaps put this in the base class
        TISensorMeasurement* TAgentIntrinsicsSensor::getSensorMeasurement()
        {
            return m_sensorMeasurement;
        }

}