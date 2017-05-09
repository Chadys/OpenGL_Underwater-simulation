//
// Created by julie on 13/02/17.
//

#ifndef PROJET_SPRITE_RENDERER_H
#define PROJET_SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Texture.h"
#include "Shader.h"
#include "State_Manager.h"
#include "Camera.h"
#include "Post_Processor.h"

class Sprite_Renderer
{
public:
    // Constructor (inits shaders/shapes)
    Sprite_Renderer(const Shader &shader, GLfloat repeat);
    Sprite_Renderer(const Shader &shader);
    // Destructor
    virtual ~Sprite_Renderer();
    // Renders a defined quad textured with given sprite
    virtual void DrawSprite(State_Manager &manager, const Texture2D &normals, const Texture3D &skybox, glm::vec3 position, glm::vec2 size = glm::vec2(1.0f), glm::vec3 rotation = glm::vec3(0), glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
    virtual void DrawSprite(State_Manager &manager, const Texture2D &tex, glm::vec3 position, glm::vec2 size, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
    virtual void DrawSprite(State_Manager &manager, const Texture3D &texture, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
    virtual void DrawSprite(State_Manager &manager, const Framebuffer &framebuf, PostProd::POSTPROD_EFFECT effect);

private:
    // Render state
    Shader    shader;
    GLuint    quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
    void initRenderData(GLfloat repeat);
    void initPosRenderData();
};


#endif //PROJET_SPRITE_RENDERER_H
