
#include <pytysoc_compound_body.h>

namespace py = pybind11;

namespace pytysoc
{

    PyCompoundBody::PyCompoundBody( const std::string& name,
                                    const PyBodyData& data,
                                    const PyJointData& jointData,
                                    PyCompoundBody* parentRef,
                                    py::array_t<TScalar>& localXyz,
                                    py::array_t<TScalar>& localRpy )
        : PyBody( name, data )
    {
        TYSOC_CORE_ASSERT( parentRef->ptr(), "PyCompoundBody >>> parent body-wrapper has no valid internal TIBody" );

        /* instantiate wrapper body object */
        m_body = new tysoc::TCompoundBody( name, 
                                           toTBodyData( data ), 
                                           toTJointData( jointData ),
                                           dynamic_cast< tysoc::TCompoundBody* >( parentRef->ptr() ),
                                           numpyToVec3( localXyz ),
                                           tysoc::TMat3::fromEuler( numpyToVec3( localRpy ) ) );

        /* store the reference to the parent's wrapper */
        m_pyParentRef = parentRef;

        /* wrap alredy-existing collider object */
        auto _collisionRef = m_body->collision();
        if ( _collisionRef )
        {
            m_pyCollision = new PyCollision( _collisionRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyCollision->setParentBody( this );
        }

        /* wrap alredy-existing drawable object */
        auto _visualRef = m_body->visual();
        if ( _visualRef )
        {
            m_pyVisual = new PyVisual( _visualRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyVisual->setParentBody( this );
        }

        /* wrap alredy-existing joint object */
        auto _jointRef = dynamic_cast< tysoc::TCompoundBody* >( m_body )->joint();
        if ( _jointRef )
        {
            m_pyJoint = new PyJoint( _jointRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyJoint->setOwner( this );
            m_pyJoint->setOwnerParent( m_pyParentRef );
        }
    }


    PyCompoundBody::PyCompoundBody( const std::string& name,
                                    const PyBodyData& data,
                                    const PyJointData& jointData,
                                    py::array_t<TScalar>& xyz,
                                    py::array_t<TScalar>& rpy )
        : PyBody( name, data )
    {
        /* instantiate wrapper body object */
        m_body = new tysoc::TCompoundBody( name, 
                                           toTBodyData( data ), 
                                           toTJointData( jointData ),
                                           numpyToVec3( xyz ),
                                           tysoc::TMat3::fromEuler( numpyToVec3( rpy ) ) );

        /* this object has no parent */
        m_pyParentRef = nullptr;

        /* wrap alredy-existing collider object */
        auto _collisionRef = m_body->collision();
        if ( _collisionRef )
        {
            m_pyCollision = new PyCollision( _collisionRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyCollision->setParentBody( this );
        }

        /* wrap alredy-existing drawable object */
        auto _visualRef = m_body->visual();
        if ( _visualRef )
        {
            m_pyVisual = new PyVisual( _visualRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyVisual->setParentBody( this );
        }

        /* wrap alredy-existing joint object */
        auto _jointRef = dynamic_cast< tysoc::TCompoundBody* >( m_body )->joint();
        if ( _jointRef )
        {
            m_pyJoint = new PyJoint( _jointRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyJoint->setOwner( this );
            m_pyJoint->setOwnerParent( m_pyParentRef );
        }
    }

    PyCompoundBody::PyCompoundBody( const std::string& name,
                                    const PyBodyData& data,
                                    py::array_t<TScalar>& xyz,
                                    py::array_t<TScalar>& rpy,
                                    const tysoc::eDynamicsType& dyntype )
        : PyBody( name, data )
    {
        /* instantiate wrapper body object */
        m_body = new tysoc::TCompoundBody( name, 
                                           toTBodyData( data ), 
                                           numpyToVec3( xyz ),
                                           tysoc::TMat3::fromEuler( numpyToVec3( rpy ) ),
                                           dyntype );

        /* this object has no parent */
        m_pyParentRef = nullptr;

        /* wrap alredy-existing collider object */
        auto _collisionRef = m_body->collision();
        if ( _collisionRef )
        {
            m_pyCollision = new PyCollision( _collisionRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyCollision->setParentBody( this );
        }

        /* wrap alredy-existing drawable object */
        auto _visualRef = m_body->visual();
        if ( _visualRef )
        {
            m_pyVisual = new PyVisual( _visualRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyVisual->setParentBody( this );
        }

        /* wrap alredy-existing joint object */
        auto _jointRef = dynamic_cast< tysoc::TCompoundBody* >( m_body )->joint();
        if ( _jointRef )
        {
            m_pyJoint = new PyJoint( _jointRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyJoint->setOwner( this );
            m_pyJoint->setOwnerParent( m_pyParentRef );
        }
    }

    PyCompoundBody::~PyCompoundBody()
    {
        m_pyJoint = nullptr;
        m_pyParentRef = nullptr;
    }

}