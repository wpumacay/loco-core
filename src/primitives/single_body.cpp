
#include <primitives/single_body.h>

namespace tysoc {

    TSingleBody::TSingleBody( const std::string& name,
                              const TBodyData& data,
                              const TVec3& position,
                              const TMat3& rotation )
        : TIBody( name, data )
    {
        /* set current and initial position and orientation directly (world-space) */
        m_pos = m_pos0 = position;
        m_rot = m_rot0 = rotation;

        /* set world-transform directly (world-space) */
        m_tf.setPosition( m_pos );
        m_tf.setRotation( m_rot );

        /* set initial world-transform directly (world-space) */
        m_tf0.setPosition( m_pos0 );
        m_tf0.setRotation( m_rot0 );

        /* set local pose (and friends) to default values */
        m_localPos = m_localPos0 = tysoc::TVec3(); // zeros
        m_localRot = m_localRot0 = tysoc::TMat3(); // identity
        m_localTf = m_localTf0 = tysoc::TMat4(); // identity
    }

    TSingleBody::~TSingleBody()
    {
        // nothing extra to delete for now
    }

    void TSingleBody::setLocalPosition( const TVec3& localPosition )
    {
        // do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::setLocalRotation( const TMat3& localRotation )
    {
        // do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::setLocalEuler( const TVec3& localEuler )
    {
        // do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::setLocalQuaternion( const TVec4& localQuat )
    {
        // do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

    void TSingleBody::setLocalTransform( const TMat4& localTransform )
    {
        // do nothing, as not applicable for single-bodies (they're not part of a chain|compound)
    }

}