
#include <sensor/types/sensor_terrain_sections.h>


namespace tysoc {
namespace sensor {

    TSectionsTerrainSensor::TSectionsTerrainSensor( const std::string& name,
                                                    terrain::TSectionsTerrainGenerator* terrainGenPtr,
                                                    agent::TAgent* agentPtr,
                                                    bool useComplement )
        : TSensor( name )
    {
        m_agentPtr = agentPtr;
        m_terrainGenPtr = terrainGenPtr;
        m_useComplement = useComplement;

        m_sensorMeasurement = new TSectionsTerrainSensorMeasurement();
        m_sensorMeasurement->type = "PathTerrainMeasurement";
        m_sensorMeasurement->usesComplement = m_useComplement;
    }

    TSectionsTerrainSensor::~TSectionsTerrainSensor()
    {
        m_terrainGenPtr = NULL;
        m_agentPtr = NULL;

        if ( m_sensorMeasurement )
        {
            delete m_sensorMeasurement;
            m_sensorMeasurement = NULL;
        }
    }

    void TSectionsTerrainSensor::update()
    {
        // @CHECK: We are using the geom because with the body we had weird behaviour. Still ...
        // we should use the body, as the geoms may be offset according to the bodies

        // Use the profiler from the terrain generator
        //auto _rootBodyName = std::string( "mjcbody_" ) + m_agentPtr->name() + std::string( "_tmjcroot" );
        //auto _agentRoot = m_agentPtr->getBody( _rootBodyName );
        auto _rootGeomName  = std::string( "mjcgeom_" ) + m_agentPtr->name() + std::string( "_tmjcroot" );
        auto _agentRoot = m_agentPtr->getGeom( _rootGeomName );
        auto _currentX = _agentRoot->pos.x;
        auto _currentY = _agentRoot->pos.y;
        auto _currentZ = _agentRoot->pos.z;

        auto _agentSampleZ = m_terrainGenPtr->getProfile1D( _currentX, _currentY );

        // sample from the terrain profiler using some spacing
        auto _maxpos = _currentX + 5.0f;
        auto _minpos = _currentX - 5.0f;
        auto _nsamples = 100;

        m_sensorMeasurement->agentPosition.x = _currentX;
        m_sensorMeasurement->agentPosition.y = _currentY;
        m_sensorMeasurement->agentPosition.z = _currentZ;

        m_sensorMeasurement->agentProjection.x = _currentX;
        m_sensorMeasurement->agentProjection.y = _currentY;
        m_sensorMeasurement->agentProjection.z = _agentSampleZ;

        m_sensorMeasurement->profile.clear();

        for ( size_t i = 0; i < _nsamples; i++ )
        {
            auto _sampleX = _minpos + ( ( _maxpos - _minpos ) * ( i ) ) / _nsamples;
            auto _sampleY = _currentY;
            auto _sampleZ = m_terrainGenPtr->getProfile1D( _sampleX, _sampleY );
            // transform to format appropiate format :
            if ( !m_useComplement )
            {
                // FORMAT 1
                /*
                *      (agent-root)
                *            X - - - - - - - - - - - - - -
                *            .                        
                *            .                          
                *            .               ______________
                *            .              /. . . . . . . 
                *            .             / . . . . . . . 
                *            .            /. . . . . . . .   measurement (format 1)
                *          __.___________/_._._._._._._._. ___________________________ projection as base
                *              |
                *             
                *              |  profile
                *            
                *         _ _  | _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ Z = 0
                */
                _sampleZ = _sampleZ - _agentSampleZ;
            }
            else
            {
                // FORMAT 2
                /*
                *      (agent-root)
                *            X - - - - - - - - - - - - - - - - - - - - - - - - - - agent z as base
                *            .   .    .  .  .   .    .
                *            .   .    .     .   .    .  measurement (format 2)
                *            .   .    .  .  .______________
                *            .   .    .  .  /
                *            .   .    .  . /
                *            .   .    .  ./
                *          __.___.____.__/
                *              |
                *             
                *              |  profile
                *            
                *         _ _  | _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ Z = 0
                */

                _sampleZ = _currentZ - _sampleZ;
            }

            m_sensorMeasurement->profile.push_back( _sampleX );
            m_sensorMeasurement->profile.push_back( _sampleY /*- 0.505f * m_terrainGenPtr->getSectionDepth()*/ );
            m_sensorMeasurement->profile.push_back( _sampleZ );
        }

        // std::cout << "finished measurements" << std::endl;
    }

    TSensorMeasurement* TSectionsTerrainSensor::getSensorMeasurement()
    {
        return m_sensorMeasurement;
    }

}}