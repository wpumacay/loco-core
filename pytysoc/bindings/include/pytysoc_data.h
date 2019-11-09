
#pragma once

#include <pytysoc_common.h>
#include <components/data.h>

namespace py = pybind11;

namespace pytysoc
{

    struct PyHeightFieldData : public tysoc::THeightFieldData
    {
        void setNumWidthSamples( int numWidthSamples );
        void setNumDepthSamples( int numDepthSamples );
        void setHeightData( py::array_t< float >& heightData );

        int getNumWidthSamples();
        int getNumDepthSamples();
        py::array_t< float > getHeightData();
    };

    PyHeightFieldData toPyHeightFieldData( const tysoc::THeightFieldData& data );
    tysoc::THeightFieldData toTHeightFieldData( const PyHeightFieldData& pydata );

    struct PyShapeData : public tysoc::TShapeData
    {
        void setType( const tysoc::eShapeType& type );
        void setSize( py::array_t<TScalar>& size );
        void setLocalPos( py::array_t<TScalar>& localPos );
        void setLocalRot( py::array_t<TScalar>& localRot );
        void setFilename( const std::string& filename );
        void setHeightFieldData( const PyHeightFieldData& hdata );

        tysoc::eShapeType getType();
        py::array_t<TScalar> getSize();
        py::array_t<TScalar> getLocalPos();
        py::array_t<TScalar> getLocalRot();
        std::string getFilename();
        PyHeightFieldData getHeightFieldData();
    };

    PyShapeData toPyShapeData( const tysoc::TShapeData& data );
    tysoc::TShapeData toTShapeData( const PyShapeData& pydata );

    struct PyCollisionData : public tysoc::TCollisionData
    {
        void setType( const tysoc::eShapeType& type );
        void setSize( py::array_t<TScalar>& size );
        void setLocalPos( py::array_t<TScalar>& localPos );
        void setLocalRot( py::array_t<TScalar>& localRot );
        void setFilename( const std::string& filename );
        void setHeightFieldData( const PyHeightFieldData& hdata );

        tysoc::eShapeType getType();
        py::array_t<TScalar> getSize();
        py::array_t<TScalar> getLocalPos();
        py::array_t<TScalar> getLocalRot();
        std::string getFilename();
        PyHeightFieldData getHeightFieldData();
    };

    PyCollisionData toPyCollisionData( const tysoc::TCollisionData& data );
    tysoc::TCollisionData toTCollisionData( const PyCollisionData& pydata );

    struct PyVisualData : public tysoc::TVisualData
    {
        void setType( const tysoc::eShapeType& type );
        void setSize( py::array_t<TScalar>& size );
        void setLocalPos( py::array_t<TScalar>& localPos );
        void setLocalRot( py::array_t<TScalar>& localRot );
        void setFilename( const std::string& filename );
        void setHeightFieldData( const PyHeightFieldData& hdata );
        void setAmbientColor( py::array_t<TScalar>& ambient );
        void setDiffuseColor( py::array_t<TScalar>& diffuse );
        void setSpecularColor( py::array_t<TScalar>& specular );
        void setShininess( const TScalar& shininess );

        tysoc::eShapeType getType();
        py::array_t<TScalar> getSize();
        py::array_t<TScalar> getLocalPos();
        py::array_t<TScalar> getLocalRot();
        std::string getFilename();
        PyHeightFieldData getHeightFieldData();
        py::array_t<TScalar> getAmbientColor();
        py::array_t<TScalar> getDiffuseColor();
        py::array_t<TScalar> getSpecularColor();
        TScalar getShininess();
    };

    PyVisualData toPyVisualData( const tysoc::TVisualData& data );
    tysoc::TVisualData toTVisualData( const PyVisualData& pydata );

    struct PyBodyData : public tysoc::TBodyData
    {
        void setDynType( const tysoc::eDynamicsType& dyntype );
        void setMass( const TScalar& mass );
        void setInertia( py::array_t<TScalar>& inertia );
        void setInertialFrame( py::array_t<TScalar>& inertialFrame );

        tysoc::eDynamicsType getDynType();
        bool getHasInertia();
        TScalar getMass();
        py::array_t<TScalar> getInertia();
        py::array_t<TScalar> getInertialFrame();

        void addCollision( PyCollisionData collision );
        void addVisual( PyVisualData visual );

        std::vector< PyCollisionData > getCollisions();
        std::vector< PyVisualData > getVisuals();

        std::vector< PyCollisionData > pyCollisions;
        std::vector< PyVisualData > pyVisuals;
    };

    PyBodyData toPyBodyData( const tysoc::TBodyData& data );
    tysoc::TBodyData toTBodyData( const PyBodyData& pydata );

}

#define PYTYSOC_ENUM_BINDINGS(m) \
    py::enum_<tysoc::eShapeType>(m, "eShapeType", py::arithmetic()) \
            .value( "NONE", tysoc::eShapeType::NONE ) \
            .value( "BOX", tysoc::eShapeType::BOX ) \
            .value( "PLANE", tysoc::eShapeType::PLANE ) \
            .value( "SPHERE", tysoc::eShapeType::SPHERE ) \
            .value( "CYLINDER", tysoc::eShapeType::CYLINDER ) \
            .value( "CAPSULE", tysoc::eShapeType::CAPSULE ) \
            .value( "MESH", tysoc::eShapeType::MESH ) \
            .value( "HFIELD", tysoc::eShapeType::HFIELD ); \
    py::enum_<tysoc::eJointType>(m, "eJointType", py::arithmetic()) \
            .value( "FREE", tysoc::eJointType::FREE ) \
            .value( "REVOLUTE", tysoc::eJointType::REVOLUTE ) \
            .value( "PRISMATIC", tysoc::eJointType::PRISMATIC ) \
            .value( "SPHERICAL", tysoc::eJointType::SPHERICAL ) \
            .value( "PLANAR", tysoc::eJointType::PLANAR ) \
            .value( "FIXED", tysoc::eJointType::FIXED ); \
    py::enum_<tysoc::eDynamicsType>(m, "eDynamicsType", py::arithmetic()) \
            .value( "DYNAMIC", tysoc::eDynamicsType::DYNAMIC ) \
            .value( "STATIC", tysoc::eDynamicsType::STATIC ) \
            .value( "KINEMATIC", tysoc::eDynamicsType::KINEMATIC );

