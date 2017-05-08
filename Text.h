//
// Created by julie on 04/03/17.
//

#ifndef PROJET_TEXT_H
#define PROJET_TEXT_H


#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Text_Renderer.h"
#include "State_Manager.h"

class Text
{
public:
    std::string text;
    glm::vec2 Position;
    GLfloat Size_factor;
    glm::vec4 Color;

    Text(std::string t, glm::vec2 position, GLfloat size_factor = 1.0f, glm::vec4 color = glm::vec4(glm::vec3(0), 1));
    virtual void      Draw(State_Manager &manager, Text_Renderer &renderer);
};


#endif //PROJET_TEXT_H
