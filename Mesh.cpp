//
// Created by julie on 26/04/17.
//

#include "Mesh.h"

Mesh::Mesh(vector<Vertex> &vertices, vector<GLuint> &indices, vector<Texture> &textures){
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // Now that we have all the required data, set the vertex buffers and its attribute pointers.
    this->setupMesh();
}

void Mesh::Draw(Shader shader, bool outline){
    // Bind appropriate textures
    GLuint diffuseNr = 1, specularNr = 1, emissiveNr = 1, normalNr = 1, heightNr = 1, opacityNr = 1;
    vector<string> reset_tex;
    for(GLuint i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Retrieve texture number (the N in diffuse_textureN)
        std::stringstream ss;
        string number;
        string name = this->textures[i].type;
        if(name == "texture_diffuse")
            ss << diffuseNr++; // Transfer GLuint to stream
        else if(name == "texture_specular")
            ss << specularNr++;
        else if(name == "texture_emissive")
            ss << emissiveNr++;
        else if(name == "texture_normal")
            ss << normalNr++;
        else if(name == "texture_height")
            ss << heightNr++;
        else if(name == "texture_opacity")
            ss << opacityNr++;
        number = ss.str();
        // Now set the sampler to the correct texture unit
        reset_tex.push_back("material."+name);
        shader.SetInteger((reset_tex.back() + number).c_str(), i);
        shader.SetInteger(reset_tex.back().c_str(), GL_TRUE);
        // And finally bind the texture
        this->textures[i].tex.Bind();
    }


    // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
    shader.SetFloat("material.shininess", 8.0f);
    // Draw mesh
    glBindVertexArray(this->VAO);
    shader.SetInteger("back", GL_FALSE);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
//    if(outline) {
//        //Back face for contouring
//        shader.SetInteger("back", GL_TRUE);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//        glCullFace(GL_FRONT);
//        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//        glCullFace(GL_BACK);
//    }
    glBindVertexArray(0);
    for (GLuint i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    for (const string &tex_bool : reset_tex)
        shader.SetInteger(tex_bool.c_str(), GL_FALSE);
}

void Mesh::setupMesh(){
    // Create buffers/arrays
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    // Vertex Tangents
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));
    // Vertex Texture Coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}