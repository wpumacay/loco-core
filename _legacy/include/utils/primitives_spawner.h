
#pragma once

#include <map>
#include <queue>
#include <vector>
#include <iostream>
#include <string>

#define PRIMITIVES_SPAWNER_POOL_SIZE 30
#define PRIMITIVES_DEFAULT_LIFETIME 15 // 15 seconds

#define BOX_INDX        0
#define SPHERE_INDX     1
#define CAPSULE_INDX    2
#define CYLINDER_INDX   3

namespace tysoc {
namespace utils {


    struct TDebugPrimitive
    {
        std::string                             type;
        struct { float x; float y; float z; }   size;
        struct { float x; float y; float z; }   restPosition;// place here when recycled
        struct { float x; float y; float z; }   position;
        float                                   rotmat[9];
        struct { float vx; float y; float z; }  velocity;
        struct { float r; float g; float b; }   color;
        bool                                    inUse;
        float                                   lifetime;
    };

    class TPrimitivesSpawner
    {

        protected :

        std::map< std::string, int > m_keyMapping;

        std::vector< TDebugPrimitive* >    m_primitives[4];
        std::queue< TDebugPrimitive* >     m_queueAvailablePrimitives[4];
        std::queue< TDebugPrimitive* >     m_queueWorkingPrimitives[4];

        virtual TDebugPrimitive* _createPrimitiveInternal( const std::string& type,
                                                           float sx, float sy, float sz,
                                                           float x, float y, float z ) = 0;
        virtual void _recyclePrimitiveInternal( TDebugPrimitive* primitivePtr ) = 0;
        virtual void _activatePrimitiveInternal( TDebugPrimitive* primitivePtr,
                                                 float sx, float sy, float sz,
                                                 float x, float y, float z ) = 0;
        virtual void _updatePrimitiveInternal( TDebugPrimitive* primitivePtr ) = 0;
        
        TDebugPrimitive* _createPrimitive( const std::string& type,
                                           float sx, float sy, float sz,
                                           float x, float y, float z );
        void _recyclePrimitive( TDebugPrimitive* primitivePtr );
        void _activatePrimitive( TDebugPrimitive* primitivePtr,
                                 float sx, float sy, float sz,
                                 float x, float y, float z );

        public :

        TPrimitivesSpawner();
        ~TPrimitivesSpawner();

        void initialize();
        void update( float dt );
        void spawnObject( const std::string& type,
                          float sx, float sy, float sz, 
                          float x, float y, float z );

        std::vector< TDebugPrimitive* > getPrimitives( const std::string& type );
    };



}}