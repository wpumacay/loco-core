
#include <loco_common_py.h>
#include <loco_object.h>

namespace py = pybind11;

namespace loco 
{
    void bindings_object( py::module& m )
    {
        py::enum_<loco::eObjectType>( m, "ObjectType", py::arithmetic() )
            .value( "BASE", loco::eObjectType::BASE )
            .value( "DRAWABLE", loco::eObjectType::DRAWABLE )
            .value( "SINGLE_BODY", loco::eObjectType::SINGLE_BODY )
            .value( "SINGLE_BODY_COLLIDER", loco::eObjectType::SINGLE_BODY_COLLIDER );

        py::class_<TObject>( m, "Object" )
            .def( "Initialize", &TObject::Initialize )
            .def( "PreStep", &TObject::PreStep )
            .def( "PostStep", &TObject::PostStep )
            .def( "Reset", &TObject::Reset )
            .def( "DetachSim", &TObject::DetachSim )
            .def( "DetachViz", &TObject::DetachViz )
            .def( "GetObjectType", &TObject::GetObjectType )
            .def_property( "pos", 
                []( const TObject* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 3>( self->pos() );
                    },
                []( TObject* self, const py::array_t<TScalar>& arr_pos ) -> void
                    {
                        self->SetPosition( tinymath::nparray_to_vector<TScalar, 3>( arr_pos ) );
                    } )
            .def_property( "rot",
                []( const TObject* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::matrix_to_nparray<TScalar, 3>( self->rot() );
                    },
                []( TObject* self, const py::array_t<TScalar>& arr_rot ) -> void
                    {
                        self->SetRotation( tinymath::nparray_to_matrix<TScalar, 3>( arr_rot ) );
                    } )
            .def_property( "euler",
                []( const TObject* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 3>( self->euler() );
                    },
                []( TObject* self, const py::array_t<TScalar>& arr_euler ) -> void
                    {
                        self->SetEuler( tinymath::nparray_to_vector<TScalar, 3>( arr_euler ) );
                    } )
            .def_property( "quat",
                []( const TObject* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::vector_to_nparray<TScalar, 4>( self->quat() );
                    },
                []( TObject* self, const py::array_t<TScalar>& arr_quat ) -> void
                    {
                        self->SetQuaternion( tinymath::nparray_to_vector<TScalar, 4>( arr_quat ) );
                    } )
            .def_property( "tf",
                []( const TObject* self ) -> py::array_t<TScalar>
                    {
                        return tinymath::matrix_to_nparray<TScalar, 4>( self->tf() );
                    },
                []( TObject* self, const py::array_t<TScalar>& arr_tf ) -> void
                    {
                        self->SetTransform( tinymath::nparray_to_matrix<TScalar, 4>( arr_tf ) );
                    } )
            .def_property_readonly( "name", &TObject::name )
            .def( "__repr__", []( const TObject* self )
                {
                    auto _strrep = std::string( "Object(\n" );
                    _strrep += "cpp-address : " + loco::PointerToHexAddress( self ) + "\n";
                    _strrep += "name        : " + self->name() + "\n";
                    _strrep += "obj-type    : " + loco::ToString( self->GetObjectType() ) + "\n";
                    _strrep += "position    : " + loco::ToString( self->pos() ) + "\n";
                    _strrep += "rotation    :\n" + loco::ToString( self->rot() ) + "\n";
                    _strrep += "euler       : " + loco::ToString( self->euler() ) + "\n";
                    _strrep += "quaternion  : " + loco::ToString( self->quat() ) + "\n";
                    _strrep += "tf          :\n" + loco::ToString( self->tf() ) + "\n";
                    _strrep += ")";
                    return _strrep;
                } );
    }
}