
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
        LOCO_CORE_ASSERT( parentRef->ptr(), "PyCompoundBody >>> parent body-wrapper has no valid internal TIBody" );

        /* instantiate wrapper body object */
        m_bodyRef = new tysoc::TCompoundBody( name, 
                                           toTBodyData( data ), 
                                           toTJointData( jointData ),
                                           dynamic_cast< tysoc::TCompoundBody* >( parentRef->ptr() ),
                                           numpyToVec3( localXyz ),
                                           tysoc::TMat3::fromEuler( numpyToVec3( localRpy ) ) );

        /* store the reference to the parent's wrapper */
        m_pyParentRef = parentRef;

        /* wrap alredy-existing collider object */
        auto _collisionRef = m_bodyRef->collision();
        if ( _collisionRef )
        {
            m_pyCollision = new PyCollision( _collisionRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyCollision->setParentBody( this );
        }

        /* wrap alredy-existing drawable object */
        auto _visualRef = m_bodyRef->visual();
        if ( _visualRef )
        {
            m_pyVisual = new PyVisual( _visualRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyVisual->setParentBody( this );
        }

        /* wrap alredy-existing joint object */
        auto _jointRef = dynamic_cast< tysoc::TCompoundBody* >( m_bodyRef )->joint();
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
        m_bodyRef = new tysoc::TCompoundBody( name, 
                                           toTBodyData( data ), 
                                           toTJointData( jointData ),
                                           numpyToVec3( xyz ),
                                           tysoc::TMat3::fromEuler( numpyToVec3( rpy ) ) );

        /* this object has no parent */
        m_pyParentRef = nullptr;

        /* wrap alredy-existing collider object */
        auto _collisionRef = m_bodyRef->collision();
        if ( _collisionRef )
        {
            m_pyCollision = new PyCollision( _collisionRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyCollision->setParentBody( this );
        }

        /* wrap alredy-existing drawable object */
        auto _visualRef = m_bodyRef->visual();
        if ( _visualRef )
        {
            m_pyVisual = new PyVisual( _visualRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyVisual->setParentBody( this );
        }

        /* wrap alredy-existing joint object */
        auto _jointRef = dynamic_cast< tysoc::TCompoundBody* >( m_bodyRef )->joint();
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
        m_bodyRef = new tysoc::TCompoundBody( name, 
                                           toTBodyData( data ), 
                                           numpyToVec3( xyz ),
                                           tysoc::TMat3::fromEuler( numpyToVec3( rpy ) ),
                                           dyntype );

        /* this object has no parent */
        m_pyParentRef = nullptr;

        /* wrap alredy-existing collider object */
        auto _collisionRef = m_bodyRef->collision();
        if ( _collisionRef )
        {
            m_pyCollision = new PyCollision( _collisionRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyCollision->setParentBody( this );
        }

        /* wrap alredy-existing drawable object */
        auto _visualRef = m_bodyRef->visual();
        if ( _visualRef )
        {
            m_pyVisual = new PyVisual( _visualRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyVisual->setParentBody( this );
        }

        /* wrap alredy-existing joint object */
        auto _jointRef = dynamic_cast< tysoc::TCompoundBody* >( m_bodyRef )->joint();
        if ( _jointRef )
        {
            m_pyJoint = new PyJoint( _jointRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyJoint->setOwner( this );
            m_pyJoint->setOwnerParent( m_pyParentRef );
        }
    }

    PyCompoundBody::PyCompoundBody( tysoc::TCompoundBody* bodyRef )
        : PyBody( bodyRef->name(), toPyBodyData( bodyRef->data() ) )
    {
        /* keep the reference to the body */
        m_bodyRef = bodyRef;

        /* grab parent body if applicable (this will recurse until root is reached) */
        if ( bodyRef->parent() )
            m_pyParentRef = new PyCompoundBody( bodyRef->parent() );

        /* wrap already existing collider object */
        auto _collisionRef = bodyRef->collision();
        if ( _collisionRef )
        {
            m_pyCollision = new PyCollision( _collisionRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyCollision->setParentBody( this );
        }

        /* wrap alredy-existing drawable object */
        auto _visualRef = bodyRef->visual();
        if ( _visualRef )
        {
            m_pyVisual = new PyVisual( _visualRef );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyVisual->setParentBody( this );
        }

        /* wrap alredy-existing joint object */
        auto _jointRef = bodyRef->joint();
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
        m_pyCompoundRef = nullptr;
        m_pyJoint = nullptr;
        m_pyParentRef = nullptr;
        m_pyChildrenRefs.clear();
    }

    void PyCompoundBody::setCompound( PyCompound* pyCompoundRef )
    {
        assert( m_bodyRef );
        assert( pyCompoundRef );
        assert( pyCompoundRef->ptr() );

        m_pyCompoundRef = pyCompoundRef;
        dynamic_cast< tysoc::TCompoundBody* >( m_bodyRef )->setCompound( m_pyCompoundRef->ptr() );
    }

    void PyCompoundBody::setParent( PyCompoundBody* pyParentRef )
    {
        assert( m_bodyRef );
        assert( pyParentRef );
        assert( pyParentRef->ptr() );

        m_pyParentRef = pyParentRef;
        dynamic_cast< tysoc::TCompoundBody* >( m_bodyRef )->setParent( dynamic_cast< tysoc::TCompoundBody* >( m_pyParentRef->ptr() ) );
    }

    std::pair<PyCompoundBody*, PyJoint* > PyCompoundBody::addBodyJointPair( const std::string& name,
                                                                            const PyBodyData& bodyData,
                                                                            const PyJointData& jointData,
                                                                            py::array_t<TScalar>& bodyLocalTransform )
    {
        assert( m_bodyRef );

        auto _compoundBodyRef = dynamic_cast< tysoc::TCompoundBody* >( m_bodyRef );
        auto _bodyJointPair = _compoundBodyRef->addBodyJointPair( name,
                                                                  toTBodyData( bodyData ),
                                                                  toTJointData( jointData ),
                                                                  numpyToMat4( bodyLocalTransform ) );

        if ( !_bodyJointPair.first || !_bodyJointPair.second )
        {
            LOCO_CORE_ERROR( "PyCompoundBody::addBodyJointPair() >>> something went wrong while \
                               creating the requested body-joint pair on the c++ side" );
            return { nullptr, nullptr };
        }

        return { new PyCompoundBody( _bodyJointPair.first ), new PyJoint( _bodyJointPair.second ) };
    }

    std::pair<PyCompoundBody*, PyJoint* > PyCompoundBody::addBodyJointPair( const std::string& name,
                                                                            const tysoc::eShapeType& bodyShape,
                                                                            py::array_t<TScalar>& bodySize,
                                                                            py::array_t<TScalar>& bodyColor,
                                                                            py::array_t<TScalar>& bodyLocalTransform,
                                                                            const tysoc::eJointType& jointType,
                                                                            py::array_t<TScalar>& jointAxis,
                                                                            py::array_t<TScalar>& jointLimits,
                                                                            py::array_t<TScalar>& jointLocalTransform )
    {
        assert( m_bodyRef );

        auto _compoundBodyRef = dynamic_cast< tysoc::TCompoundBody* >( m_bodyRef );
        auto _bodyJointPair = _compoundBodyRef->addBodyJointPair( name,
                                                                  bodyShape,
                                                                  numpyToVec3( bodySize ),
                                                                  numpyToVec3( bodyColor ),
                                                                  numpyToMat4( bodyLocalTransform ),
                                                                  jointType,
                                                                  numpyToVec3( jointAxis ),
                                                                  numpyToVec2( jointLimits ),
                                                                  numpyToMat4( jointLocalTransform ) );

        if ( !_bodyJointPair.first || !_bodyJointPair.second )
        {
            LOCO_CORE_ERROR( "PyCompoundBody::addBodyJointPair() >>> something went wrong while \
                               creating the requested body-joint pair on the c++ side" );
            return { nullptr, nullptr };
        }

        return { new PyCompoundBody( _bodyJointPair.first ), new PyJoint( _bodyJointPair.second ) };
    }

}