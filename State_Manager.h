//
// Created by julie on 13/02/17.
//

#ifndef PREPROJET_STATE_MANAGER_H
#define PREPROJET_STATE_MANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"

enum Active_Shader {
    NO_SHADER,
    WORLD,
    SKYBOX,
    MODEL,
    TEXT,
    DEBUG
};

// The different texture possible
enum Active_Tex2D {
    NO_TEX,
    FLOOR,
    BG
};

// Active fading effect
enum Fade_Effect {
    NO_FADE,
    FADIN,
    FADOUT
};

class State_Manager{
public:
    Active_Shader  shader;
    Active_Tex2D   tex2D;
    GLboolean      tex3D;
    GLfloat 	   fade;
    Fade_Effect    Fade;
    State_Manager();
    void Active(Shader &_shader);
    virtual void ActiveTex2D(const Tex& texture);
    virtual void ActiveTex3D(const Tex& texture);
    void Update(GLfloat dt);
};


#endif //PREPROJET_STATE_MANAGER_H
