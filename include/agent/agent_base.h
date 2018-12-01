
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map>

namespace tysocAgent
{


    class TIAgent
    {

        protected : 

        std::string m_name;

        float m_posX;
        float m_posY;
        float m_posZ;

        virtual void _updateInternal() = 0;
        virtual void _setPositionInternal( float x, float y, float z ) = 0;

        public :

        TIAgent( const std::string& name,
                 float startX = 0.0f, 
                 float startY = 0.0f, 
                 float startZ = 0.0f );
        virtual ~TIAgent();

        void setPosition( float x, float y, float z );
        void getPosition( float& x, float& y, float& z );
        void update();

        std::string getName();
    };



}