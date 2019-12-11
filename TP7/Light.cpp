#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include "glimac/glm.hpp"
#include "glimac/Sphere.hpp"
#include "glimac/common.hpp"
#include "glimac/Image.hpp"
#include <vector>
#include "glimac/FreeflyCamera.hpp"


using namespace glimac;
using namespace glm;
using namespace std;
using namespace detail;

struct SceneProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    SceneProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/directionallight.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
    }
};

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    //Creation FreeflyCamera

    FreeflyCamera camera = FreeflyCamera();
    camera.initializeAttrib();

    //Loading shaders
    FilePath applicationPath(argv[0]);
    SceneProgram SceneProgram(applicationPath);

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    //definition locations variables uniformes
    GLuint uKd;
    GLuint uKs;
    GLuint uShininess;
    GLuint uLightDir_vs;
    GLuint uLightIntensity;
    uKd = glGetUniformLocation(SceneProgram.m_Program.getGLId(), "uKd");
    uKs = glGetUniformLocation(SceneProgram.m_Program.getGLId(), "uKs");
    uShininess = glGetUniformLocation(SceneProgram.m_Program.getGLId(), "uShininess");
    uLightDir_vs = glGetUniformLocation(SceneProgram.m_Program.getGLId(), "uLightDir_vs");
    uLightIntensity = glGetUniformLocation(SceneProgram.m_Program.getGLId(), "uLightIntensity");

    // GPU checks depth
    glEnable(GL_DEPTH_TEST);
 
    mat4 ProjMatrix = perspective(radians(70.f),800.f/600.f,0.1f, 100.f);

    mat4 MVMatrix = camera.getViewMatrix();

                            //     cout << "MVMatrix " << endl; 
                            // for (int i=0; i<4; i++) {
                            //     for (int j=0; j<4 ; j++) {
                            //         cout << MVMatrix[i][j] << endl;
                            //     }
                            // }

    // To check the ViewMatrix of the Camera
    // for (int i=0; i<4; i++) {
    //     for (int j=0; j<4 ; j++) {
    //         cout << MVMatrix[i][j] << endl;
    //     }
    // }

    mat4 NormalMatrix = transpose(inverse(MVMatrix));



    //Initialisation sphere
    Sphere sphere(1, 16, 16);



    //vbo creation
    GLuint vbo;
    glGenBuffers(1, &vbo);

    //Binding vbo
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof(ShapeVertex), sphere.getDataPointer(), GL_STATIC_DRAW);

    //débinder
    glBindBuffer(GL_ARRAY_BUFFER,0);

    //vao creation
    GLuint vao;
    glGenVertexArrays(1,&vao);

    //Binding vao
    glBindVertexArray(vao);

    //attributs activation
    const GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    const GLuint VERTEX_ATTR_NORMAL = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);

    const GLuint VERTEX_ATTR_TEXTURE = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    //rebinder le VBO
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*) offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*) offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*) offsetof(ShapeVertex, texCoords));

    //debinder VAO
    glBindVertexArray(0);

    //Definition of 32 rotation axis
    vector<vec3> RandomAxis;
    for (int i=0; i<32; i++) {
        RandomAxis.push_back(sphericalRand(5.f));
        //InitPosition.push_back(translate(mat4(),vec3(linearRand(-5.0f, 5.0f),linearRand(-5.0f, 5.0f),linearRand(-5.0f, 5.0f))));
    }

    vector<vec3> RandomKd;
    for (int i=0; i<32; i++) {
        RandomKd.push_back(vec3(linearRand (0.0,1.0),linearRand (0.0,1.0),linearRand (0.0,1.0)));
    }

    vector<vec3> RandomKs;
    for (int i=0; i<32; i++) {
        RandomKs.push_back(vec3(linearRand (0.0,1.0),linearRand (0.0,1.0),linearRand (0.0,1.0)));
    }

    vector<float> RandomShininess;
    for (int i=0; i<32; i++) {
        RandomShininess.push_back(linearRand (0.0,3.0));
    }

    vec3 Kd = vec3(linearRand (0.0,1.0),linearRand (0.0,1.0),linearRand (0.0,1.0));
    vec3 Ks = vec3(linearRand (0.0,1.0),linearRand (0.0,1.0),linearRand (0.0,1.0));
    float Shininess = linearRand (0.0,3.0);
    vec3 LightDir= vec3(1.0,1.0,1.0);
    vec3 LightIntensity = vec3(0.5,0.5,0.5);

    int mouseX, mouseY;
    bool mouseDown;

    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {

            switch(e.type) {

                case SDL_QUIT:
                    done = true; // Leave the loop after this iteration
                    break;


                /* Clic souris */
                case SDL_MOUSEBUTTONDOWN:
                    mouseY = e.button.y;
                    mouseX = e.button.x;
                    mouseDown = true;
                    break;
                
                case SDL_MOUSEBUTTONUP:
                    mouseDown = false;
                    break;
            
                case SDL_MOUSEMOTION:                
                    if (mouseDown) {
                        camera.rotateUp(e.button.x - mouseX);
                        camera.rotateLeft(e.button.y - mouseY);
                        camera.showPosition();
                        mouseX = e.button.x;
                        mouseY = e.button.y;
                    }
                    break;

                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_z) {
                        camera.moveFront(5.f);
                    }
                    if (e.key.keysym.sym == SDLK_s) {
                        camera.moveFront(-5.f);
                    }
                    if (e.key.keysym.sym == SDLK_q) {
                        camera.moveLeft(1.f);
                    }
                    if (e.key.keysym.sym == SDLK_d) {
                        camera.moveLeft(-1.f);
                    }
                    break;

                case SDL_KEYUP:
                    //std::cout << "touche levée (code = "<< e.key.keysym.sym << ")" << std::endl;
                    break;
            }
        }


        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        //Clear the window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.initializeAttrib();
        MVMatrix = camera.getViewMatrix();

        //rebinder vao
        glBindVertexArray(vao);

        SceneProgram.m_Program.use();

        //turn light
        LightDir = rotateX(LightDir, windowManager.getTime()*0.005f);

        glUniform3fv( uKd, 1, value_ptr(Kd));
        glUniform3fv( uKs, 1, value_ptr(Ks));
        glUniform1f( uShininess, Shininess);
        glUniform3fv(uLightDir_vs, 1, value_ptr(mat3(camera.getViewMatrix())*LightDir));
        glUniform3fv(uLightIntensity, 1, value_ptr(LightIntensity));

        //Draw Earth
        MVMatrix = rotate(MVMatrix, windowManager.getTime(), vec3(0, 1, 0));
        glUniformMatrix4fv(SceneProgram.uMVMatrix, 1, GL_FALSE, value_ptr(MVMatrix));
        glUniformMatrix4fv(SceneProgram.uNormalMatrix, 1, GL_FALSE, value_ptr(transpose(inverse(MVMatrix))));
        glUniformMatrix4fv(SceneProgram.uMVPMatrix, 1, GL_FALSE, value_ptr(ProjMatrix * MVMatrix));

        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        //Draw Satellite
        for (int i=0; i<32; i++) {
            glUniform3fv( uKd, 1, value_ptr(RandomKd[i]));
            glUniform3fv( uKs, 1, value_ptr(RandomKs[i]));
            glUniform1f( uShininess, RandomShininess[i]);
            MVMatrix = camera.getViewMatrix();
            MVMatrix = rotate(MVMatrix, windowManager.getTime(), RandomAxis[i]); // Rotation
            MVMatrix = translate(MVMatrix, vec3(-2, 0, 0)); // Rotation * Translation
            MVMatrix = scale(MVMatrix, vec3(0.2, 0.2, 0.2)); // Rotation * Translation * Scale
            glUniformMatrix4fv(SceneProgram.uMVPMatrix, 1, GL_FALSE , value_ptr(ProjMatrix*MVMatrix));
            glUniformMatrix4fv(SceneProgram.uMVMatrix, 1,  GL_FALSE, value_ptr(MVMatrix));
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        }

        //débinder vao
        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }
    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);

    return EXIT_SUCCESS;
}
