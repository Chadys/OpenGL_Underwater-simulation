//
// Created by julie on 13/02/17.
//

#ifndef PROJET_STATE_MANAGER_H
#define PROJET_STATE_MANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Framebuffer.h"

// Active fading effect
enum Fade_Effect {
    NO_FADE,
    FADIN,
    FADOUT
};

class State_Manager{
public:
    GLuint              shader;
    GLuint              tex2D;
    unsigned short      tex3D;
    GLuint              framebuf;
    GLfloat 	        fade;
    Fade_Effect         Fade;
    State_Manager();
    void Active(Shader &_shader);
    virtual void ActiveTex2D(const Texture2D& texture);
    virtual void ActiveTex2D(const Framebuffer& framebuf);
    virtual void ActiveTex3D(const Texture3D& texture);
    virtual void ActiveFramebuf(const Framebuffer& buffer);
    virtual void DeactiveFramebuf();
    void Update(GLfloat dt);
};


#endif //PROJET_STATE_MANAGER_H
