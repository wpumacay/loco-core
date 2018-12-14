
#include <sensor/types/sensor_agent_intrinsics.h>


namespace tysoc {
namespace sensor {

        TAgentIntrinsicsSensor::TAgentIntrinsicsSensor( const std::string& name,
                                                        agent::TIAgent* agentPtr )
            : TISensor( name )
        {
            m_type                      = SENSOR_TYPE_INTRINSICS;
            m_agentPtr                  = agentPtr;
            m_sensorMeasurement         = new TAgentIntrinsicsSensorMeasurement();
            m_sensorMeasurement->type   = "AgentIntrinsicsMeasurement";

            // Take an initial measurement
            update();
        }

        TAgentIntrinsicsSensor::~TAgentIntrinsicsSensor()
        {
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

            if ( m_agentPtr->type() != agent::AGENT_TYPE_KINTREE )
            {
                std::cout << "ERROR> agentPtr should be of type AGENT_TYPE_KINTREE" << std::endl;
                return;
            }

            // @CHECK: make this not dependent of the engine
            // why did I this in the first place?, perhaps I was in a hurry

            m_sensorMeasurement->bodiesRelativePosition.clear();
            // m_sensorMeasurement->bodiesVelocities.clear();

            m_sensorMeasurement->thetas.clear();
            m_sensorMeasurement->thetadots.clear();

            auto _agentRootPosition = m_agentPtr->getPosition();

            m_sensorMeasurement->rootPosition.x = _agentRootPosition.x;
            m_sensorMeasurement->rootPosition.y = _agentRootPosition.y;
            m_sensorMeasurement->rootPosition.z = _agentRootPosition.z;

            // Compute the relative positions and velocities of the bodies/geometries to the root body/geometry
            // @CHECK: We are using the geometries instead, because of a weird issue I still can't solve.
            //         I will try to solve this a bit later, so for now, we use the geometries.
            auto _kinBodies = reinterpret_cast< agent::TAgentKinTree* >( m_agentPtr )->getKinTreeBodies();
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
        }

        // @CHECK: Seems that we area always returning the same, and the ...
        // object already is polymorphic, so perhaps put this in the base class
        TISensorMeasurement* TAgentIntrinsicsSensor::getSensorMeasurement()
        {
            return m_sensorMeasurement;
        }

}}