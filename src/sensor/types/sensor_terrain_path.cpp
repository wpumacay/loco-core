
#include <sensor/types/sensor_terrain_path.h>


namespace tysocsensor
{



    TPathTerrainSensor::TPathTerrainSensor( tysocterrain::TPathTerrainGenerator* terrainGenPtr,
                                            tysocagent::TAgent* agentPtr)
        : TSensor( agentPtr, terrainGenPtr )
    {
        m_sensorMeasurement = new TPathTerrainSensorMeasurement();
        m_sensorMeasurement->type = "PathTerrainMeasurement";
    }

    TPathTerrainSensor::~TPathTerrainSensor()
    {
        m_terrainGenPtr = NULL;
        m_agentPtr = NULL;

        if ( m_sensorMeasurement )
        {
            delete m_sensorMeasurement;
            m_sensorMeasurement = NULL;
        }
    }

    void TPathTerrainSensor::update()
    {
        // Use the profiler from the terrain generator
        auto _agentRootBody = m_agentPtr->getBody( "root" );
        auto _currentX = _agentRootBody->pos.x;
        auto _currentY = _agentRootBody->pos.y;
        auto _currentZ = _agentRootBody->pos.z;

        // sample from the terrain profiler using some spacing
        auto _maxpos = _currentX + 2.5f;
        auto _minpos = _currentX - 2.5f;
        auto _nsamples = 20;
        m_sensorMeasurement->profile.clear();

        for ( size_t i = 0; i < _nsamples; i++ )
        {
            auto _sampleX = _minpos + ( ( _maxpos - _minpos ) * ( i ) ) / _nsamples;
            auto _sampleY = _currentY;
            auto _sampleZ = reinterpret_cast< tysocterrain::TPathTerrainGenerator* >
                                ( m_terrainGenPtr )->getProfile1D( _sampleX );

            m_sensorMeasurement->profile.push_back( _sampleX );
            m_sensorMeasurement->profile.push_back( _sampleY );
            m_sensorMeasurement->profile.push_back( _sampleZ );
        }
    }

    TSensorMeasurement* TPathTerrainSensor::getSensorMeasurement()
    {
        return m_sensorMeasurement;
    }
}