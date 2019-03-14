
#pragma once

#include <tysoc_common.h>
#include <sandbox/joint.h>

namespace tysoc {
namespace sandbox {

    class TIJoint;

    class TIBody
    {

        protected :

        std::string m_name;
        std::string m_type;
        std::string m_filename;

        TVec3 m_position;
        TMat3 m_rotation;
        TVec3 m_size;

        TScalar m_mass;
        TMat3 m_inertia;
        TVec3 m_friction;

        std::vector< TIJoint* > m_joints;

        virtual void _updateInternal() = 0;
        virtual void _changePosition() = 0;
        virtual void _changeRotation() = 0;
        virtual void _changeSize() = 0;

        public :

        TIBody( const std::string& name,
                const std::string& type,
                const TVec3& position,
                const TMat3& rotation,
                const TVec3& size,
                const std::string& filename = "" );
        virtual ~TIBody();

        void setPosition( const TVec3& position );
        void setRotMat( const TMat3& rotmat );
        void setRotEuler( const TVec3& euler );
        void setRotQuat( const TVec4& quat );
        void setSize( const TVec3& size );

        TVec3 position();
        TMat3 rotMat();
        TVec3 rotEuler();
        TVec4 rotQuat();
        TVec3 size();

        std::string type();
        std::string name();
        std::string filename();

        TScalar mass();
        TMat3 inertia();
        TVec3 friction();

        void update();

        void addJoint( TIJoint* jointPtr );

        std::vector< TIJoint* > joints();
    };



}}