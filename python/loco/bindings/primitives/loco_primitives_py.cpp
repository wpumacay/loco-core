
#include <loco_common_py.h>
#include <primitives/loco_single_body.h>

namespace py = pybind11;

namespace loco
{
    void bindings_primitives( py::module& m )
    {
        // Bindings for single-body primitive
        {
            py::class_< TSingleBody, TIBody >( m, "SingleBody" )
                .def( py::init( []( const std::string& name,
                                    const TBodyData& data,
                                    const py::array_t<TScalar>& arr_position,
                                    const py::array_t<TScalar>& arr_rotation ) 
                    {
                        return std::make_unique<TSingleBody>( name,
                                                              data,
                                                              tinymath::nparray_to_vector<TScalar, 3>( arr_position ),
                                                              tinymath::nparray_to_matrix<TScalar, 3>( arr_rotation ) );
                    } ) )
                .def( "__repr__", []( const TSingleBody* self )
                    {
                        auto _strrep = std::string( "SingleBody(\n" );
                        _strrep += "cpp-address : " + loco::PointerToHexAddress( self ) + "\n";
                        _strrep += "name        : " + self->name() + "\n";
                        _strrep += "classType   : " + loco::ToString( self->classType() ) + "\n";
                        _strrep += "dyntype     : " + loco::ToString( self->dyntype() ) + "\n";
                        _strrep += "collision   : " + ( self->collision() ? loco::PointerToHexAddress( self->collision() ) : std::string( "null" ) ) + "\n";
                        _strrep += "visual      : " + ( self->visual() ? loco::PointerToHexAddress( self->visual() ) : std::string( "null" ) ) + "\n";
                        _strrep += "position    : " + loco::ToString( self->pos() ) + "\n";
                        _strrep += "rotation    :\n" + loco::ToString( self->rot() ) + "\n";
                        _strrep += "euler       : " + loco::ToString( self->euler() ) + "\n";
                        _strrep += "quaternion  : " + loco::ToString( self->quat() ) + "\n";
                        _strrep += "tf          :\n" + loco::ToString( self->tf() ) + "\n";
                        _strrep += "localPos    : " + loco::ToString( self->localPos() ) + "\n";
                        _strrep += "localRot    :\n" + loco::ToString( self->localRot() ) + "\n";
                        _strrep += "localEuler  : " + loco::ToString( self->localEuler() ) + "\n";
                        _strrep += "localQuat   : " + loco::ToString( self->localQuat() ) + "\n";
                        _strrep += "localTf     :\n" + loco::ToString( self->localTf() ) + "\n";
                        _strrep += ")";
                        return _strrep;
                    } );
        }
    }
}