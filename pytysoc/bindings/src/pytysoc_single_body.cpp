
#include <pytysoc_single_body.h>

namespace py = pybind11;

namespace pytysoc
{

    PySingleBody::PySingleBody( const std::string& name,
                                const PyBodyData& data,
                                py::array_t<TScalar>& xyz,
                                py::array_t<TScalar>& rpy )
        : PyBody( name, data )
    {
        /* instantiate wrapper body object */
        m_bodyRef = new tysoc::TSingleBody( name, 
                                            toTBodyData( data ), 
                                            numpyToVec3( xyz ),
                                            tysoc::TMat3::fromEuler( numpyToVec3( rpy ) ) );

        /* wrap alredy-existing collider object */
        auto _collision = m_bodyRef->collision();
        if ( _collision )
        {
            m_pyCollision = new PyCollision( _collision );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyCollision->setParentBody( this );
        }

        /* wrap alredy-existing drawable object */
        auto _visual = m_bodyRef->visual();
        if ( _visual )
        {
            m_pyVisual = new PyVisual( _visual );
            // @todo: setting parent again (twice). Okish so far, but should change it later to avoid any issues
            m_pyVisual->setParentBody( this );
        }
    }

    PySingleBody::~PySingleBody()
    {
        // nothing extra to release for now
    }

}