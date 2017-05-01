//
// Created by julie on 26/04/17.
//

#include "Model.h"
/*******************************************************************
** Taken and adapted from learnopengl.com (loading model tutorial)
******************************************************************/
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
// GL Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Model.h"
#include "Resource_Manager.h"

/*  Functions   */
Model::Model()
        : Span_lrf(0), Span_udb(0) { }
// Draws the model, and thus all its meshes
void Model::Draw(State_Manager &manager, Shader shader, glm::vec3 position, glm::vec3 size, glm::vec3 rotation,
                 glm::vec3 color, GLfloat alpha, bool outline, bool wings, float deformation_magnitude,
                 GLfloat whirlpooling, glm::vec3 centerpoint, glm::mat4 projection, glm::mat4 view)
{
    manager.tex2D = NO_TEX;
    manager.Active(shader);

    // Prepare transformations
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(centerpoint));
    model = glm::rotate(model, glm::radians(whirlpooling), glm::vec3(0.0f,1.0f,0.0f));
    model = glm::translate(model, glm::vec3(-centerpoint));

    model = glm::translate(model, glm::vec3(position));  // First translate (transformations are: scale happens first, then rotation and then final translation happens; reversed order)
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f,0.0f,0.0f)); // Then rotate
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f,1.0f,0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f,0.0f,1.0f));
    model = glm::scale(model, glm::vec3(size)); // Last scale
    shader.SetMatrix4("model", model);
    shader.SetMatrix3("transpose_inverse_viewmodel", glm::mat3(glm::transpose(glm::inverse(view * model))));
    shader.SetFloat("alpha", alpha);
    shader.SetVector3f("modelColor", color);
    shader.SetInteger("wings", wings);
    shader.SetFloat("magnitude", deformation_magnitude);
    shader.SetMatrix4("projection", projection);
    shader.SetMatrix4("view", view);

    for(GLuint i = 0; i < this->meshes.size(); i++)
        this->meshes[i].Draw(shader, outline);
}


/*  Model Data  */

vector<Mesh> meshes;
string directory;
vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

/*  Functions   */

// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode* node, const aiScene* scene)
{
    // Process each mesh located at the current node
    for(GLuint i = 0; i < node->mNumMeshes; i++) {
        // The node object only contains indices to index the actual objects in the scene.
        // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(this->processMesh(mesh, scene));
    }
    // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // Data to fill
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    // Process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // Walk through each of the mesh's vertices
    for(GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // Normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        if (material->GetTextureCount(aiTextureType_NORMALS)) {
            // Tangents
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
        }
        // Texture Coordinates
        if(mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);

        // Update the model' sides distance to the origin if needed
        Span_lrf.z = vertex.Position.x < Span_lrf.z ? vertex.Position.x : Span_lrf.z; //model is -x oriented so the min x is the front
        Span_udb.z = vertex.Position.x > Span_udb.z ? vertex.Position.x : Span_udb.z; //max x is the back
        Span_udb.y = vertex.Position.y < Span_udb.y ? vertex.Position.y : Span_udb.y; //min y is down
        Span_udb.x = vertex.Position.y > Span_udb.x ? vertex.Position.y : Span_udb.x; //max y is up
        Span_lrf.y = vertex.Position.z < Span_lrf.y ? vertex.Position.z : Span_lrf.y; //min z is right
        Span_lrf.x = vertex.Position.z > Span_lrf.x ? vertex.Position.z : Span_lrf.x; //max z is left

    }
    // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // Retrieve all indices of the face and store them in the indices vector
        for(GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // Diffuse: texture_diffuseN
    // Specular: texture_specularN
    // Normal: texture_normalN
    // 1. Diffuse maps
    vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. Specular maps
    vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. Emission maps
    vector<Texture> emissionMaps = this->loadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emissive");
    textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());
    // 4. Normal maps
    vector<Texture> heightMaps = this->loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    // 5. Normal maps
    vector<Texture> normalMaps = this->loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 6. Opacity maps
    vector<Texture> opacityMaps = this->loadMaterialTextures(material, aiTextureType_OPACITY, "texture_opacity");
    textures.insert(textures.end(), opacityMaps.begin(), opacityMaps.end());
    // }

    // Return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

// Checks all material textures of a given type and loads the textures if they're not loaded yet.
// The required info is returned as a Texture struct.
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        GLboolean skip = false;
        for(GLuint j = 0; j < textures_loaded.size(); j++)
        {
            if(textures_loaded[j].path == str)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {   // If texture hasn't been loaded already, load it
            Texture texture;
            string filename = string(str.C_Str());
            filename = this->directory + '/' + filename;
            texture.tex = ResourceManager::LoadTexture(filename.c_str(), GL_FALSE, GL_TRUE, typeName+std::to_string(i));
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
            this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}