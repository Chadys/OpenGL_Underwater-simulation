//
// Created by julie on 14/02/17.
//

#include "Resource_Manager.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Texture3D>    ResourceManager::Cubemaps;
std::map<std::string, Shader>       ResourceManager::Shaders;
std::map<std::string, Model>        ResourceManager::Models;
std::map<std::string, Framebuffer>  ResourceManager::Framebuffers;
irrklang::ISoundEngine *ResourceManager::SoundEngine = irrklang::createIrrKlangDevice();


Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name, std::vector<const GLchar *> common_shaders)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile, common_shaders);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, GLboolean repeat, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha, repeat);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

Texture3D ResourceManager::LoadCubemap(std::vector<std::string> faces, std::string name)
{
    Cubemaps[name] = loadCubemapFromFile(faces);
    return Cubemaps[name];
}

Texture3D ResourceManager::GetCubemap(std::string name)
{
    return Cubemaps[name];
}

Model ResourceManager::LoadModel(std::string file, std::string name)
{
    Models[name] = loadModelFromFile(file);
    return Models[name];
}

Model ResourceManager::GetModel(std::string name)
{
    return Models[name];
}

Framebuffer ResourceManager::LoadFramebuffer(GLuint width, GLuint height, std::string name) {
    Framebuffer fb;
    fb.Generate(width, height);
    Framebuffers[name] = fb;
    return Framebuffers[name];
}

Framebuffer ResourceManager::GetFramebuffer(std::string name) {
    return Framebuffers[name];
}

void ResourceManager::Clear()
{
    // (Properly) delete all shaders
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (Properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
    // (Properly) delete all cubemaps
    for (auto iter : Cubemaps)
        glDeleteTextures(1, &iter.second.ID);
    // (Properly) delete all framebuffers
    for (auto iter : Framebuffers)
        glDeleteFramebuffers(1, &iter.second.fbo);
    SoundEngine->drop();
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::vector<const GLchar *> common_shaders)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode, fragmentCode, geometryCode;
    try
    {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream, gShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
        }
        // Deal with common shaders
        for (const GLchar *cShaderFile : common_shaders){
            std::ifstream commonShaderFile(cShaderFile);
            char type = cShaderFile[strlen(cShaderFile)-2];
            switch(type){
                case 'f':
                    fShaderStream << commonShaderFile.rdbuf();
                    break;
                case 'v':
                    vShaderStream << commonShaderFile.rdbuf();
                    break;
                default:
                    gShaderStream << commonShaderFile.rdbuf();
            }
            commonShaderFile.close();
        }
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if (gShaderFile != nullptr)
            geometryCode = gShaderStream.str();
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    const GLchar *gShaderCode = geometryCode.c_str();
    // 2. Now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha, GLboolean repeat)
{
    // Create Texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    if (repeat)
    {
        texture.Wrap_S = GL_REPEAT;
        texture.Wrap_T = GL_REPEAT;
    }
    // Load image
    int width, height;
    unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    if (!image){
        std::cout << "ERROR::TEXTURE: " << SOIL_last_result() << std::endl;
        return texture;
    }
    // Now generate texture
    texture.Generate(static_cast<GLuint>(width), static_cast<GLuint>(height), image);
    // And finally free image data
    SOIL_free_image_data(image);
    return texture;
}

Texture3D ResourceManager::loadCubemapFromFile(std::vector<std::string> faces)
{
    // Create Texture object
    Texture3D texture;
    texture.Bind();
    // Load image
    int width, height;
    unsigned char* image;
    for(GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        if (!image){
            std::cout << "ERROR::CUBEMAP_TEXTURE: " << SOIL_last_result() << std::endl;
            return texture;
        }
        texture.Generate(static_cast<GLuint>(width), static_cast<GLuint>(height),image,i);
        SOIL_free_image_data(image);
    }
    return texture;
}

// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
Model ResourceManager::loadModelFromFile(std::string file){
    Model model;
    // Read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs |
                                                   aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph |
                                                   aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
    // Check for errors
    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP: " << importer.GetErrorString() << std::endl;
        return model;
    }
    // Retrieve the directory path of the filepath
    model.directory = file.substr(0, file.find_last_of('/'));

    // Process ASSIMP's root node recursively
    model.processNode(scene->mRootNode, scene);
    return model;
}