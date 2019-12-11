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

using namespace glimac;
using namespace glm;
using namespace std;
using namespace detail;

struct EarthProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uCloudTexture");
    }
};

struct MoonProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    MoonProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/text3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
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

    //Download image
    unique_ptr<Image> earth = loadImage("/media/line/Ubuntu/IMAC2/Synthèse d'image/GLImac-Template/assets/textures/EarthMap.jpg");
    unique_ptr<Image> moon = loadImage("/media/line/Ubuntu/IMAC2/Synthèse d'image/GLImac-Template/assets/textures/MoonMap.jpg");
    unique_ptr<Image> cloud = loadImage("/media/line/Ubuntu/IMAC2/Synthèse d'image/GLImac-Template/assets/textures/CloudMap.jpg");

    //Checking
    if ( earth == NULL or moon == NULL or cloud == NULL) {
        cout << "The pointer is NULL, Program is over" << endl;
        return 0;
    }
    

    //Loading shaders
    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/" + argv[1],
                                    applicationPath.dirPath() + "shaders/" + argv[2]);
    program.use();

    Program programtext = loadProgram(applicationPath.dirPath() + "shaders/" + argv[1],
                                        applicationPath.dirPath() + "shaders/" + argv[3]);
    programtext.use();


    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    //definition locations variables uniformes

    //program (de base)
    GLint uLocationMVP = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLint uLocationMV = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    GLint uLocationNormal = glGetUniformLocation(program.getGLId(), "uNormalMatrix");

    //programtext
    GLint uLocationTexture = glGetUniformLocation(programtext.getGLId(), "uTexture");
    GLint uLocationTextureCloud = glGetUniformLocation(programtext.getGLId(), "uTextureCloud");

    //Generate a texture

    GLuint SphereTexture;
    GLuint MoonTexture;
    GLuint CloudTexture;
    glGenTextures(1, &CloudTexture);
    glGenTextures(1, &SphereTexture);
    glGenTextures(1, &MoonTexture);
    
    glBindTexture(GL_TEXTURE_2D, SphereTexture); // la texture SphereTexture est bindée sur l'unité GL_TEXTURE
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, earth->getWidth(), earth->getHeight(),0,GL_RGBA,GL_FLOAT,earth->getPixels());
        //filtres
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, CloudTexture);
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, cloud->getWidth(), cloud->getHeight(),0,GL_RGBA,GL_FLOAT,cloud->getPixels());
            //filtres
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, MoonTexture);
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, moon->getWidth(), moon->getHeight(),0,GL_RGBA,GL_FLOAT,moon->getPixels());
            //filtres
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // GPU checks depth
    glEnable(GL_DEPTH_TEST);

    mat4 ProjMatrix = perspective(radians(70.f),800.f/600.f,0.1f, 100.f);

    mat4 MVMatrix = translate(mat4(),vec3(0.f,0.f,-5.f));

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
    //Dire à Celine que je n'arrive pas à faire marcher sphericalRand
    vector<mat4> InitPosition;
    vector<vec3> RandomAxis;
    for (int i=0; i<32; i++) {
        RandomAxis.push_back(sphericalRand(5.f));
        //InitPosition.push_back(translate(mat4(),vec3(linearRand(-5.0f, 5.0f),linearRand(-5.0f, 5.0f),linearRand(-5.0f, 5.0f))));
    }

    //vec3 p = sphericalRand(5.f);

    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        //Clear the window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        MVMatrix = translate(mat4(),vec3(0.f,0.f,-5.f));

        glUniformMatrix4fv(uLocationMVP, 1, GL_FALSE , value_ptr(ProjMatrix*MVMatrix));
        glUniformMatrix4fv(uLocationMV, 1,  GL_FALSE, value_ptr(MVMatrix));
        glUniformMatrix4fv(uLocationNormal, 1,  GL_FALSE, value_ptr(NormalMatrix));


        //cout << "je suis là" << endl;
        
        //Binder earth
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, SphereTexture); // la texture SphereTexture est bindée sur l'unité GL_TEXTURE
        //glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, earth->getWidth(), earth->getHeight(),0,GL_RGBA,GL_FLOAT,earth->getPixels());

        //Binder Cloud
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, CloudTexture);
        //glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, cloud->getWidth(), cloud->getHeight(),0,GL_RGBA,GL_FLOAT,cloud->getPixels());


        //rebinder vao
        glBindVertexArray(vao);

        glUniform1i(uLocationTexture,0);
        glUniform1i(uLocationTextureCloud,1);

        // EARTH
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, SphereTexture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, CloudTexture);

        MVMatrix = rotate(MVMatrix, windowManager.getTime()/2, vec3(0, 1, 0));
        glUniformMatrix4fv(uLocationMVP, 1, GL_FALSE , value_ptr(ProjMatrix*MVMatrix));
        glUniformMatrix4fv(uLocationMV, 1,  GL_FALSE, value_ptr(MVMatrix));
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);


        // MOON

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, MoonTexture);

        //Binder Moon

        for (int i=0; i<32; i++) {
            
            MVMatrix = translate(mat4(),vec3(0.f,0.f,-5.f));
            MVMatrix = rotate(MVMatrix, windowManager.getTime(), RandomAxis[i]); // Translation * Rotation
            MVMatrix = translate(MVMatrix, vec3(-2, 0, 0)); // Translation * Rotation * Translation
            MVMatrix = scale(MVMatrix, vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
            glUniformMatrix4fv(uLocationMVP, 1, GL_FALSE , value_ptr(ProjMatrix*MVMatrix));
            glUniformMatrix4fv(uLocationMV, 1,  GL_FALSE, value_ptr(MVMatrix));
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        //débinder vao
        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

    glDeleteTextures(1, &MoonTexture);
    glDeleteTextures(1, &SphereTexture);
    glDeleteTextures(1, &CloudTexture);
    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);

    return EXIT_SUCCESS;
}
