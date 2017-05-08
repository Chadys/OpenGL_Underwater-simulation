//
// Created by julie on 08/05/17.
//

#ifndef UNDER_THE_SEA_FRAMEBUFFER_H
#define UNDER_THE_SEA_FRAMEBUFFER_H

#include <GL/glew.h>

class Framebuffer {
public:
    GLuint fbo;
    GLuint texColorBuffer;
    GLuint rbo;

    void Generate(GLuint width, GLuint height);
    void Bind() const;
    void BindTex() const;

private:
};


#endif //UNDER_THE_SEA_FRAMEBUFFER_H
