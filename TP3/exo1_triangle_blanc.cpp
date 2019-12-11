#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>

using namespace std;
using namespace glimac;

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // vbo creation
    GLuint vbo;
    glGenBuffers(1, &vbo);

    //Bindind vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //Coordonates
    GLfloat vertices[] = { -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f };
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    //Débinder
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vao Creation
    GLuint vao;
    glGenVertexArrays(1, &vao);

    //Bindind vao
    //glBindersVertexArray(GL_ARRAY_BUFFER);
    glBindVertexArray(vao);

    //Attributs activation
    //au lieu de faire ça glEnableVertexAttribArray(0);
    //To what corresponds the 0
    const GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    //rebinder le VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    //débinder VAO
    glBindVertexArray(0);


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

        //Nettoyer la fenêtre
        glClear(GL_COLOR_BUFFER_BIT);

        //rebinder vao
        glBindVertexArray(vao);

        //dessiner sommet
        glDrawArrays(GL_TRIANGLES,0,3);

        //débinder
        glBindVertexArray(0);



        // Update the display
        windowManager.swapBuffers();
    }

    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);


    return EXIT_SUCCESS;
}
