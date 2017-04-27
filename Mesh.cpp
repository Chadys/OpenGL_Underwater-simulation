//
// Created by julie on 26/04/17.
//

#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures){
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // Now that we have all the required data, set the vertex buffers and its attribute pointers.
    this->setupMesh();
}

void Mesh::Draw(Shader shader, glm::vec3 position, glm::vec3 size, glm::vec3 rotation,
                glm::vec3 color, GLfloat alpha, bool outline, glm::mat4 projection, glm::mat4 view){
    // Bind appropriate textures
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
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
            ss << specularNr++; // Transfer GLuint to stream
        number = ss.str();
        // Now set the sampler to the correct texture unit
        shader.SetInteger((name + number).c_str(), i);
        // And finally bind the texture
        this->textures[i].tex.Bind();
    }

    // Prepare transformations
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position));  // First translate (transformations are: scale happens first, then rotation and then final translation happens; reversed order)
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f,0.0f,0.0f)); // Then rotate
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f,1.0f,0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f,0.0f,1.0f));
    model = glm::scale(model, glm::vec3(size)); // Last scale

    // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
    shader.SetFloat("shininess", 16.0f);
    shader.SetFloat("alpha", alpha);
    shader.SetVector3f("modelColor", color);
    shader.SetMatrix4("projection", projection);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("model", model);
    // Draw mesh
    glBindVertexArray(this->VAO);
    shader.SetInteger("back", GL_FALSE);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    if(outline) {
        //Back face for contouring
        shader.SetInteger("back", GL_TRUE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glCullFace(GL_FRONT);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);

    // Always good practice to set everything back to defaults once configured.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCullFace(GL_BACK);
    for (GLuint i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
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
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}