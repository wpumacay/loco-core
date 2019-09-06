
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
                    m_currentBodyIndex  = i;
                    m_currentBodyPtr    = _singleBodies[i];
                    m_currentBodyName   = _singleBody->name();
                }

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        if ( m_currentBodyIndex != -1 )
            _renderBodyInfo();

        ImGui::End();

        if ( m_currentCollisionIndex != -1 )
            _renderCollisionPanel();

        if ( m_currentVisualIndex != -1 )
            _renderVisualPanel();
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
    }

    void TGLUiBodies::_renderCollisionPanel()
    {

    }

    void TGLUiBodies::_renderVisualPanel()
    {

    }

}