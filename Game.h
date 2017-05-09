//
// Created by julie on 13/02/17.
//

#ifndef PROJET_GAME_H
#define PROJET_GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Game_Object.h"
#include "State_Manager.h"
#include "Resource_Manager.h"
#include "Sprite_Renderer.h"
#include "Text_Renderer.h"
#include "Text.h"

#include <string>
#include <ctime>
#include <memory>
#include <random>
#include <unordered_set>

class Game {
public:
    // Game state
    GLboolean              Keys[1024], ProcessedKeys[1024];
    GLuint                 Width, Height;
    State_Manager		   State_manager;

    // Constructor/Destructor
    Game();
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    // GameLoop
    void ProcessInput(GLfloat dt);
    void ProcessMouseMovement(GLdouble xpos, GLdouble ypos);
    void ProcessMouseScroll(GLdouble yoffset);
    void Update(GLfloat dt, GLfloat currenttime);
    void RenderBuffer();
    void RenderScreen();

private:
    Camera                         Cam;
    GLdouble                       lastX, lastY;
    bool                           firstMouse;
    bool                           DEBUG;
    vector<Sprite_Renderer*>       Renderer;
    Text_Renderer                  *T_Renderer;
    vector<Plane>                  planes;
    vector<Particle>               bubbles;
    vector<Text>                   texts;
    std::vector<GameModel> 	       models;
    std::mt19937		           gen;
    PostProd::POSTPROD_EFFECT      effect;
    static std::unordered_set<int> no_neighbor;
    static std::unordered_set<int> convolute;

    vector<string>           get_skybox(string path, string ext);
    void                     setConstantShadersUniforms(vector<Shader> &shaders);
    void                     add_models();
    void                     add_bubbles(Texture2D &tex, unsigned int n);
    void                     add_trailing_bubbles(Texture2D &tex, GameModel &mod, unsigned int n);
    void                     UpdatePostProd();
};


#endif //PROJET_GAME_H
