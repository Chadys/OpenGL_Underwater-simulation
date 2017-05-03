//
// Created by julie on 13/02/17.
//

#ifndef PREPROJET_OBJECT_H
#define PREPROJET_OBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

#include "Texture.h"
#include "Sprite_Renderer.h"
#include "State_Manager.h"
#include "Model.h"


/*------------------------------------INTERFACES-----------------------------------------*/

class GameObject
{
public:
    // Render state
    Texture2D   Tex;
    // Draw sprite
    GameObject(Texture2D sprite) : Tex(sprite) { }
    virtual void Draw(State_Manager &manager, Sprite_Renderer &renderer, glm::mat4 projection, glm::mat4 view) = 0;
};

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class Game_Object3D
{
public:
    // Object state
    glm::vec3   Position, Size, Color, Rotation;
    GLfloat     Alpha;
    // Constructor(s)
    Game_Object3D(glm::vec3 pos, glm::vec3 size, glm::vec3 color = glm::vec3(1.0f))
            : Position(pos), Size(size), Color(color), Rotation(0), Alpha(1.0f) { }
};



/*----------------------------------------3D PLANES---------------------------------------------*/

class Plane : public GameObject
{
public:
    // Object state
    glm::vec3   Position, Rotation;
    glm::vec2   Size;
    Texture3D   Reflect;
    // Constructor(s)
    Plane(glm::vec3 pos, glm::vec2 size, Texture2D normals, Texture3D reflect);
    virtual void Draw(State_Manager &manager, Sprite_Renderer &renderer, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());

};


/*----------------------------------COLORED-SQUARE-----------------------------------------*/

class Square
{
public:
    // Object state
    glm::vec2   Position, Size;
    GLboolean   IsCircle;
    glm::vec3   Color1, Color2;
    Effect      Effet;

    // Constructor(s)
    Square(glm::vec2 position, glm::vec2 size, glm::vec3 color1, glm::vec3 color2 = glm::vec3(0), Effect effect = NONE, GLboolean circle = GL_FALSE);
    // Draw sprite
    virtual void Draw(State_Manager &manager, Sprite_Renderer &renderer, glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4(), GLboolean border = GL_FALSE);
};


/*------------------------------------MODEL CONTAINER-----------------------------------------*/

// Represents the different sides of a model
enum Side {
    UP_SIDE,
    DOWN_SIDE,
    FRONT_SIDE,
    BACK_SIDE,
    LEFT_SIDE,
    RIGHT_SIDE
};

// class regrouping property of a 3D model
class GameModel : public Game_Object3D
{
public:
    // Object state
    Model       model;
    bool        outline, cullface, wings;
    GLfloat     whirlpooling, speed, starting_height, deformation_magnitude;
    glm::vec3   centerpoint;
    // Constructor(s)
    GameModel(std::string file, std::string name);
    // Draw sprite
    void Draw(State_Manager &manager, Shader shader, glm::mat4 projection, glm::mat4 view);
    void Update(GLfloat dt);
    // Set model's position correctly :
    // the given side of the model will be localised as to be at pos location if it is facing the orientation side_local
    // Putting the model in its correct orientation for it to work will need to be done elsewhere.
    void SetSide(GLfloat pos, Side side_local, Side side_model);
    void SetSide(GLfloat pos, Side side_local) { SetSide(pos, side_local, side_local); }
};

#endif //PREPROJET_OBJECT_H
