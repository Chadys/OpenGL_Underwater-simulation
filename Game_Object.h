//
// Created by julie on 13/02/17.
//

#ifndef PROJET_OBJECT_H
#define PROJET_OBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

#include "Texture.h"
#include "Sprite_Renderer.h"
#include "State_Manager.h"
#include "Model.h"
#include "irrKlang-64bit-1.5.0/include/ik_ISoundEngine.h"


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
    virtual void Draw(State_Manager &manager, Sprite_Renderer &renderer,
                      glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());

};


/*----------------------------------PARTICLE-----------------------------------------*/

class Particle : public GameObject
{
public:
    // Object state
    glm::vec3    Position, Starting_pos, Velocity;
    glm::vec2    Size;
    float        Decay;
    static float NO_DECAY;

    // Constructor(s)
    Particle(glm::vec3 position, glm::vec2 size, Texture2D tex, glm::vec3 velocity = glm::vec3(0),
             float decay = NO_DECAY, irrklang::ISoundEngine* engine = nullptr, const char *audio_path = "");
    // Draw sprite
    virtual void Draw(State_Manager &manager, Sprite_Renderer &renderer,
                      glm::mat4 projection = glm::mat4(), glm::mat4 view = glm::mat4());
    void Update(GLfloat dt, GLfloat currenttime);
};


/*------------------------------------MODEL CONTAINER-----------------------------------------*/

// class regrouping property of a 3D model
class GameModel : public Game_Object3D
{
public:
    // Object state
    Model       model;
    bool        outline, cullface, wings;
    GLfloat     whirlpooling, speed, starting_height, deformation_magnitude;
    glm::vec3   centerpoint;
    glm::mat4   Last_true_pos, New_true_pos;
    // Constructor(s)
    GameModel(std::string file, std::string name);
    // Draw sprite
    void Draw(State_Manager &manager, Shader shader, glm::mat4 projection, glm::mat4 view);
    void Update(GLfloat dt);
};

#endif //PROJET_OBJECT_H
