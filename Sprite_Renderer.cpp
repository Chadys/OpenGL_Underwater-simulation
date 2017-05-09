//
// Created by julie on 13/02/17.
//

#include "Sprite_Renderer.h"


Sprite_Renderer::Sprite_Renderer(const Shader &shader, GLfloat repeat){
    this->shader = shader;
    if(repeat)
        this->initRenderData(repeat);
    else
        this->initPosRenderData();
}

Sprite_Renderer::Sprite_Renderer(const Shader &shader){
    this->shader = shader;
    this->initRenderData();
}


Sprite_Renderer::~Sprite_Renderer(){
    glDeleteVertexArrays(1, &this->quadVAO);
}

// SKYBOX
void Sprite_Renderer::initRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLfloat skyboxVertices[] = {
            // Positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// SQUARE
void Sprite_Renderer::initRenderData(GLfloat repeat)
{
    // Configure VAO/VBO
    GLuint VBO;

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLfloat vertices2d[] = {
            // Pos          // Tex
            -1.0f, -1.0f,   0.0f, 0.0f,
            1.0f, -1.0f,    repeat, 0.0f,
            -1.0f, 1.0f,    0.0f, repeat,
            1.0f, 1.0f,     repeat, repeat
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2d), &vertices2d, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// ONE POSITION
void Sprite_Renderer::initPosRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLfloat vertice[] = { 0.0f, 0.0f, 0.0f };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertice), &vertice, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// TEXTURED 3D PLANE
void Sprite_Renderer::DrawSprite(State_Manager &manager, const Texture2D &normals, const Texture3D &skybox, glm::vec3 position, glm::vec2 size, glm::vec3 rotation, glm::mat4 projection, glm::mat4 view)
{
    // Prepare transformations
    manager.Active(this->shader);
    glm::mat4 model;
    model = glm::translate(model, position);  // First translate (transformations are: scale happens first, then rotation and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); //to rotate around center, first translate the center to 0,0
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f,0.0f,0.0f)); // Then rotate
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f,1.0f,0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f,0.0f,1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); //then translate back (reversed order)

    model = glm::scale(model, glm::vec3(size, 1)); // Last scale

    this->shader.SetMatrix4("model", model);
    this->shader.SetMatrix4("view", view);
    this->shader.SetMatrix4("projection", projection);
    shader.SetMatrix3("transpose_inverse_viewmodel", glm::mat3(glm::transpose(glm::inverse(view * model))));

    glActiveTexture(GL_TEXTURE0);
    manager.ActiveTex2D(normals);
    glActiveTexture(GL_TEXTURE1);
    manager.ActiveTex3D(skybox);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// PARTICULE
void Sprite_Renderer::DrawSprite(State_Manager &manager, const Texture2D &tex, glm::vec3 position, glm::vec2 size, glm::mat4 projection, glm::mat4 view)
{
    //Prepare transformations
    manager.Active(this->shader);
    glm::mat4 model;
    model = glm::translate(model, position);

    this->shader.SetMatrix4("model", model);
    this->shader.SetMatrix4("view", view);
    this->shader.SetMatrix4("projection", projection);
    this->shader.SetVector2f("Billboard_Size", size);

    glActiveTexture(GL_TEXTURE0);
    manager.ActiveTex2D(tex);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
}

// SKYBOX
void Sprite_Renderer::DrawSprite(State_Manager &manager, const Texture3D &texture, glm::mat4 projection, glm::mat4 view)
{
    // Prepare transformations
    manager.Active(this->shader);

    glDepthFunc(GL_LEQUAL);
    view = glm::mat4(glm::mat3(view));
    this->shader.SetMatrix4("view", view);
    this->shader.SetMatrix4("projection", projection);

    glBindVertexArray(this->quadVAO);
    glActiveTexture(GL_TEXTURE0);
    manager.ActiveTex3D(texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}

void Sprite_Renderer::DrawSprite(State_Manager &manager, const Framebuffer &framebuf, PostProd::POSTPROD_EFFECT effect){
    manager.Active(this->shader);

    this->shader.SetInteger("effect", effect);

    glActiveTexture(GL_TEXTURE0);
    manager.ActiveTex2D(framebuf);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Sprite_Renderer::SwapShader(Shader &shader_) {
    if (this->shader.ID != shader_.ID)
        this->shader = shader_;
}