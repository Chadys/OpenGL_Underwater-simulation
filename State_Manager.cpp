//
// Created by julie on 13/02/17.
//

#include "State_Manager.h"
State_Manager::State_Manager() : shader(NO_SHADER), tex2D(NO_TEX), tex3D(0), fade(0), Fade(FADIN) { }

void State_Manager::Active(Shader &_shader){
    if (this->shader != _shader.ID/3){
        this->shader = static_cast<Active_Shader>(_shader.ID/3);
        _shader.Use();
    }
    _shader.SetFloat("fade", this->fade);
}

void State_Manager::ActiveTex2D(const Texture2D& texture){
    if (this->tex2D != texture.ID/2+1){
        this->tex2D = static_cast<Active_Tex2D>(texture.ID/2+1);
        texture.Bind();
    }
}

void State_Manager::ActiveTex3D(const Texture3D& texture){
    if (this->tex3D < 2){
        this->tex3D++;
        texture.Bind();
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