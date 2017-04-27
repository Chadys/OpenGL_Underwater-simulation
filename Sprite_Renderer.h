//
// Created by julie on 13/02/17.
//

#ifndef PREPROJET_SPRITE_RENDERER_H
#define PREPROJET_SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Texture.h"
#include "Shader.h"
#include "State_Manager.h"
#include "Camera.h"

enum Effect {
    NONE,
    RADIAL_GRAD,
    V_GRAD,
    H_GRAD
};

class Sprite_Renderer
{
public:
    // Constructor (inits shaders/shapes)
    Sprite_Renderer(const Shader &shader, GLfloat repeat);
    Sprite_Renderer(const Shader &shader);
    // Destructor
    virtual ~Sprite_Renderer();
    // Renders a defined quad textured with given sprite
    virtual void DrawSprite(State_Manager &manager, const Tex &texture, GLboolean &destroy, glm::vec3 position, glm::vec2 size = glm::vec2(1.0f), glm::vec3 rotation = glm::vec3(0), GLboolean billboard = GL_FALSE, glm::vec2 deplace = glm::vec2(0), glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
    virtual void DrawSprite(State_Manager &manager, glm::vec2 position, glm::vec2 size, GLboolean isCircle, glm::vec3 color1, glm::vec3 color2, Effect effect, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4(), GLboolean border = GL_FALSE);
    virtual void DrawSprite(State_Manager &manager, const Tex &texture, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());

private:
    // Render state
    Shader    shader;
    GLuint    quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
    void initRenderData(GLfloat repeat);
};


#endif //PREPROJET_SPRITE_RENDERER_H
