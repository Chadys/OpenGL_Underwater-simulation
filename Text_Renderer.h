//
// Created by julie on 04/03/17.
//

#ifndef PREPROJET_TEXT_RENDERER_H
#define PREPROJET_TEXT_RENDERER_H


#include <map>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Texture.h"
#include "Shader.h"
#include "State_Manager.h"
#include "Resource_Manager.h"


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
};


// A renderer class for rendering text displayed by a font loaded using the
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class Text_Renderer
{
public:
    // Holds a list of pre-compiled Characters
    std::map<GLchar, Character> Characters;
    // Shader used for text rendering
    Shader TextShader;
    // Constructor
    Text_Renderer(GLuint width, GLuint height);
    // Pre-compiles a list of characters from the given font
    void Load(std::string font, GLuint fontSize);
    // Renders a string of text using the precompiled list of characters
    void RenderText(State_Manager &manager, std::string text, GLfloat x, GLfloat y, GLfloat scale = 1.0f, glm::vec4 color = glm::vec4(glm::vec3(0), 1));
private:
    // Render state
    GLuint VAO, VBO;
};

#endif //PREPROJET_TEXT_RENDERER_H
