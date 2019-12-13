
#include <pytysoc_data.h>

namespace py = pybind11;

namespace pytysoc
{

    /***************************************************************************
    *                                                                          *
    *                       HeightFieldData extension                          *
    *                                                                          *
    ***************************************************************************/

    void PyHeightFieldData::setNumWidthSamples( int numWidthSamples )
    {
        this->nWidthSamples = numWidthSamples;
    }

    void PyHeightFieldData::setNumDepthSamples( int numDepthSamples )
    {
        this->nDepthSamples = numDepthSamples;
    }

    void PyHeightFieldData::setHeightData( py::array_t< float >& heightData )
    {
        this->heightData = numpyToVecArray( heightData );
    }

    int PyHeightFieldData::getNumWidthSamples()
    {
        return this->nWidthSamples;
    }

    int PyHeightFieldData::getNumDepthSamples()
    {
        return this->nDepthSamples;
    }

    py::array_t< float > PyHeightFieldData::getHeightData()
    {
        return vecArrayToNumpy( this->heightData );
    }

    PyHeightFieldData toPyHeightFieldData( const tysoc::THeightFieldData& data )
    {
        PyHeightFieldData _pydata;

        _pydata.nWidthSamples = data.nWidthSamples;
        _pydata.nDepthSamples = data.nDepthSamples;
        _pydata.heightData = data.heightData;

        return _pydata;
    }

