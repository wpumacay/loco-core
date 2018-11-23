
#include <sensor/types/sensor_agent_intrinsics.h>


namespace tysocsensor
{

        TAgentIntrinsicsSensor::TAgentIntrinsicsSensor( const std::string& name,
                                                        tysocagent::TAgent* agentPtr )
            : TSensor( name, agentPtr, NULL )
        {
            m_sensorMeasurement = new TAgentIntrinsicsSensorMeasurement();
            m_sensorMeasurement->type = "AgentIntrinsicsMeasurement";
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
            m_sensorMeasurement->bodiesRelativePosition.clear();
            // m_sensorMeasurement->bodiesVelocities.clear();

            m_sensorMeasurement->thetas.clear();
            m_sensorMeasurement->thetadots.clear();

            auto _rootGeomName = std::string( "mjcgeom_" ) + m_agentPtr->name() + std::string( "_tmjcroot" );
            auto _agentRoot = m_agentPtr->getGeom( _rootGeomName );

            m_sensorMeasurement->rootPosition.x = _agentRoot->pos.x;
            m_sensorMeasurement->rootPosition.y = _agentRoot->pos.y;
            m_sensorMeasurement->rootPosition.z = _agentRoot->pos.z;

            // Compute the relative positions and velocities of the bodies/geometries to the root body/geometry
            // @CHECK: We are using the geometries instead, because of a weird issue I still can't solve.
            //         I will try to solve this a bit later, so for now, we use the geometries.
            auto _geometries = m_agentPtr->geometriesBuffer();
            for ( size_t i = 0; i < _geometries.size(); i++ )
            {
                float _dx = _geometries[i]->pos.x - _agentRoot->pos.x;
                float _dy = _geometries[i]->pos.y - _agentRoot->pos.y;
                float _dz = _geometries[i]->pos.z - _agentRoot->pos.z;

                m_sensorMeasurement->bodiesRelativePosition.push_back( _dx );
                m_sensorMeasurement->bodiesRelativePosition.push_back( _dy );
                m_sensorMeasurement->bodiesRelativePosition.push_back( _dz );
            }
        }

        // @CHECK: Seems that we area always returning the same, and the ...
        // object already is polymorphic, so perhaps put this in the base class
        TSensorMeasurement* TAgentIntrinsicsSensor::getSensorMeasurement()
        {
            return m_sensorMeasurement;
        }





}