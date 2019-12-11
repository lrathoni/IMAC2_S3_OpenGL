#include <GL/glew.h>
#include "glm.hpp"
#include <cmath>

class FreeflyCamera {
    private:
        glm::vec3 m_Position; //eye
        float m_fPhi;
        float m_fTheta;

        //Vectors found with the third first attributes
        glm::vec3 m_FrontVector;
        glm::vec3 m_LeftVector; // V = P*F
        glm::vec3 m_UpVector; // U = F*L

        //Private method which calculate the three vectors up
        void computeDirectionVectors() {
            this->m_FrontVector = this->m_Position; //?? C'est vraiment ça?
            glm::vec3 F = glm::vec3( glm::cos(this->m_fTheta) * glm::sin(this->m_fPhi), glm::sin(this->m_fTheta) , glm::cos(this->m_fTheta)*glm::sin(this->m_fPhi));
            //std:: cout << "F = " << F << std::endl;
            this->m_LeftVector = this->m_Position + F;
            glm::vec3 L = glm::vec3( glm::sin(this->m_fTheta + (glm::pi<float>())/2.f), 0.f, glm::cos(this->m_fTheta + (glm::pi<float>())/2.f));
            //std:: cout << "L = " << L << std::endl;
            this->m_UpVector = glm::cross(F,L);
        }
    
    public:
        FreeflyCamera() : 
        // à changer, c'est juste pour mettre en place les bouttons
            m_Position(glm::vec3(0.f,0.f,0.f)),
            //Convetion --> F = vec3(0,0,-1)
            m_fPhi(glm::pi<float>()),
            m_fTheta(0.f)
            {}

        void showPosition() {
            std::cout << "Position = " << this->m_Position << std::endl;
            std::cout << "Theta = " << this->m_fTheta << std::endl;
            std::cout << "Phi = " << this->m_fPhi << std::endl;
        }

        void initializeAttrib() {
            computeDirectionVectors();
        }
        
        void moveLeft(float t) {
            this->m_Position += t * glm::vec3( glm::sin(this->m_fTheta + (glm::pi<float>())/2.f), 0.f, glm::cos(this->m_fTheta + (glm::pi<float>())/2.f));
        }

        void moveFront(float t) {
            this->m_Position += t * glm::vec3( glm::cos(this->m_fTheta) * glm::sin(this->m_fPhi), glm::sin(this->m_fTheta) , glm::cos(this->m_fTheta)*glm::sin(this->m_fPhi));
        }

        void rotateLeft(float degrees) {
            this->m_fPhi += glm::radians(degrees);
        }

        void rotateUp(float degrees) {
            this->m_fTheta += glm::radians(degrees);
        }

        glm::mat4 getViewMatrix() const {
            return glm::lookAt(this->m_Position, this->m_LeftVector, this->m_UpVector);
        }

};