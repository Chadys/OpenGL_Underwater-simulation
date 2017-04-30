//
// Created by julie on 13/02/17.
//

#ifndef PREPROJET_GAME_H
#define PREPROJET_GAME_H

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

using namespace std;

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
    void Render();

private:
    Camera                   Cam;
    GLfloat                  lastX, lastY;
    GLboolean                firstMouse;
    GLboolean                DEBUG;
    vector<Sprite_Renderer*> Renderer;
    Text_Renderer            *T_Renderer;
    vector<Object3D>         objects;
    vector<Square>           squares;
    vector<Text>             texts;
    std::vector<GameModel> 	 models;
    vector<string> get_skybox(string path, string ext);
};


#endif //PREPROJET_GAME_H
