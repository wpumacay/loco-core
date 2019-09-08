
#include <pytysoc_data.h>

namespace py = pybind11;

namespace pytysoc
{

    /***************************************************************************
    *                                                                          *
    *                          ShapeData extension                             *
    *                                                                          *
    ***************************************************************************/

    void PyShapeData::setType( const tysoc::eShapeType& type )
    {
        this->type = type;
    }

    void PyShapeData::setSize( py::array_t<TScalar>& size )
    {
        this->size = numpyToVec3( size );
    }

    void PyShapeData::setLocalPos( py::array_t<TScalar>& localPos )
    {
        this->localPos = numpyToVec3( localPos );
    }

    void PyShapeData::setLocalRot( py::array_t<TScalar>& localRot )
    {
        this->localRot = tysoc::TMat3::fromEuler( numpyToVec3( localRot ) );
    }

    void PyShapeData::setFilename( const std::string& filename )
    {
        this->filename = filename;
    }

    tysoc::eShapeType PyShapeData::getType()
    {
        return this->type;
    }

    py::array_t<TScalar> PyShapeData::getSize()
    {
        return vec3ToNumpy( this->size );
    }

    py::array_t<TScalar> PyShapeData::getLocalPos()
    {
        return vec3ToNumpy( this->localPos );
    }

    py::array_t<TScalar> PyShapeData::getLocalRot()
    {
        return vec3ToNumpy( tysoc::TMat3::toEuler( this->localRot ) );
    }

    std::string PyShapeData::getFilename()
    {
        return this->filename;
    }

    PyShapeData toPyShapeData( const tysoc::TShapeData& data )
    {
        PyShapeData _pydata;
        _pydata.type = data.type;
        _pydata.size = data.size;
        _pydata.localPos = data.localPos;
        _pydata.localRot = data.localRot;
        _pydata.filename = data.filename;

        return _pydata;
    }

    tysoc::TShapeData toTShapeData( const PyShapeData& pydata )
    {
        tysoc::TShapeData _data;
        _data.type = pydata.type;
        _data.size = pydata.size;
        _data.localPos = pydata.localPos;
        _data.localRot = pydata.localRot;
        _data.filename = pydata.filename;

        return _data;
    }

    /***************************************************************************
    *                                                                          *
    *                        CollisionData extension                           *
    *                                                                          *
    ***************************************************************************/

    void PyCollisionData::setType( const tysoc::eShapeType& type )
    {
        this->type = type;
    }

    void PyCollisionData::setSize( py::array_t<TScalar>& size )
    {
        this->size = numpyToVec3( size );
    }

    void PyCollisionData::setLocalPos( py::array_t<TScalar>& localPos )
    {
        this->localPos = numpyToVec3( localPos );
    }

    void PyCollisionData::setLocalRot( py::array_t<TScalar>& localRot )
    {
        this->localRot = tysoc::TMat3::fromEuler( numpyToVec3( localRot ) );
    }

    void PyCollisionData::setFilename( const std::string& filename )
    {
        this->filename = filename;
    }

    tysoc::eShapeType PyCollisionData::getType()
    {
        return this->type;
    }

    py::array_t<TScalar> PyCollisionData::getSize()
    {
        return vec3ToNumpy( this->size );
    }

    py::array_t<TScalar> PyCollisionData::getLocalPos()
    {
        return vec3ToNumpy( this->localPos );
    }

    py::array_t<TScalar> PyCollisionData::getLocalRot()
    {
        return vec3ToNumpy( tysoc::TMat3::toEuler( this->localRot ) );
    }

    std::string PyCollisionData::getFilename()
    {
        return this->filename;
    }

    PyCollisionData toPyCollisionData( const tysoc::TCollisionData& data )
    {
        PyCollisionData _pydata;
        _pydata.type = data.type;
        _pydata.size = data.size;
        _pydata.localPos = data.localPos;
        _pydata.localRot = data.localRot;
        _pydata.filename = data.filename;
        _pydata.collisionGroup = data.collisionGroup;
        _pydata.collisionMask = data.collisionMask;
        _pydata.friction = data.friction;
        _pydata.density = data.density;

        return _pydata;
    }

    tysoc::TCollisionData toTCollisionData( const PyCollisionData& pydata )
    {
        tysoc::TCollisionData _data;
        _data.type = pydata.type;
        _data.size = pydata.size;
        _data.localPos = pydata.localPos;
        _data.localRot = pydata.localRot;
        _data.filename = pydata.filename;
        _data.collisionGroup = pydata.collisionGroup;
        _data.collisionMask = pydata.collisionMask;
        _data.friction = pydata.friction;
        _data.density = pydata.density;

        return _data;
    }

    /***************************************************************************
    *                                                                          *
    *                          VisualData extension                            *
    *                                                                          *
    ***************************************************************************/

    void PyVisualData::setType( const tysoc::eShapeType& type )
    {
        this->type = type;
    }

    void PyVisualData::setSize( py::array_t<TScalar>& size )
    {
        this->size = numpyToVec3( size );
    }

    void PyVisualData::setLocalPos( py::array_t<TScalar>& localPos )
    {
        this->localPos = numpyToVec3( localPos );
    }

    void PyVisualData::setLocalRot( py::array_t<TScalar>& localRot )
    {
        this->localRot = tysoc::TMat3::fromEuler( numpyToVec3( localRot ) );
    }

