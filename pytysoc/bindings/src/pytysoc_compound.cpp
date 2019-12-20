
#include <pytysoc_compound.h>

namespace py = pybind11;

namespace pytysoc
{

    PyCompound::PyCompound( const std::string& name,
                            py::array_t<TScalar>& xyz,
                            py::array_t<TScalar>& rpy,
                            const tysoc::eDynamicsType& dyntype )
    {
        /* create the internal cpp compound (keep a reference only, as ownership will belong to the cpp scenario) */
        m_compoundRef = new tysoc::TCompound( name, 
                                              numpyToVec3( xyz ), 
                                              tysoc::TMat3::fromEuler( numpyToVec3( rpy ) ), 
                                              dyntype );
        /* there's no root yet, until the user adds bodies to this compound */
        m_pyRootBodyRef = nullptr;
    }

    PyCompound::~PyCompound()
    {
        m_compoundRef = nullptr;
        m_pyRootBodyRef = nullptr;
    }

    PyCompoundBody* PyCompound::createRootBody( const std::string& name,
                                                const PyBodyData& bodyData,
                                                py::array_t<TScalar>& localTransform )
    {
        assert( m_compoundRef );

        if ( m_pyRootBodyRef )
        {
            TYSOC_CORE_WARN( "PyCompound::createRootBody() >>> compound \"{0}\" already has a root body.", m_compoundRef->name() );
            return nullptr;
        }

        auto _compoundBodyRef = m_compoundRef->createRootBody( name,
                                                               toTBodyData( bodyData ),
                                                               numpyToMat4( localTransform ) );

        if ( !_compoundBodyRef )
        {
            TYSOC_CORE_WARN( "PyCompound::createRootBody() >>> something wen't wrong while creating c++ compound-body "
                             "\"{0}\" for compound \"{1}\"", name, m_compoundRef->name() );
            return nullptr;
        }

        /* keep a reference to the newly created root */
        m_pyRootBodyRef = new PyCompoundBody( _compoundBodyRef );
        m_pyRootBodyRef->setCompound( this );
        m_pyBodiesRefs.push_back( m_pyRootBodyRef );

        return m_pyRootBodyRef;
    }

    PyCompoundBody* PyCompound::createRootBody( const std::string& name,
                                                const tysoc::eShapeType& shape,
                                                py::array_t<TScalar>& size,
                                                py::array_t<TScalar>& color,
                                                py::array_t<TScalar>& localTransform )
    {
        assert( m_compoundRef );

        if ( m_pyRootBodyRef )
        {
            TYSOC_CORE_WARN( "PyCompound::createRootBody() >>> compound \"{0}\" already has a root body.", m_compoundRef->name() );
            return nullptr;
        }

        auto _compoundBodyRef = m_compoundRef->createRootBody( name,
                                                               shape,
                                                               numpyToVec3( size ),
                                                               numpyToVec3( color ),
                                                               numpyToMat4( localTransform ) );

        if ( !_compoundBodyRef )
        {
            TYSOC_CORE_WARN( "PyCompound::createRootBody() >>> something wen't wrong while creating c++ compound-body "
                             "\"{0}\" for compound \"{1}\"", name, m_compoundRef->name() );
            return nullptr;
        }

        /* keep a reference to the newly created root */
        m_pyRootBodyRef = new PyCompoundBody( _compoundBodyRef );
        m_pyRootBodyRef->setCompound( this );
        m_pyBodiesRefs.push_back( m_pyRootBodyRef );

        return m_pyRootBodyRef;
    }

    std::pair< PyCompoundBody*, PyJoint* > PyCompound::createRootBodyJointPair( const std::string& bodyName,
                                                                                const PyBodyData& bodyData,
                                                                                const PyJointData& jointData,
                                                                                py::array_t<TScalar>& localTransform )
    {
        assert( m_compoundRef );

        if ( m_pyRootBodyRef )
        {
            TYSOC_CORE_WARN( "PyCompound::createRootBodyJointPair() >>> compound \"{0}\" already has a root body.", m_compoundRef->name() );
            return { nullptr, nullptr };
        }

        auto _bodyJointPair = m_compoundRef->createRootBodyJointPair( bodyName,
                                                                      toTBodyData( bodyData ),
                                                                      toTJointData( jointData ),
                                                                      numpyToMat4( localTransform ) );

        if ( !_bodyJointPair.first || !_bodyJointPair.second )
        {
            TYSOC_CORE_WARN( "PyCompound::createRootBodyJointPair() >>> something wen't wrong while creating c++ compound-body "
                             "\"{0}\" for compound \"{1}\"", bodyName, m_compoundRef->name() );
            return { nullptr, nullptr };
        }

        /* keep a reference to the newly created root */
        m_pyRootBodyRef = new PyCompoundBody( _bodyJointPair.first );
        m_pyRootBodyRef->setCompound( this );
        m_pyBodiesRefs.push_back( m_pyRootBodyRef );

        return { m_pyRootBodyRef, m_pyRootBodyRef->joint() };
    }

