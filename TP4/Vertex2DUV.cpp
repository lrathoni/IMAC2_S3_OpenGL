#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include "glimac/glm.hpp"
#include "glimac/Image.hpp"
#include <cstddef>
#include <cmath>

using namespace glimac;
using namespace glm;
using namespace std;


class Vertex2DUV {
    public:
        glm::vec2 position;
        glm::vec2 texture;

        Vertex2DUV() {};
        Vertex2DUV(glm::vec2 p_position, glm::vec2 p_texture) :
            position(p_position),
            texture(p_texture)
        {};
};


//Définition Matrices de transformation
    mat3 translate(float x, float y) {
        mat3 translate = mat3(vec3(1,0,0), vec3(0,1,0), vec3(x,y,1));
        return translate;
    }

    mat3 scale(float sx, float sy) {
        mat3 scale = mat3(vec3(sx,0,0), vec3(0,sy,0), vec3(0,0,1));
        return scale;
    }

    mat3 rotate(float a) {
        float b = radians(a);
        mat3 rotate = mat3( vec3(cos(b),sin(b),0), vec3(-sin(b),cos(b),0), vec3(0,0,1));
        return rotate;
    }

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
    unique_ptr<Image> triForce = loadImage("/media/line/Ubuntu/IMAC2/Synthèse d'image/GLImac-Template/assets/textures/triforce.png");

    //Checking
    if ( triForce == NULL) {
        cout << "The pointer is NULL, Program is over" << endl;
        return 0;
    }

    //Generate a texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, triForce->getWidth(), triForce->getHeight(),0,GL_RGBA,GL_FLOAT,triForce->getPixels());

    //filtres
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //débinder texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //Loading shaders
    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/" + argv[1],
                                    applicationPath.dirPath() + "shaders/" + argv[2]);
    program.use();

    // Definition locations variables uniformes
    // GLint uLocation = glGetUniformLocation(program.getGLId(), "uTime");
    // glUniform1f(uLocation, 45.0);

    GLint uLocation = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    GLint uLocationColor = glGetUniformLocation(program.getGLId(), "uColor");
    GLint uLocationTexture = glGetUniformLocation(program.getGLId(), "uTexture");

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    //vbo creation
    GLuint vbo;
    glGenBuffers(1, &vbo);

    //Binding vbo
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    Vertex2DUV vertices[] = {
        Vertex2DUV(glm::vec2(-1,-1), glm::vec2(0,1)),
        Vertex2DUV(glm::vec2(1,-1), glm::vec2(1,1)),
        Vertex2DUV(glm::vec2(0,1), glm::vec2(0.5,0))
    };

    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);

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

    const GLuint VERTEX_ATTR_TEXTURE = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    //rebinder le VBO
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*) offsetof(Vertex2DUV, position));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*) offsetof(Vertex2DUV, texture));

    glBindBuffer(GL_ARRAY_BUFFER,0);

    //debinder VAO
    glBindVertexArray(0);

    //def iterateur sur uTime
    float iter = 0;
    float iter2 = 0;
    float iterAll=0;


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
        
        iter +=1;
        iter2 -=1;
        iterAll+=0.5;
        mat3 reduceMatrix = scale(0.25, 0.25);
        mat3 rotateMatrix = rotate(iter);
        mat3 rotateinverseMatrix = rotate(iter2);
        mat3 rotateAllMatrix = rotate(iterAll);

        //glUniform1f(uLocation, iter );

        //Nettoyer la fenêtre
        glClear(GL_COLOR_BUFFER_BIT);

        //rebinder vao
        glBindVertexArray(vao);

        //dessiner sommet

        //color
        vec3 color1 = vec3(1,0.3,0.3);
        glUniform3fv(uLocationColor,1,value_ptr(color1));
        mat3 translateMatrix = translate(-0.5,0.5);
        
        //rotate etc
        glUniformMatrix3fv(uLocation, 1, GL_FALSE, value_ptr(rotateAllMatrix*translateMatrix*reduceMatrix*rotateMatrix));

        glBindTexture(GL_TEXTURE_2D,texture);
        glUniform1i(uLocationTexture,0);

        //draw one triangle
        glDrawArrays(GL_TRIANGLES,0,3);

        // same operations
        color1 = vec3(0,0.5,1);
        glUniform3fv(uLocationColor,1,value_ptr(color1));
        translateMatrix = translate(-0.5, -0.5);
        glUniformMatrix3fv(uLocation, 1, GL_FALSE, value_ptr(rotateAllMatrix*translateMatrix*reduceMatrix*rotateMatrix));
        glDrawArrays(GL_TRIANGLES,0,3);

        color1 = vec3(0.8,0.7,0.2);
        glUniform3fv(uLocationColor,1,value_ptr(color1));
        translateMatrix = translate(0.5, 0.5);
        glUniformMatrix3fv(uLocation, 1, GL_FALSE, value_ptr(rotateAllMatrix*translateMatrix*reduceMatrix*rotateinverseMatrix));
        glDrawArrays(GL_TRIANGLES,0,3);

        color1 = vec3(0.4,0.8,0.5);
        glUniform3fv(uLocationColor,1,value_ptr(color1));
        translateMatrix = translate(0.5, -0.5);
        glUniformMatrix3fv(uLocation, 1, GL_FALSE, value_ptr(rotateAllMatrix*translateMatrix*reduceMatrix*rotateinverseMatrix));
        glDrawArrays(GL_TRIANGLES,0,3);

        glBindTexture(GL_TEXTURE_2D,0);
        


        //débinder
        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }
    
    glDeleteTextures(1,&texture);
    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);


    return EXIT_SUCCESS;
}
