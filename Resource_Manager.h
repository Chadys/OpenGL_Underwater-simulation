//
// Created by julie on 14/02/17.
//

#ifndef PREPROJET_RESOURCE_MANAGER_H
#define PREPROJET_RESOURCE_MANAGER_H


#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include "Texture.h"
#include "Shader.h"
#include "Model.h"



// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined.
class ResourceManager
{
public:
    // Resource storage
    static std::map<std::string, Shader>     Shaders;
    static std::map<std::string, Texture2D>  Textures;
    static std::map<std::string, Texture3D>  Cubemaps;
    static std::map<std::string, Model>      Models;

    // Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader   LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name, std::vector<const GLchar *> common_shaders = {});
    // Retrieves a stored shader
    static Shader   GetShader(std::string name);
    // Loads (and generates) a texture from file
    static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, GLboolean repeat, std::string name);
    // Retrieves a stored texture
    static Texture2D GetTexture(std::string name);
    // Loads (and generates) a cubemaps from file
    static Texture3D LoadCubemap(std::vector<std::string> faces, std::string name);
    // Retrieves a stored cubemaps
    static Texture3D GetCubemap(std::string name);
    // Loads (and generates) a model from file
    static Model LoadModel(std::string file, std::string name);
    // Retrieves a stored model
    static Model GetModel(std::string name);
    // Properly de-allocates all loaded resources
    static void Clear();
private:
    // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    // Loads and generates a shader from file
    static Shader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr, std::vector<const GLchar *> common_shaders = {});
    // Loads a single texture from file
    static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha, GLboolean repeat);
    // Loads a single texture from file
    static Texture3D loadCubemapFromFile(std::vector<std::string> faces);
    // Loads a single model from file
    static Model loadModelFromFile(std::string file);
};

#endif //PREPROJET_RESOURCE_MANAGER_H
