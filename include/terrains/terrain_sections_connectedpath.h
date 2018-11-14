
#pragma once

#include <terrain_base.h>


namespace tysocterrain
{


    struct TConnectedPathSection
    {
        struct { float x; float y; float z; }   size;
        struct { float x; float y; float z; }   pos;
        float                                   rotmat[9];
        struct { float x; float y; float z; }   p1;
        struct { float x; float y; float z; }   p2;
        struct { float x; float y; float z; }   p3;
        struct { float x; float y; float z; }   p4;
        struct { float x; float y; float z; }   normal;
        float                                   rbound;
        bool                                    awaitingInitialization;
        bool                                    awaitingDestruction;
        // @TODO: Check destruction (not sure if will leak)
    };

    struct TProfileGenerator
    {
        virtual float generate( float x ) = 0;
    };

    struct TSineProfileGenerator : public TProfileGenerator
    {
        private :

        float m_period;
        float m_phase;
        float m_ampl;

        public :

        TSineProfileGenerator( float ampl, float period, float phase );
        float generate( float x ) override;
    };

    class TScenarioConnectedPath : public TScenario
    {

        private :

        float m_currentX;// current position in the axis of generation
        float m_baseY;
        float m_baseZ;

        float m_sectionsDepth;
        float m_sectionsTickness;

        float m_sectionsDeltaX;

        std::vector< TConnectedPathSection* > m_sections;

        TProfileGenerator* m_generator;

        void _generateProfile1D( float x );


        void _updateSections( TScenarioContext* scenarioContext );
        void _createSection();
        void _removeSection();


        public :

        TScenarioConnectedPath( TProfileGenerator* generator,
                                float sectionDeltaX, 
                                float sectionsDepth, 
                                float sectionTickness,
                                float baseY = 0.0f,
                                float baseZ = 0.0f );
        ~TScenarioConnectedPath();

        void initialize( TScenarioContext* scenarioContext ) override;
        void update( TScenarioContext* scenarioContext ) override;

        std::vector< TConnectedPathSection* > getSections() { return m_sections; }
    };


}