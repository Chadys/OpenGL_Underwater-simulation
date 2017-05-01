//
// Created by julie on 26/04/17.
//
/*******************************************************************
** Taken and adapted from learnopengl.com (loading model tutorial)
******************************************************************/

#ifndef PROJET_MODEL_H
#define PROJET_MODEL_H


// Std. Includes
#include <string>
#include <vector>
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <assimp/scene.h>

#include "Mesh.h"
#include "State_Manager.h"

class Model
{
    friend class ResourceManager;
public:
    /*  Model Data  */
    glm::vec3   Span_lrf, Span_udb;		//distance between each side of the model and its origin (left,right, front and up,down,back)

    // Constructor.
    Model();

    // Draws the model, and thus all its meshes
    void Draw(State_Manager &manager, Shader shader, glm::vec3 position, glm::vec3 size, glm::vec3 rotation,
              glm::vec3 color, GLfloat alpha, bool outline, bool wings, float deformation_magnitude,
              GLfloat whirlpooling, glm::vec3 centerpoint, glm::mat4 projection, glm::mat4 view);

private:
    /*  Model Data  */
    vector<Mesh>	meshes;
    string			directory;
    vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

    /*  Functions   */

    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // Checks all material textures of a given type and loads the textures if they're not loaded yet.
    // The required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};


#endif //PROJET_MODEL_H
