//
// Created by julie on 13/02/17.
//

#include "Game_Object.h"
#include "Resource_Manager.h"

//-----------------Particle
float Particle::NO_DECAY = FLT_MAX;

Particle::Particle(glm::vec3 position, glm::vec2 size, Texture2D tex, glm::vec3 velocity, float decay)
        : GameObject(tex), Position(position), Starting_pos(position), Size(size), Velocity(velocity), Decay(decay) { }

void Particle::Draw(State_Manager &manager, Sprite_Renderer &renderer, glm::mat4 projection, glm::mat4 view) {
    renderer.DrawSprite(manager, this->Tex, this->Position, this->Size, projection, view);
}

void Particle::Update(GLfloat dt, GLfloat currenttime){
    this->Position += this->Velocity*dt;
    if(this->Decay != NO_DECAY)
        this->Decay -= dt;
    if(this->Velocity.y > 0){
        this->Position.x = this->Starting_pos.x+(glm::sin(currenttime * this->Velocity.y));
    }
    else{
        this->Position.y = this->Starting_pos.y+(glm::sin(currenttime * (this->Velocity.x+this->Velocity.z)));
    }
}

//-----------------Plane

Plane::Plane(glm::vec3 pos, glm::vec2 size, Texture2D normals, Texture3D reflect)
        : GameObject(normals), Position(pos), Size(size), Reflect(reflect) {
    this->Rotation.y+=180;
}

void Plane::Draw(State_Manager &manager, Sprite_Renderer &renderer, glm::mat4 projection, glm::mat4 view)
{
    renderer.DrawSprite(manager, this->Tex, this->Reflect, this->Position, this->Size, this->Rotation, projection, view);
}

//-----------------Model

GameModel::GameModel(std::string file, std::string name)
        : Game_Object3D(glm::vec3(0), glm::vec3(1), glm::vec3(0.0f,0.0f,1.0f)), // The color here is stocked in HSV format
        outline(true), cullface(true), wings(false), whirlpooling(0), speed(50), starting_height(0), deformation_magnitude(0), centerpoint(0) {
    this->model = ResourceManager::LoadModel(file, name);
}

void GameModel::Draw(State_Manager &manager, Shader shader, glm::mat4 projection, glm::mat4 view){
    this->model.Draw(manager, shader, this->Position, this->Size, this->Rotation, this->Color, this->Alpha,
                     this->outline, this->wings, this->deformation_magnitude, this->whirlpooling, this->centerpoint, projection, view);
}

void GameModel::Update(GLfloat dt){
    this->whirlpooling += dt*this->speed;
    this->Position.y = this->starting_height+(glm::sin(0.2f*this->whirlpooling)*glm::pi<float>());
}