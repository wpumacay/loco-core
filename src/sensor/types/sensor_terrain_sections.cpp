
#include <sensor/types/sensor_terrain_sections.h>


namespace tysoc {
namespace sensor {

    TSectionsTerrainSensor::TSectionsTerrainSensor( const std::string& name,
                                                    terrain::TSectionsTerrainGenerator* terrainGenPtr,
                                                    agent::TIAgent* agentPtr,
                                                    bool useComplement )
        : TISensor( name )
    {
        m_type          = SENSOR_TYPE_EXTRINSICS_TERRAIN;
        m_agentPtr      = agentPtr;
        m_terrainGenPtr = terrainGenPtr;
        m_useComplement = useComplement;

        m_sensorMeasurement = new TSectionsTerrainSensorMeasurement();
        m_sensorMeasurement->type = "PathTerrainMeasurement";
        m_sensorMeasurement->usesComplement = m_useComplement;

        // Take an initial measurement
        update();
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
        // @CHECK: We were using the geom because with the body we had weird behaviour. Still ...
        // we should use the body, as the geoms may be offset according to the bodies

        auto _agentRootPosition = m_agentPtr->getPosition();
        auto _currentX = _agentRootPosition.x;
        auto _currentY = _agentRootPosition.y;
        auto _currentZ = _agentRootPosition.z;

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

        // print();
    }

    void TSectionsTerrainSensor::print()
    {
        std::cout << "*******************************************" << std::endl;
        std::cout << "---------------" << std::endl;
        std::cout << "sensor name: " << m_name << std::endl;
        std::cout << "sensor type: " << m_type << std::endl;
        std::cout << "---------------" << std::endl;

        for ( size_t i = 0; i < m_sensorMeasurement->profile.size() / 3; i++ )
        {
            std::cout << "x: " << m_sensorMeasurement->profile[3 * i + 0] << " - "
                      << "y: " << m_sensorMeasurement->profile[3 * i + 1] << " - "
                      << "z: " << m_sensorMeasurement->profile[3 * i + 2] << std::endl;
        }

        std::cout << "*******************************************" << std::endl;
    }

    TISensorMeasurement* TSectionsTerrainSensor::getSensorMeasurement()
    {
        return m_sensorMeasurement;
    }

}}