    void PyVisualData::setFilename( const std::string& filename )
    {
        this->filename = filename;
    }

    tysoc::eShapeType PyVisualData::getType()
    {
        return this->type;
    }

    py::array_t<TScalar> PyVisualData::getSize()
    {
        return vec3ToNumpy( this->size );
    }

    py::array_t<TScalar> PyVisualData::getLocalPos()
    {
        return vec3ToNumpy( this->localPos );
    }

    py::array_t<TScalar> PyVisualData::getLocalRot()
    {
        return vec3ToNumpy( tysoc::TMat3::toEuler( this->localRot ) );
    }

    std::string PyVisualData::getFilename()
    {
        return this->filename;
    }

    PyVisualData toPyVisualData( const tysoc::TVisualData& data )
    {
        PyVisualData _pydata;
        _pydata.type = data.type;
        _pydata.size = data.size;
        _pydata.localPos = data.localPos;
        _pydata.localRot = data.localRot;
        _pydata.filename = data.filename;
        _pydata.ambient = data.ambient;
        _pydata.diffuse = data.diffuse;
        _pydata.specular = data.specular;
        _pydata.shininess = data.shininess;

        return _pydata;
    }

    tysoc::TVisualData toTVisualData( const PyVisualData& pydata )
    {
        tysoc::TVisualData _data;
        _data.type = pydata.type;
        _data.size = pydata.size;
        _data.localPos = pydata.localPos;
        _data.localRot = pydata.localRot;
        _data.filename = pydata.filename;
        _data.ambient = pydata.ambient;
        _data.diffuse = pydata.diffuse;
        _data.specular = pydata.specular;
        _data.shininess = pydata.shininess;

        return _data;
    }

    /***************************************************************************
    *                                                                          *
    *                            BodyData extension                            *
    *                                                                          *
    ***************************************************************************/

    void PyBodyData::setDynType( const tysoc::eDynamicsType& dyntype )
    {
        this->dyntype = dyntype;
    }

    void PyBodyData::setHasInertia( const bool& hasInertia )
    {
        this->hasInertia = hasInertia;
    }

    void PyBodyData::setMass( const TScalar& mass )
    {
        this->mass = mass;
    }

    void PyBodyData::setInertia( py::array_t<TScalar>& inertia )
    {
        auto _stlVecInertia = numpyToVecArray( inertia );

        if ( _stlVecInertia.size() < 6 )
        {
            std::cout << "WARNING> given inertia should have exactly 6 elements "
                      << "corresponding to Ixx, Iyy, Izz, Ixy, Ixz, Iyz in that order" << std::endl;
            return;
        }

        for ( size_t i = 0; i < 6; i++ )
            this->inertia[i] = _stlVecInertia[i];
    }

    void PyBodyData::setInertialFrame( py::array_t<TScalar>& inertialFrame )
    {
        this->inertialFrame = numpyToMat4( inertialFrame );
    }

    tysoc::eDynamicsType PyBodyData::getDynType()
    {
        return this->dyntype;
    }

    bool PyBodyData::getHasInertia()
    {
        return this->hasInertia;
    }

    TScalar PyBodyData::getMass()
    {
        return this->mass;
    }

    py::array_t<TScalar> PyBodyData::getInertia()
    {
        std::vector<TScalar> _stlVecInertia;
        for ( size_t i = 0; i < 6; i++ )
            _stlVecInertia.push_back( this->inertia[i] );

        return vecArrayToNumpy( _stlVecInertia );
    }

    py::array_t<TScalar> PyBodyData::getInertialFrame()
    {
        return mat4ToNumpy( this->inertialFrame );
    }

    void PyBodyData::addCollision( PyCollisionData pyCollision )
    {
        collisions.push_back( toTCollisionData( pyCollision ) );
    }

    void PyBodyData::addVisual( PyVisualData pyVisual )
    {
        visuals.push_back( toTVisualData( pyVisual ) );
    }

    std::vector< PyCollisionData > PyBodyData::getCollisions()
    {
        std::vector< PyCollisionData > _pycols;
        for ( size_t i = 0; i < collisions.size(); i++ )
            _pycols.push_back( toPyCollisionData( collisions[i] ) );

        return _pycols;
    }

    std::vector< PyVisualData > PyBodyData::getVisuals()
    {
        std::vector< PyVisualData > _pyvisuals;
        for ( size_t i = 0; i < visuals.size(); i++ )
            _pyvisuals.push_back( toPyVisualData( visuals[i] ) );
        
        return _pyvisuals;
    }

    PyBodyData toPyBodyData( const tysoc::TBodyData& data )
    {
        PyBodyData _pydata;
        _pydata.dyntype = data.dyntype;
        _pydata.hasInertia = data.hasInertia;
        _pydata.mass = data.mass;
        _pydata.inertia = data.inertia;
        _pydata.inertialFrame = data.inertialFrame;
        _pydata.collisions = data.collisions;
        _pydata.visuals = data.visuals;

        return _pydata;
    }

    tysoc::TBodyData toTBodyData( const PyBodyData& pydata )
    {
        tysoc::TBodyData _data;
        _data.dyntype = pydata.dyntype;
        _data.hasInertia = pydata.hasInertia;
        _data.mass = pydata.mass;
        _data.inertia = pydata.inertia;
        _data.inertialFrame = pydata.inertialFrame;
        _data.collisions = pydata.collisions;
        _data.visuals = pydata.visuals;

        return _data;
    }

}