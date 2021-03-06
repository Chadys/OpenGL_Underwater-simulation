//
// Created by julie on 14/02/17.
//

#ifndef PROJET_SHADER_H
#define PROJET_SHADER_H


#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


// General purpsoe shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility
// functions for easy management.
class Shader
{
public:
    // State
    GLuint ID;
    // Constructor
    Shader() { }
    // Sets the current shader as active
    Shader  &Use();
    // Compiles the shader from given source code
    void    Compile          (const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr); // Note: geometry source code is optional
    // Utility functions
    void    SetFloat         (const GLchar *name, GLfloat value, GLboolean useShader = GL_FALSE);
    void    SetInteger       (const GLchar *name, GLint value, GLboolean useShader = GL_FALSE);
    void    SetVector2ui     (const GLchar *name, GLuint x, GLuint y, GLboolean useShader = GL_FALSE);
    void    SetVector2f      (const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = GL_FALSE);
    void    SetVector2f      (const GLchar *name, const glm::vec2 &value, GLboolean useShader = GL_FALSE);
    void    SetVector3f      (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = GL_FALSE);
    void    SetVector3f      (const GLchar *name, const glm::vec3 &value, GLboolean useShader = GL_FALSE);
    void    SetVector4f      (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = GL_FALSE);
    void    SetVector4f      (const GLchar *name, const glm::vec4 &value, GLboolean useShader = GL_FALSE);
    void    SetMatrix4       (const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = GL_FALSE);
    void    SetMatrix3       (const GLchar *name, const glm::mat3 &matrix, GLboolean useShader = GL_FALSE);
    void    SetFloatArray    (const GLchar *name, const GLfloat array[], unsigned int quantity, GLboolean useShader = GL_FALSE);
    void    SetVector2fArray (const GLchar *name, const GLfloat *array, unsigned int quantity, GLboolean useShader = GL_FALSE);
private:
    // Checks if compilation or linking failed and if so, print the error logs
    void    checkCompileErrors(GLuint object, std::string type);
};


#endif //PROJET_SHADER_H
