//
// Created by julie on 08/05/17.
//

#include "Framebuffer.h"
#include <iostream>

void Framebuffer::Generate(GLuint width, GLuint height) {
    glGenFramebuffers(1, &(this->fbo));
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    // Generate texture to use as color output
    glGenTextures(1, &(this->texColorBuffer));
    glBindTexture(GL_TEXTURE_2D, this->texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texColorBuffer, 0);

    // Generate render buffer object to use as depth and stencil testing
    glGenRenderbuffers(1, &(this->rbo));
    glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // Attach it to currently bound framebuffer object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "ERROR::FRAMEBUFFER " << "Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind() const{
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
}

void Framebuffer::BindTex() const{
    glBindTexture(GL_TEXTURE_2D, this->texColorBuffer);
}