    std::pair< PyCompoundBody*, PyJoint* > PyCompound::createRootBodyJointPair( const std::string& bodyName,
                                                                                const tysoc::eShapeType& bodyShape,
                                                                                py::array_t<TScalar>& bodySize,
                                                                                py::array_t<TScalar>& bodyColor,
                                                                                py::array_t<TScalar>& bodyLocalTransform,
                                                                                const tysoc::eJointType& jointType,
                                                                                py::array_t<TScalar>& jointAxis,
                                                                                py::array_t<TScalar>& jointLimits,
                                                                                py::array_t<TScalar>& jointLocalTransform )
    {
        assert( m_compoundRef );

        if ( m_pyRootBodyRef )
        {
            TYSOC_CORE_WARN( "PyCompound::createRootBodyJointPair() >>> compound \"{0}\" already has a root body.", m_compoundRef->name() );
            return { nullptr, nullptr };
        }

        auto _bodyJointPair = m_compoundRef->createRootBodyJointPair( bodyName,
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
            TYSOC_CORE_WARN( "PyCompound::createRootBodyJointPair() >>> something wen't wrong while creating c++ compound-body "
                             "\"{0}\" for compound \"{1}\"", bodyName, m_compoundRef->name() );
            return { nullptr, nullptr };
        }

        /* keep a reference to the newly created root */
        m_pyRootBodyRef = new PyCompoundBody( _bodyJointPair.first );
        m_pyRootBodyRef->setCompound( this );
        m_pyBodiesRefs.push_back( m_pyRootBodyRef );

        return { m_pyRootBodyRef, m_pyRootBodyRef->joint() };
    }

    void PyCompound::addCompoundBody( PyCompoundBody* pyCompoundBody )
    {
        TYSOC_CORE_ASSERT( m_compoundRef, "PyCompound::addCompoundBody() >>> wrapper contains null reference to a compound" );

        m_compoundRef->addCompoundBody( std::unique_ptr< tysoc::TCompoundBody >( dynamic_cast< tysoc::TCompoundBody* >( pyCompoundBody->ptr() ) ) );

        m_pyBodiesRefs.push_back( pyCompoundBody );
    }

    void PyCompound::setPosition( py::array_t<TScalar>& xyz )
    {
        assert( m_compoundRef );

        m_compoundRef->setPosition( numpyToVec3( xyz ) );
    }

    void PyCompound::setRotationMat( py::array_t<TScalar>& rotmat )
    {
        assert( m_compoundRef );

        m_compoundRef->setRotation( numpyToMat3( rotmat ) );
    }

    void PyCompound::setRotationEuler( py::array_t<TScalar>& rpy )
    {
        assert( m_compoundRef );

        m_compoundRef->setRotation( tysoc::TMat3::fromEuler( numpyToVec3( rpy ) ) );
    }

    void PyCompound::setRotationQuaternion( py::array_t<TScalar>& quat )
    {
        assert( m_compoundRef );

        m_compoundRef->setRotation( tysoc::TMat3::fromQuaternion( numpyToVec4( quat ) ) );
    }

    void PyCompound::setTransform( py::array_t<TScalar>& tf )
    {
        assert( m_compoundRef );

        m_compoundRef->setTransform( numpyToMat4( tf ) );
    }

    PyCompoundBody* PyCompound::getBodyByName( const std::string& name )
    {
        assert( m_compoundRef );

        auto _compoundBodyRef = m_compoundRef->getBodyByName( name );
        if ( _compoundBodyRef )
            return new PyCompoundBody( _compoundBodyRef );

        return nullptr;
    }

    py::array_t<TScalar> PyCompound::pos() const
    {
        assert( m_compoundRef );

        return vec3ToNumpy( m_compoundRef->pos() );
    }

    py::array_t<TScalar> PyCompound::rot() const
    {
        assert( m_compoundRef );

        return mat3ToNumpy( m_compoundRef->rot() );
    }

    py::array_t<TScalar> PyCompound::quat() const
    {
        assert( m_compoundRef );

        return vec4ToNumpy( m_compoundRef->quat() );
    }

    py::array_t<TScalar> PyCompound::euler() const
    {
        assert( m_compoundRef );

        return vec3ToNumpy( m_compoundRef->euler() );
    }

    py::array_t<TScalar> PyCompound::tf() const
    {
        assert( m_compoundRef );

        return mat4ToNumpy( m_compoundRef->tf() );
    }

    py::array_t<TScalar> PyCompound::pos0() const
    {
        assert( m_compoundRef );

        return vec3ToNumpy( m_compoundRef->pos0() );
    }

    py::array_t<TScalar> PyCompound::rot0() const
    {
        assert( m_compoundRef );

        return mat3ToNumpy( m_compoundRef->rot0() );
    }

    py::array_t<TScalar> PyCompound::quat0() const
    {
        assert( m_compoundRef );

        return vec4ToNumpy( m_compoundRef->quat0() );
    }

    py::array_t<TScalar> PyCompound::euler0() const
    {
        assert( m_compoundRef );

        return vec3ToNumpy( m_compoundRef->euler0() );
    }

    py::array_t<TScalar> PyCompound::tf0() const
    {
        assert( m_compoundRef );

        return mat4ToNumpy( m_compoundRef->tf0() );
    }

    std::string PyCompound::name() const
    {
        assert( m_compoundRef );

        return m_compoundRef->name();
    }

    tysoc::eDynamicsType PyCompound::dyntype() const
    {
        assert( m_compoundRef );

        return m_compoundRef->dyntype();
    }

}