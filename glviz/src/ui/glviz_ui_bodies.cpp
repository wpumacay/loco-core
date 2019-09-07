
#include <ui/glviz_ui_bodies.h>


namespace tysoc {


    TGLUiBodies::TGLUiBodies( TGLUi* parentUiPtr,
                              TScenario* scenarioPtr )
    {
        m_parentUiPtr = parentUiPtr;
        m_scenarioPtr = scenarioPtr;

        m_currentBodyIndex = -1;
        m_currentCollisionIndex = -1;
        m_currentVisualIndex = -1;

        m_currentBodyPtr = NULL;
        m_currentCollisionPtr = NULL;
        m_currentVisualPtr = NULL;

        m_currentBodyName = "";
        m_currentCollisionName = "";
        m_currentVisualName = "";
    }

    TGLUiBodies::~TGLUiBodies()
    {
        m_parentUiPtr = NULL;
        m_scenarioPtr = NULL;
        m_currentBodyPtr = NULL;
        m_currentCollisionPtr = NULL;
        m_currentVisualPtr = NULL;
    }

    void TGLUiBodies::render()
    {
        assert( m_scenarioPtr );

        ImGui::Begin( "Bodies-information" );
        
        auto _singleBodies = m_scenarioPtr->getBodies();
        if ( ImGui::BeginCombo( "single-bodies", m_currentBodyName.c_str() ) )
        {
            for ( size_t i = 0; i < _singleBodies.size(); i++ )
            {
                auto _singleBody = _singleBodies[i];
                bool _isSelected = ( _singleBody->name() == m_currentBodyName );

                if ( ImGui::Selectable( _singleBody->name().c_str(), _isSelected ) )
                {
                    if ( _singleBody->name() != m_currentBodyName )
                    {
                        m_currentCollisionPtr = NULL;
                        m_currentCollisionIndex = -1;
                        m_currentCollisionName = "";

                        m_currentVisualPtr = NULL;
                        m_currentVisualIndex = -1;
                        m_currentVisualName = "";
                    }
                    
                    m_currentBodyIndex  = i;
                    m_currentBodyPtr    = _singleBody;
                    m_currentBodyName   = _singleBody->name();
                }

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        if ( m_currentBodyIndex != -1 )
            _renderBodyInfo();

        if ( m_currentCollisionIndex != -1 )
            _renderCollisionInfo();

        if ( m_currentVisualIndex != -1 )
            _renderVisualInfo();

        ImGui::End();
    }

    void TGLUiBodies::_renderBodyInfo()
    {
        assert( m_currentBodyPtr != NULL );

        auto _position = m_currentBodyPtr->pos();
        auto _rotation = m_currentBodyPtr->euler();

        float _posvals[3] = { _position.x, _position.y, _position.z };
        float _rotvals[3] = { _rotation.x, _rotation.y, _rotation.z };

        ImGui::InputFloat3( "pos-xyz", _posvals );
        ImGui::InputFloat3( "rot-euler-xyz", _rotvals );

        _position = { _posvals[0], _posvals[1], _posvals[2] };
        _rotation = { _rotvals[0], _rotvals[1], _rotvals[2] };

        m_currentBodyPtr->setPosition( _position );
        m_currentBodyPtr->setRotation( TMat3::fromEuler( _rotation ) );

        ImGui::Spacing();

        auto _collisions = m_currentBodyPtr->collisions();
        if ( ImGui::BeginCombo( "colliders", m_currentCollisionName.c_str() ) )
        {
            for ( size_t i = 0; i < _collisions.size(); i++ )
            {
                auto _collision = _collisions[i];
                bool _isSelected = ( _collision->name() == m_currentCollisionName );

                if ( ImGui::Selectable( _collision->name().c_str(), _isSelected ) )
                {
                    m_currentCollisionIndex  = i;
                    m_currentCollisionPtr    = _collision;
                    m_currentCollisionName   = _collision->name();
                }

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::Spacing();

        auto _visuals = m_currentBodyPtr->visuals();
        if ( ImGui::BeginCombo( "visuals", m_currentVisualName.c_str() ) )
        {
            for ( size_t i = 0; i < _visuals.size(); i++ )
            {
                auto _visual = _visuals[i];
                bool _isSelected = ( _visual->name() == m_currentVisualName );

                if ( ImGui::Selectable( _visual->name().c_str(), _isSelected ) )
                {
                    m_currentVisualIndex  = i;
                    m_currentVisualPtr    = _visual;
                    m_currentVisualName   = _visual->name();
                }

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }
    }

    void TGLUiBodies::_renderCollisionInfo()
    {
        assert( m_currentCollisionPtr != NULL );

        ImGui::Spacing();

        auto _size = m_currentCollisionPtr->size();
        _renderShapeInfo( m_currentCollisionPtr->data(), _size, true );

        m_currentCollisionPtr->changeSize( _size );
    }

    void TGLUiBodies::_renderVisualInfo()
    {
        assert( m_currentVisualPtr != NULL );

        ImGui::Spacing();

        auto _size = m_currentVisualPtr->size();
        _renderShapeInfo( m_currentVisualPtr->data(), _size, false );

        m_currentVisualPtr->changeSize( _size );
    }

    void TGLUiBodies::_renderShapeInfo( const TShapeData& data,
                                        TVec3& size,
                                        bool isCollider )
    {
        if ( data.type == eShapeType::PLANE )
        {
            ImGui::InputFloat( isCollider ? "width-col" : "width-vis", &size.x );
            ImGui::InputFloat( isCollider ? "depth-col" : "width-vis", &size.y );
        }
        else if ( data.type == eShapeType::BOX )
        {
            ImGui::InputFloat( isCollider ? "width-col" : "width-vis", &size.x );
            ImGui::InputFloat( isCollider ? "depth-col" : "depth-vis", &size.y );
            ImGui::InputFloat( isCollider ? "height-col" : "height-vis", &size.z );
        }
        else if ( data.type == eShapeType::SPHERE )
        {
            ImGui::InputFloat( isCollider ? "radius-col" : "radius-vis", &size.x );
        }
        else if ( data.type == eShapeType::CYLINDER )
        {
            ImGui::InputFloat( isCollider ? "radius-col" : "radius-vis", &size.x );
            ImGui::InputFloat( isCollider ? "height-col" : "height-vis", &size.y );
        }
        else if ( data.type == eShapeType::CAPSULE )
        {
            ImGui::InputFloat( isCollider ? "radius-col" : "radius-vis", &size.x );
            ImGui::InputFloat( isCollider ? "height-col" : "height-vis", &size.y );
        }
    }

}