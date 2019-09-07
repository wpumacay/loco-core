
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

    void PyBodyData::addCollision( PyCollisionData collision )
    {
        pyCollisions.push_back( collision );
    }

    void PyBodyData::addVisual( PyVisualData visual )
    {
        pyVisuals.push_back( visual );
    }

    std::vector< PyCollisionData > PyBodyData::getCollisions()
    {
        return pyCollisions;
    }

    std::vector< PyVisualData > PyBodyData::getVisuals()
    {
        return pyVisuals;
    }

}