#define PYTYSOC_DATA_BINDINGS(m) \
    py::class_<pytysoc::PyHeightFieldData>(m, "PyHeightFieldData") \
            .def( py::init<>() ) \
            .def_property( "nWidthSamples", &pytysoc::PyHeightFieldData::getNumWidthSamples, &pytysoc::PyHeightFieldData::setNumWidthSamples ) \
            .def_property( "nDepthSamples", &pytysoc::PyHeightFieldData::getNumDepthSamples, &pytysoc::PyHeightFieldData::setNumDepthSamples ) \
            .def_property( "heightData", &pytysoc::PyHeightFieldData::getHeightData, &pytysoc::PyHeightFieldData::setHeightData ); \
    py::class_<pytysoc::PyShapeData>(m, "PyShapeData") \
            .def( py::init<>() ) \
            .def_property( "type", &pytysoc::PyShapeData::getType, &pytysoc::PyShapeData::setType ) \
            .def_property( "size", &pytysoc::PyShapeData::getSize, &pytysoc::PyShapeData::setSize ) \
            .def_property( "localPos", &pytysoc::PyShapeData::getLocalPos, &pytysoc::PyShapeData::setLocalPos ) \
            .def_property( "localRot", &pytysoc::PyShapeData::getLocalRot, &pytysoc::PyShapeData::setLocalRot ) \
            .def_property( "filename", &pytysoc::PyShapeData::getFilename, &pytysoc::PyShapeData::setFilename ) \
            .def_property( "hdata", &pytysoc::PyShapeData::getHeightFieldData, &pytysoc::PyShapeData::setHeightFieldData ); \
    py::class_<pytysoc::PyCollisionData>(m, "PyCollisionData") \
            .def( py::init<>() ) \
            .def_property( "type", &pytysoc::PyCollisionData::getType, &pytysoc::PyCollisionData::setType ) \
            .def_property( "size", &pytysoc::PyCollisionData::getSize, &pytysoc::PyCollisionData::setSize ) \
            .def_property( "localPos", &pytysoc::PyCollisionData::getLocalPos, &pytysoc::PyCollisionData::setLocalPos ) \
            .def_property( "localRot", &pytysoc::PyCollisionData::getLocalRot, &pytysoc::PyCollisionData::setLocalRot ) \
            .def_property( "filename", &pytysoc::PyCollisionData::getFilename, &pytysoc::PyCollisionData::setFilename ) \
            .def_property( "hdata", &pytysoc::PyCollisionData::getHeightFieldData, &pytysoc::PyCollisionData::setHeightFieldData ); \
    py::class_<pytysoc::PyVisualData>(m, "PyVisualData") \
            .def( py::init<>() ) \
            .def_property( "type", &pytysoc::PyVisualData::getType, &pytysoc::PyVisualData::setType ) \
            .def_property( "size", &pytysoc::PyVisualData::getSize, &pytysoc::PyVisualData::setSize ) \
            .def_property( "localPos", &pytysoc::PyVisualData::getLocalPos, &pytysoc::PyVisualData::setLocalPos ) \
            .def_property( "localRot", &pytysoc::PyVisualData::getLocalRot, &pytysoc::PyVisualData::setLocalRot ) \
            .def_property( "filename", &pytysoc::PyVisualData::getFilename, &pytysoc::PyVisualData::setFilename ) \
            .def_property( "ambient", &pytysoc::PyVisualData::getAmbientColor, &pytysoc::PyVisualData::setAmbientColor ) \
            .def_property( "diffuse", &pytysoc::PyVisualData::getDiffuseColor, &pytysoc::PyVisualData::setDiffuseColor ) \
            .def_property( "specular", &pytysoc::PyVisualData::getSpecularColor, &pytysoc::PyVisualData::setSpecularColor ) \
            .def_property( "shininess", &pytysoc::PyVisualData::getShininess, &pytysoc::PyVisualData::setShininess ) \
            .def_property( "hdata", &pytysoc::PyVisualData::getHeightFieldData, &pytysoc::PyVisualData::setHeightFieldData ); \
    py::class_<pytysoc::PyBodyData>(m, "PyBodyData") \
            .def( py::init<>() ) \
            .def_property( "dyntype", &pytysoc::PyBodyData::getDynType, &pytysoc::PyBodyData::setDynType ) \
            .def_property( "mass", &pytysoc::PyBodyData::getMass, &pytysoc::PyBodyData::setMass ) \
            .def_property( "inertia", &pytysoc::PyBodyData::getInertia, &pytysoc::PyBodyData::setInertia ) \
            .def_property( "inertialFrame", &pytysoc::PyBodyData::getInertialFrame, &pytysoc::PyBodyData::setInertialFrame ) \
            .def( "addCollision", &pytysoc::PyBodyData::addCollision ) \
            .def( "addVisual", &pytysoc::PyBodyData::addVisual ) \
            .def( "getCollisions", &pytysoc::PyBodyData::getCollisions ) \
            .def( "getVisuals", &pytysoc::PyBodyData::getVisuals );
