
#include <terrains/terrain_sections_connectedpath.h>



namespace tysocterrain
{
    // Generators

    TSineProfileGenerator::TSineProfileGenerator( float ampl, 
                                                  float period, 
                                                  float phase )
    {
        m_ampl = ampl;
        m_period = period;
        m_phase = phase;
    }

    float TSineProfileGenerator::generate( float x )
    {
        return m_ampl * std::sin( ( 2.0f * M_PI * x / m_period ) + m_phase );
    }


    // Scenario
    
    TScenarioConnectedPath::TScenarioConnectedPath( TProfileGenerator* generator,
                                                    float sectionDeltaX, 
                                                    float sectionsDepth, 
                                                    float sectionTickness,
                                                    float baseY,
                                                    float baseZ )
    {
        m_sectionsDeltaX    = sectionDeltaX;
        m_sectionsTickness  = sectionTickness;
        m_sectionsDepth     = sectionsDepth;
        m_currentX          = 0.0f;
        m_baseY             = baseY;
        m_baseZ             = baseZ;
        m_generator         = generator;
    }

    TScenarioConnectedPath::~TScenarioConnectedPath()
    {
        if ( m_generator )
        {
            delete m_generator;
            m_generator = NULL;
        }
    }

    void TScenarioConnectedPath::initialize( TScenarioContext* scenarioContext )
    {
        m_currentX = scenarioContext->trackingpoint.x;

        // create some sections in front
        for ( size_t i = 0; i < 20; i++ )
        {
            _createSection();
        }
    }

    void TScenarioConnectedPath::update( TScenarioContext* scenarioContext )
    {
        _updateSections( scenarioContext );
    }

    void TScenarioConnectedPath::_updateSections( TScenarioContext* scenarioContext )
    {
        auto _dx = ( scenarioContext->trackingpoint.x - m_currentX );
        if ( _dx > m_sectionsDeltaX )
        {
            _createSection();
        }
    }
    
    void TScenarioConnectedPath::_createSection()
    {
        if ( m_sections.size() != 0 )
        {
            // go ahead by one section delta
            m_currentX += m_sectionsDeltaX;
        }

        float _prevX = m_currentX - 0.5f * m_sectionsDeltaX;
        float _nextX = m_currentX + 0.5f * m_sectionsDeltaX;
        
        // create the new section data
        auto _section = new TConnectedPathSection();
        _section->awaitingInitialization = true;
        _section->awaitingDestruction = false;
        // compute vertices
        {
            _section->p1.x = _prevX;
            _section->p1.y = m_baseY + 0.5f * m_sectionsDepth;
            _section->p1.z = m_generator->generate( _prevX );

            _section->p2.x = _nextX;
            _section->p2.y = m_baseY + 0.5f * m_sectionsDepth;
            _section->p2.z = m_generator->generate( _nextX );

            _section->p3.x = _section->p2.x;
            _section->p3.y = _section->p2.y - m_sectionsDepth;
            _section->p3.z = _section->p2.z;

            _section->p4.x = _section->p1.x;
            _section->p4.y = _section->p1.y - m_sectionsDepth;
            _section->p4.z = _section->p1.z;
        }
        // compute other necessary info
        {
            // position
            _section->pos.x = ( _section->p1.x + _section->p2.x + _section->p3.x + _section->p4.x ) / 4.0f;
            _section->pos.y = ( _section->p1.y + _section->p2.y + _section->p3.y + _section->p4.y ) / 4.0f;
            _section->pos.z = ( _section->p1.z + _section->p2.z + _section->p3.z + _section->p4.z ) / 4.0f;
            
            // normal ( p21 cross p41, where p41 = p4 - p1, p21 = p2 - p1 )
            auto _p41x = _section->p4.x - _section->p1.x; // 0.0
            auto _p41y = _section->p4.y - _section->p1.y; // depth
            auto _p41z = _section->p4.z - _section->p1.z; // 0.0

            auto _p21x = _section->p2.x - _section->p1.x; // spawndeltax
            auto _p21y = _section->p2.y - _section->p1.y; // 0.0
            auto _p21z = _section->p2.z - _section->p1.z; // dz

            // @TODO: Add some asserts. Because of the assumed plane the normal.y must be 0.0
            _section->normal.x = _p21y * _p41z - _p41y * _p21z; // -depth * dz
            _section->normal.y = _p21z * _p41x - _p41z * _p21x; // 0.0
            _section->normal.z = _p21x * _p41y - _p41x * _p21y; // spawndelta * depth

            auto _norm = std::sqrt( _section->normal.x * _section->normal.x +
                                    _section->normal.y * _section->normal.y +
                                    _section->normal.z * _section->normal.z );

            _section->normal.x = _section->normal.x / _norm;
            _section->normal.y = _section->normal.y / _norm;
            _section->normal.z = _section->normal.z / _norm;

            // size
            _section->size.x = std::sqrt( _p21x * _p21x + _p21y * _p21y + _p21z * _p21z );
            _section->size.y = std::sqrt( _p41x * _p41x + _p41y * _p41y + _p41z * _p41z ); // should be the same as m_sectionDepth
            _section->size.z = m_sectionsTickness;

            _section->rbound = 0.5f * std::sqrt( _section->size.x * _section->size.x +
                                                 _section->size.y * _section->size.y +
                                                 _section->size.z * _section->size.z );

            // rotation matrix -> stored in column major form
            /*
            *  
            * [ _  0  _ ]    [  nz  0  nx ]
            * [ t  1  n ] =  [  0   1  0  ] (because it's a plane)
            * [    0    ]    [ -nx  0  nz ]
            */
            _section->rotmat[0] = _section->normal.z;
            _section->rotmat[1] = 0.0f;
            _section->rotmat[2] = -_section->normal.x;

            _section->rotmat[3] = 0.0f;
            _section->rotmat[4] = 1.0f;
            _section->rotmat[5] = 0.0f;

            _section->rotmat[6] = _section->normal.x;
            _section->rotmat[7] = _section->normal.y;// must be 0.0
            _section->rotmat[8] = _section->normal.z;
        }

        m_sections.push_back( _section );
    }

    void TScenarioConnectedPath::_removeSection()
    {
        // @TODO
    }
}