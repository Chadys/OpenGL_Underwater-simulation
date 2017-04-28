//
// Created by julie on 26/04/17.
//
/*******************************************************************
** Taken and adapted from learnopengl.com (loading model tutorial)
******************************************************************/

#ifndef PROJET_MESH_H
#define PROJET_MESH_H


// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/types.h>
#include "Texture.h"
#include "Shader.h"

using std::string;
using std::vector;


struct Vertex {
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // TexCoords
    glm::vec2 TexCoords;
};

struct Texture {
    Texture2D tex;
    string type;
    aiString path;
};

class Mesh {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    /*  Functions  */
    // Constructor
    Mesh(vector<Vertex> &vertices, vector<GLuint> &indices, vector<Texture> &textures);

    // Render the mesh
    void Draw(Shader shader, glm::vec3 position, glm::vec3 size, glm::vec3 rotation,
              glm::vec3 color, GLfloat alpha, bool outline, glm::mat4 projection, glm::mat4 view);

private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;

    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void setupMesh();
};


#endif //PROJET_MESH_H
