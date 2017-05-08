//
// Created by julie on 13/02/17.
//

#include "State_Manager.h"
State_Manager::State_Manager() : shader(0), tex2D(0), tex3D(0), framebuf(0), fade(0), Fade(FADIN) { }

void State_Manager::Active(Shader &_shader){
    if (this->shader != _shader.ID){
        this->shader = _shader.ID;
        _shader.Use();
    }
    _shader.SetFloat("fade", this->fade);
}

void State_Manager::ActiveTex2D(const Texture2D& texture){
    if (this->tex2D != texture.ID){
        this->tex2D = texture.ID;
        texture.Bind();
    }
}

void State_Manager::ActiveTex2D(const Framebuffer& framebuf){
    if (this->tex2D != framebuf.texColorBuffer){
        this->tex2D = framebuf.texColorBuffer;
        framebuf.BindTex();
    }
}

void State_Manager::ActiveTex3D(const Texture3D& texture){
    if (this->tex3D < 2){
        this->tex3D++;
        texture.Bind();
    }
}

void State_Manager::ActiveFramebuf(const Framebuffer &buffer) {
    if (this->framebuf != buffer.fbo){
        this->framebuf = buffer.fbo;
        buffer.Bind();
    }
}

void State_Manager::DeactiveFramebuf() {
    if (this->framebuf != 0){
        this->framebuf = 0;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void State_Manager::Update(GLfloat dt){
    if (this->Fade == FADIN){
        this->fade += dt;
        if (this->fade > 1){
            this->fade = 1;
            this->Fade = NO_FADE;
        }
    }
    if (this->Fade == FADOUT){
        this->fade -= dt;
        if (this->fade < 0){
            this->fade = 0;
            this->Fade = FADIN;
        }
    }
}