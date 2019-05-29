
#include <glviz_ui.h>



namespace tysoc {
namespace viz {

    TCustomUI::TCustomUI( TScenario* scenarioPtr,
                          TCustomContextUI* uiContextPtr )
        : TIVisualizerUI( scenarioPtr )
    {
        m_uiContextPtr          = uiContextPtr;
        m_glfwWindowPtr         = uiContextPtr->glfwWindowPtr;

        m_basicCurrentKinTreeName    = "";
        m_basicCurrentKinTreeIndx    = -1;

        m_basicCurrentBodyIndx      = -1;
        m_basicCurrentJointIndx     = -1;
        m_basicCurrentCollisionIndx = -1;
        m_basicCurrentVisualIndx    = -1;
        m_lastElementPicked         = -1;
    }

    TCustomUI::~TCustomUI()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        
        m_glfwWindowPtr = NULL;

        if ( m_uiContextPtr )
        {
            delete m_uiContextPtr;
            m_uiContextPtr = NULL;
        }
    }

    void TCustomUI::_initUIInternal()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& _io = ImGui::GetIO(); (void) _io;

        ImGui_ImplGlfw_InitForOpenGL( m_glfwWindowPtr, false );
    #ifdef __APPLE__
        ImGui_ImplOpenGL3_Init( "#version 150" );
    #else
        ImGui_ImplOpenGL3_Init( "#version 130" );
    #endif
        ImGui::StyleColorsDark();
    }

    void TCustomUI::_renderUIInternal()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if ( m_uiContextPtr->isUiActive )
        {
            // @TODO|@CHECK|@WIP: Should abstract away the ...
            // widgets and render them like if it was tree

            // Demo functionality
            if ( m_uiContextPtr->isBasicUiActive )
                _renderBasicMainMenu();
        }

        ImGui::Render();
        int _ww, _wh;
        glfwGetFramebufferSize( m_glfwWindowPtr, &_ww, &_wh );
        glViewport( 0, 0, _ww, _wh );
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
    }

    // Basic UI functionality (for testing purposes) *********************************

    void TCustomUI::_renderBasicMainMenu()
    {
        ImGui::Begin( "Main Menu" );

        auto _kinTreeAgents = m_scenarioPtr->getAgentsByType( agent::AGENT_TYPE_KINTREE );

        if ( ImGui::BeginCombo( "Kinematic trees", m_basicCurrentKinTreeName.c_str() ) )
        {
            for ( size_t i = 0; i < _kinTreeAgents.size(); i++ )
            {
                auto _kinTreeAgent = _kinTreeAgents[i];
                bool _isSelected = ( _kinTreeAgent->name() == m_basicCurrentKinTreeName );

                if ( ImGui::Selectable( _kinTreeAgent->name().c_str(), _isSelected ) )
                {
                    m_basicCurrentKinTreeName = _kinTreeAgent->name();
                    m_basicCurrentKinTreeIndx = i;
                }

                if ( _isSelected )
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }

        ImGui::Spacing();
        ImGui::Checkbox( "View Sandbox Menu", &m_uiContextPtr->isSandboxUiActive );

        if ( m_uiContextPtr->isSandboxUiActive )
        {
            ImGui::Spacing();
            ImGui::Checkbox( "showBodies", &m_uiContextPtr->vizSandboxPtr->drawState.showBodies );
            ImGui::Spacing();
            ImGui::Checkbox( "showJoints", &m_uiContextPtr->vizSandboxPtr->drawState.showJoints );
            ImGui::Spacing();
            ImGui::Checkbox( "wireframe", &m_uiContextPtr->vizSandboxPtr->drawState.wireframe );
            ImGui::Spacing();
            ImGui::Checkbox( "frameAxes", &m_uiContextPtr->vizSandboxPtr->drawState.frameAxes );
        }

        ImGui::End();

        if ( m_basicCurrentKinTreeIndx != -1 )
        {
            size_t _indx = m_basicCurrentKinTreeIndx;
            _renderBasicKinTreeVisualsMenu( m_uiContextPtr->vizKinTreePtrs[_indx] );
            _renderBasicKinTreeModelInfoMenu( ( agent::TAgentKinTree* ) _kinTreeAgents[_indx] );
            _renderBasicKinTreeActionsMenu( ( agent::TAgentKinTree* ) _kinTreeAgents[_indx] );
            _renderBasicKinTreeSummary( ( agent::TAgentKinTree* ) _kinTreeAgents[_indx] );
        }
    }

    void TCustomUI::_renderBasicKinTreeVisualsMenu( TCustomVizKinTree* vizKinTreePtr )
    {
        ImGui::Begin( "Kinematic Tree visual options" );

        ImGui::Checkbox( "drawAsWireframe?", &vizKinTreePtr->drawState.drawAsWireframe );
        ImGui::Spacing();
        ImGui::Checkbox( "drawFrameAxes?", &vizKinTreePtr->drawState.drawFrameAxes );
        ImGui::Spacing();
        ImGui::Checkbox( "showBodies?", &vizKinTreePtr->drawState.showBodies );
        ImGui::Spacing();
        ImGui::Checkbox( "showVisuals?", &vizKinTreePtr->drawState.showVisuals );
        ImGui::Spacing();
        ImGui::Checkbox( "showCollisions?", &vizKinTreePtr->drawState.showCollisions );
        ImGui::Spacing();
        ImGui::Checkbox( "showJoints?", &vizKinTreePtr->drawState.showJoints );
        ImGui::Spacing();
        ImGui::Checkbox( "showSensors?", &vizKinTreePtr->drawState.showSensors );
        ImGui::Spacing();
        ImGui::Checkbox( "showActuators?", &vizKinTreePtr->drawState.showActuators );

        ImGui::End();
    }

    void TCustomUI::_renderBasicKinTreeModelInfoMenu( agent::TAgentKinTree* agentKinTreePtr )
    {
        auto _bodies        = agentKinTreePtr->getKinTreeBodies();
        auto _joints        = agentKinTreePtr->getKinTreeJoints();
        auto _collisions    = agentKinTreePtr->getKinTreeCollisions();
        auto _visuals       = agentKinTreePtr->getKinTreeVisuals();

        std::string _currentBodyName = ( m_basicCurrentBodyIndx != -1 ) ? 
                                            _bodies[m_basicCurrentBodyIndx]->name : "";

        std::string _currentJointName = ( m_basicCurrentJointIndx != -1 ) ? 
                                            _bodies[m_basicCurrentJointIndx]->name : "";

        std::string _currentCollisionName = ( m_basicCurrentCollisionIndx != -1 ) ? 
                                            _bodies[m_basicCurrentCollisionIndx]->name : "";

        std::string _currentVisualName = ( m_basicCurrentVisualIndx != -1 ) ? 
                                            _bodies[m_basicCurrentVisualIndx]->name : "";

        ImGui::Begin( "Kinematic Tree model information" );

        if ( ImGui::BeginCombo( "Bodies", _currentBodyName.c_str() ) )
        {
            for ( size_t i = 0; i < _bodies.size(); i++ )
            {
                bool _isSelected = ( _bodies[i]->name == _currentBodyName );

                if ( ImGui::Selectable( _bodies[i]->name.c_str(), _isSelected ) )
                {
                    _currentBodyName = _bodies[i]->name;
                    m_basicCurrentBodyIndx = i;

                    m_lastElementPicked = LAST_PICKED_BODY;
                }

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::Spacing();

        if ( ImGui::BeginCombo( "Joint", _currentJointName.c_str() ) )
        {
            for ( size_t i = 0; i < _joints.size(); i++ )
            {
                bool _isSelected = ( _joints[i]->name == _currentJointName );

                if ( ImGui::Selectable( _joints[i]->name.c_str(), _isSelected ) )
                {
                    _currentBodyName = _joints[i]->name;
                    m_basicCurrentJointIndx = i;

                    m_lastElementPicked = LAST_PICKED_JOINT;
                }

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::Spacing();

        if ( ImGui::BeginCombo( "Collision", _currentCollisionName.c_str() ) )
        {
            for ( size_t i = 0; i < _collisions.size(); i++ )
            {
                bool _isSelected = ( _collisions[i]->name == _currentCollisionName );

                if ( ImGui::Selectable( _collisions[i]->name.c_str(), _isSelected ) )
                {
                    _currentBodyName = _collisions[i]->name;
                    m_basicCurrentCollisionIndx = i;

                    m_lastElementPicked = LAST_PICKED_COLLISION;
                }

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::Spacing();

        if ( ImGui::BeginCombo( "Visual", _currentVisualName.c_str() ) )
        {
            for ( size_t i = 0; i < _visuals.size(); i++ )
            {
                bool _isSelected = ( _visuals[i]->name == _currentVisualName );

                if ( ImGui::Selectable( _visuals[i]->name.c_str(), _isSelected ) )
                {
                    _currentBodyName = _visuals[i]->name;
                    m_basicCurrentVisualIndx = i;

                    m_lastElementPicked = LAST_PICKED_VISUAL;
                }

                if ( _isSelected )
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if ( m_lastElementPicked == LAST_PICKED_BODY )
            _showBodyInfo( _bodies[m_basicCurrentBodyIndx] );

        if ( m_lastElementPicked == LAST_PICKED_JOINT )
            _showJointInfo( _joints[m_basicCurrentJointIndx] );

        if ( m_lastElementPicked == LAST_PICKED_COLLISION )
            _showCollisionInfo( _collisions[m_basicCurrentCollisionIndx] );

        if ( m_lastElementPicked == LAST_PICKED_VISUAL )
            _showVisualInfo( _visuals[m_basicCurrentVisualIndx] );

        ImGui::End();
    }

    void TCustomUI::_showBodyInfo( agent::TKinTreeBody* kinTreeBodyPtr  )
    {
        auto _relpos    = kinTreeBodyPtr->relTransform.getPosition();
        auto _relquat   = kinTreeBodyPtr->relTransform.getRotQuaternion();

        ImGui::Text( "body-name     : %s", kinTreeBodyPtr->name.c_str() );
        ImGui::Text( "rel-pos       : (%.2f,%.2f,%.2f)", _relpos.x, _relpos.y, _relpos.z );
        ImGui::Text( "rel-quat      : (%.2f,%.2f,%.2f,%.2f)", _relquat.x, _relquat.y, _relquat.z, _relquat.w );

        if ( kinTreeBodyPtr->inertiaPtr )
        {
            auto _mass  = kinTreeBodyPtr->inertiaPtr->mass;
            auto _ixx   = kinTreeBodyPtr->inertiaPtr->ixx;
            auto _iyy   = kinTreeBodyPtr->inertiaPtr->iyy;
            auto _izz   = kinTreeBodyPtr->inertiaPtr->izz;
            auto _ixy   = kinTreeBodyPtr->inertiaPtr->ixy;
            auto _ixz   = kinTreeBodyPtr->inertiaPtr->ixz;
            auto _iyz   = kinTreeBodyPtr->inertiaPtr->iyz;

            ImGui::Text( "mass          : %.3f", _mass );
            ImGui::Text( "I(xx-yy-zz)   : %.3f, %.3f, %.3f", _ixx, _iyy, _izz );
            ImGui::Text( "I(xy-xz-yz)   : %.3f, %.3f, %.3f", _ixy, _ixz, _iyz );
        }
    }

    void TCustomUI::_showJointInfo( agent::TKinTreeJoint* kinTreeJointPtr )
    {
        auto _relpos        = kinTreeJointPtr->relTransform.getPosition();
        auto _relquat       = kinTreeJointPtr->relTransform.getRotQuaternion();
        auto _type          = kinTreeJointPtr->type;
        auto _axis          = kinTreeJointPtr->axis;
        auto _lowerLimit    = kinTreeJointPtr->lowerLimit;
        auto _upperLimit    = kinTreeJointPtr->upperLimit;
        auto _limited       = kinTreeJointPtr->limited;
        auto _stiffness     = kinTreeJointPtr->stiffness;
        auto _armature      = kinTreeJointPtr->armature;
        auto _damping       = kinTreeJointPtr->damping;

        ImGui::Text( "joint-name    : %s", kinTreeJointPtr->name.c_str() );
        ImGui::Text( "rel-pos       : (%.2f,%.2f,%.2f)", _relpos.x, _relpos.y, _relpos.z );
        ImGui::Text( "rel-quat      : (%.2f,%.2f,%.2f,%.2f)", _relquat.x, _relquat.y, _relquat.z, _relquat.w );
        ImGui::Text( "type          : %s", _type.c_str() );
        ImGui::Text( "axis          : (%.2f,%.2f,%.2f)", _axis.x, _axis.y, _axis.z );
        ImGui::Text( "lower-limit   : %.3f", _lowerLimit );
        ImGui::Text( "upper-limit   : %.3f", _upperLimit );
        ImGui::Text( "limited       : %s", ( _limited ? "true" : "false" ) );
        ImGui::Text( "stiffness     : %.2f", _stiffness );
        ImGui::Text( "armature      : %.3f", _armature );
        ImGui::Text( "damping       : %.3f", _damping );
    }

    void TCustomUI::_showCollisionInfo( agent::TKinTreeCollision* kinTreeCollisionPtr )
    {
        auto _relpos        = kinTreeCollisionPtr->relTransform.getPosition();
        auto _relquat       = kinTreeCollisionPtr->relTransform.getRotQuaternion();
        auto _type          = kinTreeCollisionPtr->geometry.type;
        auto _size          = kinTreeCollisionPtr->geometry.size;
        auto _conType       = kinTreeCollisionPtr->contype;
        auto _conAffinity   = kinTreeCollisionPtr->conaffinity;
        auto _conDim        = kinTreeCollisionPtr->condim;
        auto _group         = kinTreeCollisionPtr->group;

        ImGui::Text( "collision-name: %s", kinTreeCollisionPtr->name.c_str() );
        ImGui::Text( "rel-pos       : (%.2f,%.2f,%.2f)", _relpos.x, _relpos.y, _relpos.z );
        ImGui::Text( "rel-quat      : (%.2f,%.2f,%.2f,%.2f)", _relquat.x, _relquat.y, _relquat.z, _relquat.w );
        ImGui::Text( "type          : %s", _type.c_str() );
        ImGui::Text( "size          : (%.2f,%.2f,%.2f)", _size.x, _size.y, _size.z );
        ImGui::Text( "contype       : %d", _conType );
        ImGui::Text( "conaffinity   : %d", _conAffinity );
        ImGui::Text( "condim        : %d", _conDim );
        ImGui::Text( "group         : %d", _group );
    }

    void TCustomUI::_showVisualInfo( agent::TKinTreeVisual* kinTreeVisualPtr )
    {
        auto _relpos        = kinTreeVisualPtr->relTransform.getPosition();
        auto _relquat       = kinTreeVisualPtr->relTransform.getRotQuaternion();
        auto _type          = kinTreeVisualPtr->geometry.type;
        auto _size          = kinTreeVisualPtr->geometry.size;
        auto _conType       = kinTreeVisualPtr->contype;
        auto _conAffinity   = kinTreeVisualPtr->conaffinity;
        auto _conDim        = kinTreeVisualPtr->condim;
        auto _group         = kinTreeVisualPtr->group;
        auto _friction      = kinTreeVisualPtr->friction;
        auto _density       = kinTreeVisualPtr->density;

        ImGui::Text( "visual-name   : %s", kinTreeVisualPtr->name.c_str() );
        ImGui::Text( "rel-pos       : (%.2f,%.2f,%.2f)", _relpos.x, _relpos.y, _relpos.z );
        ImGui::Text( "rel-quat      : (%.2f,%.2f,%.2f,%.2f)", _relquat.x, _relquat.y, _relquat.z, _relquat.w );
        ImGui::Text( "type          : %s", _type.c_str() );
        ImGui::Text( "size          : (%.2f,%.2f,%.2f)", _size.x, _size.y, _size.z );
        ImGui::Text( "contype       : %d", _conType );
        ImGui::Text( "conaffinity   : %d", _conAffinity );
        ImGui::Text( "condim        : %d", _conDim );
        ImGui::Text( "group         : %d", _group );
        ImGui::Text( "friction      : %.3f", _friction.buff[0] );
        ImGui::Text( "density       : %.2f", _density );
    }


    void TCustomUI::_renderBasicKinTreeActionsMenu( agent::TAgentKinTree* agentKinTreePtr )
    {
        ImGui::Begin( "Kinematic Tree actuator options" );

        auto _actuators = agentKinTreePtr->getKinTreeActuators();
        std::vector< TScalar > _actions;

        for ( size_t i = 0; i < _actuators.size(); i++ )
        {
            float _val = 0.0f;
            ImGui::SliderFloat( _actuators[i]->name.c_str(), 
                                &_val,
                                _actuators[i]->minCtrl,
                                _actuators[i]->maxCtrl );
            _actions.push_back( _val );
        }

        agentKinTreePtr->setActions( _actions );

        auto _sensors = agentKinTreePtr->getKinTreeSensors();

        for ( size_t i = 0; i < _sensors.size(); i++ )
        {
            if ( _sensors[i]->type == "joint" )
            {
                auto _kinJointSensor = reinterpret_cast< agent::TKinTreeJointSensor* >( _sensors[i] );
                ImGui::Text( "theta%d: %.3f", (int)i, _kinJointSensor->theta );
                ImGui::Text( "thetadot%d: %.3f", (int)i, _kinJointSensor->thetadot );
            }
            else if ( _sensors[i]->type == "body" )
            {
                auto _kinBodySensor = reinterpret_cast< agent::TKinTreeBodySensor* >( _sensors[i] );
                ImGui::Text( "linvel%d: %.3f, %.3f, %.3f", (int)i, 
                             _kinBodySensor->linVelocity.x, 
                             _kinBodySensor->linVelocity.y, 
                             _kinBodySensor->linVelocity.z );
                ImGui::Text( "linacc%d: %.3f, %.3f, %.3f", (int)i,  
                             _kinBodySensor->linAcceleration.x, 
                             _kinBodySensor->linAcceleration.y, 
                             _kinBodySensor->linAcceleration.z );
            }
        }

        ImGui::End();
    }

    void TCustomUI::_renderBasicKinTreeSummary( agent::TAgentKinTree* agentKinTreePtr )
    {
        ImGui::Begin( "Kinematic Tree Summary" );

        TGenericParams& _summary = agentKinTreePtr->getSummary();

        // show all floats first
        auto _floats = _summary.floats();
        
        for ( auto _it = _floats.begin(); _it != _floats.end(); _it++ )
        {
            std::string _strField = _it->first + " : " + std::to_string( _it->second ) ;
            ImGui::Text( _strField.c_str() );
        }

        // show all vec3s next
        auto _vec3s = _summary.vec3s();

        for ( auto _it = _vec3s.begin(); _it != _vec3s.end(); _it++ )
        {
            std::string _strField = _it->first + " : (" + 
                                    std::to_string( _it->second.x ) + "," +
                                    std::to_string( _it->second.y ) + "," +
                                    std::to_string( _it->second.z ) + ")";
            ImGui::Text( _strField.c_str() );
        }

        ImGui::End();
    }

    // *******************************************************************************

}}