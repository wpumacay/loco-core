
#pragma once

#include <sensor/sensor_base.h>

namespace tysoc {
namespace sensor {

    struct TSectionsTerrainSensorMeasurement : public TSensorMeasurement
    {
        struct { float x; float y; float z; } agentPosition;
        struct { float x; float y; float z; } agentProjection;
        std::vector< float > profile;
        bool usesComplement;
    };

    class TSectionsTerrainSensor : public TSensor
    {
        private :

        TSectionsTerrainSensorMeasurement* m_sensorMeasurement;

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

        // set this flag to set either format 1 or 2 :
        //   format 1 ( default ) : heights with base the projection into the terrain
        //   format 2 : heights with base the position of the agent
        bool m_useComplement;

        agent::TAgent*                          m_agentPtr;
        terrain::TSectionsTerrainGenerator*     m_terrainGenPtr;

        public :

        TSectionsTerrainSensor( const std::string& name,
                                terrain::TSectionsTerrainGenerator* terrainGenPtr,
                                agent::TAgent* agentPtr,
                                bool useComplement = false );
        ~TSectionsTerrainSensor();

        void update() override;
        TSensorMeasurement* getSensorMeasurement() override;
    };

}}