    tysoc::THeightFieldData toTHeightFieldData( const PyHeightFieldData& pydata )
    {
        tysoc::THeightFieldData _data;

        _data.nWidthSamples = pydata.nWidthSamples;
        _data.nDepthSamples = pydata.nDepthSamples;
        _data.heightData = pydata.heightData;

        return _data;
    }

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
        this->localTransform.setPosition( numpyToVec3( localPos ) );
    }

    void PyShapeData::setLocalRot( py::array_t<TScalar>& localRot )
    {
        this->localTransform.setRotation( numpyToVec3( localRot ) );
    }

    void PyShapeData::setFilename( const std::string& filename )
    {
        this->filename = filename;
    }

    void PyShapeData::setHeightFieldData( const PyHeightFieldData& pydata )
    {
        this->hdata = toTHeightFieldData( pydata );
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
        return vec3ToNumpy( this->localTransform.getPosition() );
    }

    py::array_t<TScalar> PyShapeData::getLocalRot()
    {
        return vec3ToNumpy( this->localTransform.getRotEuler() );
    }

    std::string PyShapeData::getFilename()
    {
        return this->filename;
    }

    PyHeightFieldData PyShapeData::getHeightFieldData()
    {
        return toPyHeightFieldData( this->hdata );
    }

    PyShapeData toPyShapeData( const tysoc::TShapeData& data )
    {
        PyShapeData _pydata;
        _pydata.type = data.type;
        _pydata.size = data.size;
        _pydata.localTransform = data.localTransform;
        _pydata.filename = data.filename;

        return _pydata;
    }

    tysoc::TShapeData toTShapeData( const PyShapeData& pydata )
    {
        tysoc::TShapeData _data;
        _data.type = pydata.type;
        _data.size = pydata.size;
        _data.localTransform = pydata.localTransform;
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
        this->localTransform.setPosition( numpyToVec3( localPos ) );
    }

    void PyCollisionData::setLocalRot( py::array_t<TScalar>& localRot )
    {
        this->localTransform.setRotation( numpyToVec3( localRot ) );
    }

    void PyCollisionData::setFilename( const std::string& filename )
    {
        this->filename = filename;
    }

    void PyCollisionData::setHeightFieldData( const PyHeightFieldData& pydata )
    {
        this->hdata = toTHeightFieldData( pydata );
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
        return vec3ToNumpy( this->localTransform.getPosition() );
    }

    py::array_t<TScalar> PyCollisionData::getLocalRot()
    {
        return vec3ToNumpy( this->localTransform.getRotEuler() );
    }

    std::string PyCollisionData::getFilename()
    {
        return this->filename;
    }

    PyHeightFieldData PyCollisionData::getHeightFieldData()
    {
        return toPyHeightFieldData( this->hdata );
    }

    PyCollisionData toPyCollisionData( const tysoc::TCollisionData& data )
    {
        PyCollisionData _pydata;
        _pydata.type = data.type;
        _pydata.size = data.size;
        _pydata.localTransform = data.localTransform;
        _pydata.filename = data.filename;
        _pydata.hdata = data.hdata;
        _pydata.collisionGroup = data.collisionGroup;
        _pydata.collisionMask = data.collisionMask;
        _pydata.friction = data.friction;
        _pydata.density = data.density;

        return _pydata;
    }

    tysoc::TCollisionData toCppCollisionData( const PyCollisionData& pydata )
    {
        tysoc::TCollisionData _data;
        _data.type = pydata.type;
        _data.size = pydata.size;
        _data.localTransform = pydata.localTransform;
        _data.filename = pydata.filename;
        _data.collisionGroup = pydata.collisionGroup;
        _data.collisionMask = pydata.collisionMask;
        _data.friction = pydata.friction;
        _data.density = pydata.density;
        _data.hdata = pydata.hdata;

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
        this->localTransform.setPosition( numpyToVec3( localPos ) );
    }

    void PyVisualData::setLocalRot( py::array_t<TScalar>& localRot )
    {
        this->localTransform.setRotation( numpyToVec3( localRot ) );
    }

    void PyVisualData::setFilename( const std::string& filename )
    {
        this->filename = filename;
    }

    void PyVisualData::setHeightFieldData( const PyHeightFieldData& pydata )
    {
        this->hdata = toTHeightFieldData( pydata );
    }

    void PyVisualData::setAmbientColor( py::array_t<TScalar>& ambient )
    {
        this->ambient = numpyToVec3( ambient );
    }

    void PyVisualData::setDiffuseColor( py::array_t<TScalar>& diffuse )
    {
        this->diffuse = numpyToVec3( diffuse );
    }

    void PyVisualData::setSpecularColor( py::array_t<TScalar>& specular )
    {
        this->specular = numpyToVec3( specular );
    }

    void PyVisualData::setShininess( const TScalar& shininess )
    {
        this->shininess = shininess;
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
        return vec3ToNumpy( this->localTransform.getPosition() );
    }

    py::array_t<TScalar> PyVisualData::getLocalRot()
    {
        return vec3ToNumpy( this->localTransform.getRotEuler() );
    }

    std::string PyVisualData::getFilename()
    {
        return this->filename;
    }

    PyHeightFieldData PyVisualData::getHeightFieldData()
    {
        return toPyHeightFieldData( this->hdata );
    }

    py::array_t<TScalar> PyVisualData::getAmbientColor()
    {
        return vec3ToNumpy( this->ambient );
    }

    py::array_t<TScalar> PyVisualData::getDiffuseColor()
    {
        return vec3ToNumpy( this->diffuse );
    }

    py::array_t<TScalar> PyVisualData::getSpecularColor()
    {
        return vec3ToNumpy( this->specular );
    }

    TScalar PyVisualData::getShininess()
    {
        return this->shininess;
    }

    PyVisualData toPyVisualData( const tysoc::TVisualData& data )
    {
        PyVisualData _pydata;
        _pydata.type = data.type;
        _pydata.size = data.size;
        _pydata.localTransform = data.localTransform;
        _pydata.filename = data.filename;
        _pydata.hdata = data.hdata;
        _pydata.ambient = data.ambient;
        _pydata.diffuse = data.diffuse;
        _pydata.specular = data.specular;
        _pydata.shininess = data.shininess;

        return _pydata;
    }

    tysoc::TVisualData toCppVisualData( const PyVisualData& pydata )
    {
        tysoc::TVisualData _data;
        _data.type = pydata.type;
        _data.size = pydata.size;
        _data.localTransform = pydata.localTransform;
        _data.filename = pydata.filename;
        _data.hdata = pydata.hdata;
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

    void PyBodyData::setMass( const TScalar& mass )
    {
        this->inertialData.mass = mass;
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

        this->inertialData.ixx = _stlVecInertia[0];
        this->inertialData.iyy = _stlVecInertia[1];
        this->inertialData.izz = _stlVecInertia[2];
        this->inertialData.ixy = _stlVecInertia[3];
        this->inertialData.ixz = _stlVecInertia[4];
        this->inertialData.iyz = _stlVecInertia[5];
    }

    void PyBodyData::setInertialFrame( py::array_t<TScalar>& inertialFrame )
    {
        this->inertialData.localTransform = numpyToMat4( inertialFrame );
    }

    tysoc::eDynamicsType PyBodyData::getDynType()
    {
        return this->dyntype;
    }

    TScalar PyBodyData::getMass()
    {
        return this->inertialData.mass;
    }

    py::array_t<TScalar> PyBodyData::getInertia()
    {
        std::vector<TScalar> _stlVecInertia = { this->inertialData.ixx, this->inertialData.iyy,
                                                this->inertialData.izz, this->inertialData.ixy,
                                                this->inertialData.ixz, this->inertialData.iyz };

        return vecArrayToNumpy( _stlVecInertia );
    }

    py::array_t<TScalar> PyBodyData::getInertialFrame()
    {
        return mat4ToNumpy( this->inertialData.localTransform );
    }

    void PyBodyData::setCollision( PyCollisionData pyCollision )
    {
        collision = toCppCollisionData( pyCollision );
    }

    void PyBodyData::setVisual( PyVisualData pyVisual )
    {
        visual = toCppVisualData( pyVisual );
    }

    PyCollisionData PyBodyData::getCollision()
    {
        return toPyCollisionData( collision );
    }

    PyVisualData PyBodyData::getVisual()
    {
        return toPyVisualData( visual );
    }

    PyBodyData toPyBodyData( const tysoc::TBodyData& data )
    {
        PyBodyData _pydata;
        _pydata.dyntype = data.dyntype;
        _pydata.inertialData = data.inertialData;
        _pydata.collision = data.collision;
        _pydata.visual = data.visual;

        return _pydata;
    }

    tysoc::TBodyData toTBodyData( const PyBodyData& pydata )
    {
        tysoc::TBodyData _data;
        _data.dyntype = pydata.dyntype;
        _data.inertialData = pydata.inertialData;
        _data.collision = pydata.collision;
        _data.visual = pydata.visual;

        return _data;
    }

    /***************************************************************************
    *                                                                          *
    *                           JointData extension                            *
    *                                                                          *
    ***************************************************************************/

    void PyJointData::setJointType( const tysoc::eJointType& jointType )
    {
        this->type = jointType;
    }

    void PyJointData::setAxis( py::array_t<TScalar>& axis )
    {
        this->axis = numpyToVec3( axis );
    }

    void PyJointData::setLimits( py::array_t<TScalar>& limits )
    {
        this->limits = numpyToVec2( limits );
    }

    void PyJointData::setStiffness( const TScalar& stiffness )
    {
        this->stiffness = stiffness;
    }

    void PyJointData::setArmature( const TScalar& armature )
    {
        this->armature = armature;
    }

    void PyJointData::setDamping( const TScalar& damping )
    {
        this->damping = damping;
    }

    void PyJointData::setRef( const TScalar& ref )
    {
        this->ref = ref;
    }

    void PyJointData::setLocalTransform( py::array_t<TScalar>& localTransform )
    {
        this->localTransform = numpyToMat4( localTransform );
    }

    tysoc::eJointType PyJointData::getJointType()
    {
        return this->type;
    }

    py::array_t<TScalar> PyJointData::getAxis()
    {
        return vec3ToNumpy( this->axis );
    }

    py::array_t<TScalar> PyJointData::getLimits()
    {
        return vec2ToNumpy( this->limits );
    }

    TScalar PyJointData::getStiffness()
    {
        return this->stiffness;
    }

    TScalar PyJointData::getArmature()
    {
        return this->armature;
    }

    TScalar PyJointData::getDamping()
    {
        return this->damping;
    }

    TScalar PyJointData::getRef()
    {
        return this->ref;
    }

    int PyJointData::getNqpos()
    {
        return this->nqpos;
    }

    int PyJointData::getNqvel()
    {
        return this->nqvel;
    }

    py::array_t<TScalar> PyJointData::getLocalTransform()
    {
        return mat4ToNumpy( this->localTransform );
    }

    PyJointData toPyJointData( const tysoc::TJointData& data )
    {
        PyJointData _pydata;
        _pydata.type = data.type;
        _pydata.axis = data.axis;
        _pydata.limits = data.limits;
        _pydata.stiffness = data.stiffness;
        _pydata.armature = data.armature;
        _pydata.damping = data.damping;
        _pydata.ref = data.ref;
        _pydata.nqpos = data.nqpos;
        _pydata.nqvel = data.nqvel;
        _pydata.localTransform = data.localTransform;

        return _pydata;
    }

    tysoc::TJointData toTJointData( const PyJointData& pydata )
    {
        tysoc::TJointData _data;
        _data.type = pydata.type;
        _data.axis = pydata.axis;
        _data.limits = pydata.limits;
        _data.stiffness = pydata.stiffness;
        _data.armature = pydata.armature;
        _data.damping = pydata.damping;
        _data.ref = pydata.ref;
        _data.nqpos = pydata.nqpos;
        _data.nqvel = pydata.nqvel;
        _data.localTransform = pydata.localTransform;

        return _data;
    }

}