
#include <visualizer/loco_visualizer_drawable.h>

namespace loco {
namespace visualizer {

    TDrawable::TDrawable( const std::string& name,
                          const TVisualData& visual_data )
        : TObject( name )
    {
        m_Data = visual_data;
        m_LocalTf = visual_data.localTransform;
        m_Visible = true;
        m_Wireframe = false;

        m_Parent = nullptr;
        m_DrawableAdapter = nullptr;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Created TDrawable {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Created TDrawable " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    TDrawable::~TDrawable()
    {
        DetachSim();
        DetachViz();

        m_Parent = nullptr;
        m_DrawableAdapter = nullptr;

    #if defined( LOCO_CORE_USE_TRACK_ALLOCS )
        if ( tinyutils::Logger::IsActive() )
            LOCO_CORE_TRACE( "Loco::Allocs: Destroyed TDrawable {0} @ {1}", m_name, tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Loco::Allocs: Destroyed TDrawable " << m_name << " @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void TDrawable::SetParentObject( TObject* parent_ref )
    {
        LOCO_CORE_ASSERT( parent_ref, "TDrawable::SetParentObject >>> tried adding nullptr" );

        m_Parent = parent_ref;
        m_tf = m_Parent->tf() * m_LocalTf;

        if ( m_DrawableAdapter )
            m_DrawableAdapter->SetTransform( m_tf );
    }

    void TDrawable::SetDrawableAdapter( TIDrawableAdapter* drawable_adapter_ref )
    {
        LOCO_CORE_ASSERT( drawable_adapter_ref, "TDrawable::SetDrawableAdapter >>> tried adding nullptr" );

        m_DrawableAdapter = drawable_adapter_ref;
        m_DrawableAdapter->SetTransform( m_tf );
    }

    void TDrawable::SetVisible( bool visible )
    {
        m_Visible = visible;
        if ( m_DrawableAdapter )
            m_DrawableAdapter->SetVisible( visible );
    }

    void TDrawable::SetWireframe( bool wireframe )
    {
        m_Wireframe = wireframe;
        if ( m_DrawableAdapter )
            m_DrawableAdapter->SetWireframe( wireframe );
    }

    void TDrawable::SetLocalPosition( const TVec3& local_pos )
    {
        m_LocalTf.set( local_pos, 3 );
        _UpdateTransform();
    }

    void TDrawable::SetLocalRotation( const TMat3& local_rot )
    {
        m_LocalTf.set( local_rot );
        _UpdateTransform();
    }

    void TDrawable::SetLocalEuler( const TVec3& local_euler )
    {
        auto local_rot = tinymath::rotation( local_euler );
        m_LocalTf.set( local_rot );
        _UpdateTransform();
    }

    void TDrawable::SetLocalQuat( const TVec4& local_quat )
    {
        auto local_rot = tinymath::rotation( local_quat );
        m_LocalTf.set( local_rot );
        _UpdateTransform();
    }

    void TDrawable::SetLocalTransform( const TMat4& local_tf )
    {
        m_LocalTf = local_tf;
        _UpdateTransform();
    }

    void TDrawable::ChangeSize( const TVec3& new_size )
    {
        m_Data.size = new_size;
        if ( m_DrawableAdapter )
            m_DrawableAdapter->ChangeSize( new_size );
    }

    void TDrawable::ChangeVertexData( const std::vector<float>& vertices, const std::vector<int>& faces )
    {
        if ( m_Data.type != eShapeType::CONVEX_MESH )
        {
            LOCO_CORE_WARN( "TDrawable::ChangeElevationData >>> collision shape {0} is not a mesh", m_name );
            return;
        }
        auto& mesh_data = m_Data.mesh_data;
        mesh_data.vertices = vertices;
        mesh_data.faces = faces;

        if ( m_DrawableAdapter )
            m_DrawableAdapter->ChangeVertexData( vertices, faces );
    }

    void TDrawable::ChangeElevationData( const std::vector< float >& heights )
    {
        if ( m_Data.type != eShapeType::HEIGHTFIELD )
        {
            LOCO_CORE_WARN( "TDrawable::ChangeElevationData >>> collision shape {0} is not a hfield", m_name );
            return;
        }

        auto& hfield_data = m_Data.hfield_data;
        // Sanity check: make sure that both internal and given buffers have same num-elements
        const ssize_t num_samples = hfield_data.nWidthSamples * hfield_data.nDepthSamples;
        if( num_samples != heights.size() )
        {
            LOCO_CORE_WARN( "TDrawable::ChangeElevationData >>> given buffer doesn't match expected size of collision shape {0}", m_name );
            LOCO_CORE_WARN( "\tnx-samples    : {0}", hfield_data.nWidthSamples );
            LOCO_CORE_WARN( "\tny-samples    : {0}", hfield_data.nDepthSamples );
            LOCO_CORE_WARN( "\tinternal-size : {0}", num_samples );
            LOCO_CORE_WARN( "\tgiven-size    : {0}", heights.size() );
            return;
        }

        // Change the internal elevation data (memcpy to avoid destroying and creating internal buffer)
        memcpy( hfield_data.heights.data(), heights.data(), sizeof(float) * num_samples );

        if ( m_DrawableAdapter )
            m_DrawableAdapter->ChangeElevationData( heights );
    }

    void TDrawable::ChangeColor( const TVec3& new_full_color )
    {
        m_Data.ambient = new_full_color;
        m_Data.diffuse = new_full_color;
        m_Data.specular = new_full_color;

        if ( m_DrawableAdapter )
            m_DrawableAdapter->SetColor( new_full_color );
    }

    void TDrawable::ChangeAmbientColor( const TVec3& new_ambient_color )
    {
        m_Data.ambient = new_ambient_color;

        if ( m_DrawableAdapter )
            m_DrawableAdapter->SetAmbientColor( new_ambient_color );
    }

    void TDrawable::ChangeDiffuseColor( const TVec3& new_diffuse_color )
    {
        m_Data.diffuse = new_diffuse_color;

        if ( m_DrawableAdapter )
            m_DrawableAdapter->SetDiffuseColor( new_diffuse_color );
    }

    void TDrawable::ChangeSpecularColor( const TVec3& new_specular_color )
    {
        m_Data.specular = new_specular_color;

        if ( m_DrawableAdapter )
            m_DrawableAdapter->SetSpecularColor( new_specular_color );
    }

    void TDrawable::ChangeShininess( const TScalar& shininess )
    {
        m_Data.shininess = shininess;

        if ( m_DrawableAdapter )
            m_DrawableAdapter->SetShininess( shininess );
    }

    void TDrawable::ChangeTexture( const std::string& texture )
    {
        m_Data.texture = texture;

        if ( m_DrawableAdapter )
            m_DrawableAdapter->SetTexture( texture );
    }

    void TDrawable::_UpdateTransform()
    {
        if ( m_Parent )
            m_tf = m_Parent->tf() * m_LocalTf;

        if ( m_DrawableAdapter )
            m_DrawableAdapter->SetTransform( m_tf );
    }

    void TDrawable::_InitializeInternal()
    {
        // Nothing extra to initialize (for now)
    }

    void TDrawable::_PreStepInternal()
    {
        // Not much to do before a simulation step (for now)
    }

    void TDrawable::_PostStepInternal()
    {
        if ( m_Parent )
            m_tf = m_Parent->tf() * m_LocalTf;

        if ( m_DrawableAdapter )
            m_DrawableAdapter->SetTransform( m_tf );
    }

    void TDrawable::_ResetInternal()
    {
        // Nothing to setup for a reset request
    }

    void TDrawable::_DetachSimInternal()
    {
        // Nothing to detatch from, as we don't share any simulation resources
    }

    void TDrawable::_DetachVizInternal()
    {
        if ( m_DrawableAdapter )
            m_DrawableAdapter->OnDetach();
        m_DrawableAdapter = nullptr;
    }

    void TDrawable::_SetTransformInternal( const TMat4& transform )
    {
        if ( m_Parent )
        {
            LOCO_CORE_WARN( "TDrawable::_SetTransformInternal >>> drawable \"{0}\" has a parent object. Try \
                             changing the transform of the parent object instead" );
            return;
        }

        if ( m_DrawableAdapter )
            m_DrawableAdapter->SetTransform( transform );
    }
}}