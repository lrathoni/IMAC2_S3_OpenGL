#include <GL/glew.h>
#include "glm.hpp"

class TrackballCamera 
{
    private:
        float m_fDistance; //distance from the center of the scene
        float m_fAngleX; // Angle around the xAxis (rotation up/down)
        float m_fAngleY; // Angle around the yAxis (rotation right/left)

    public:
        // TrackballCamera(float fDistance, float fAgnleX, float fAngleY) :
        //     m_fDistance(fDistance),
        //     m_fAngleX(fAgnleX),
        //     m_fAngleY(fAngleY)
        //     {};

        TrackballCamera() :
            m_fDistance(5.f),
            m_fAngleX(0.f),
            m_fAngleY(0.f)
            {};

        float get_fDistance(){
            return this->m_fDistance;
        }
        
        float get_fAgnleX(){
            return this->m_fAngleX;
        }

        float get_fAgnleY(){
            return this->m_fAngleY;
        }

        void moveFront(float delta) {
            this->m_fDistance += -delta;
        }

        void rotateLeft(float degrees) {
            this->m_fAngleY += degrees;
        }
        
        void rotateUp(float degrees) {
            this->m_fAngleX += degrees;
        }

        glm::mat4 getViewMatrix() const {
            glm::mat4 ViewMatrix;  // Build normally a Id Matrix...
            ViewMatrix = glm::translate(ViewMatrix,glm::vec3(0.f,0.f,-this->m_fDistance));
            ViewMatrix = glm::rotate(ViewMatrix, glm::radians(this->m_fAngleX), glm::vec3(1.f,0.f,0.f));
            ViewMatrix = glm::rotate(ViewMatrix, glm::radians(this->m_fAngleY), glm::vec3(0.f,1.f,0.f));
            return ViewMatrix;
        }

};