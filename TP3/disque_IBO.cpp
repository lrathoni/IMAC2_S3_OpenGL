#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include "glimac/glm.hpp"
#include <cstddef>
#include <vector>

using namespace std;
using namespace glimac;

class Vertex2DColor {

        public :
        glm::vec2 position;
        glm::vec3 color;

        Vertex2DColor(){}
        Vertex2DColor( glm::vec2 p_position, glm::vec3 p_color ) :
            position(p_position),
            color(p_color)
         {}

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
    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/triangle.vs.glsl",
                                    applicationPath.dirPath() + "shaders/triangle.fs.glsl");
    program.use();

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
    float rayon = 0.5;
    uint32_t N = 100;
    float theta = ( 2 * glm::pi<float>() ) / N ;
    vector<Vertex2DColor> vertices;

    //on remplit vertices
    vertices.push_back( Vertex2DColor(glm::vec2( 0 , 0 ), glm::vec3(1, 1, 1)));
    for (float i = 0; i < N ; i++) {
        vertices.push_back( Vertex2DColor(glm::vec2( rayon * glm::cos( i * theta ) , rayon * glm::sin( i * theta ) ), glm::vec3(1, 1, 1)));
        //vertices.push_back( Vertex2DColor(glm::vec2( rayon * glm::cos( (i+1) * theta) , rayon * glm::sin( (i+1) * theta ) ), glm::vec3(1, 1, 1)));
    };


    glBufferData(GL_ARRAY_BUFFER, (N+1) * sizeof(Vertex2DColor), vertices.data(), GL_STATIC_DRAW);

    //Débinder
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //création IBO
    GLuint ibo;
    glGenBuffers(1,&ibo);

    //bindind ibo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    //Tableau d'indice
    vector<uint32_t> indices;
    for (uint32_t i = 0; i < N-1 ; i++) {
        indices.push_back(0);
        indices.push_back(i+1);
        indices.push_back(i+2);  
    }
    //dernier triangle
        indices.push_back(0);
        indices.push_back(N);
        indices.push_back(1); 


    // remplir ibo avec les indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    //debinding ibo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    // vao Creation
    GLuint vao;
    glGenVertexArrays(1, &vao);

    //Bindind vao
    //glBindersVertexArray(GL_ARRAY_BUFFER);
    glBindVertexArray(vao);

    //Enregistrer ibo dans vao
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);

    //Attributs activation
    //au lieu de faire ça glEnableVertexAttribArray(0);
    //To what corresponds the 0
    const GLuint VERTEX_ATTR_POSITION = 3;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    const GLuint VERTEX_ATTR_COLOR = 8;
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);


    //rebinder le VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);


    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*) offsetof(Vertex2DColor, position));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*) offsetof(Vertex2DColor, color));

    
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


        //dessiner sommet avec gldrawelement
        glDrawElements(GL_TRIANGLES, N*3, GL_UNSIGNED_INT, 0);

        //débinder
        glBindVertexArray(0);



        // Update the display
        windowManager.swapBuffers();
    }

    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);


    return EXIT_SUCCESS;
}
