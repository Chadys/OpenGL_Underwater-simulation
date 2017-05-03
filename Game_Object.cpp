//
// Created by julie on 13/02/17.
//

#include "Game_Object.h"
#include "Resource_Manager.h"


Square::Square(glm::vec2 position, glm::vec2 size, glm::vec3 color1, glm::vec3 color2, Effect effect, GLboolean circle)
        : Position(position), Size(size), IsCircle(circle), Color1(color1), Color2(color2), Effet(effect) { }

void Square::Draw(State_Manager &manager, Sprite_Renderer &renderer, glm::mat4 projection, glm::mat4 view, GLboolean border)
{
    renderer.DrawSprite(manager, this->Position, this->Size, this->IsCircle, this->Color1, this->Color2, this->Effet, projection, view, border);
}

 //-----------------

Plane::Plane(glm::vec3 pos, glm::vec2 size, Texture2D normals, Texture3D reflect)
        : GameObject(normals), Position(pos), Size(size), Reflect(reflect) {
    this->Rotation.y+=180;
}

void Plane::Draw(State_Manager &manager, Sprite_Renderer &renderer, glm::mat4 projection, glm::mat4 view)
{
    renderer.DrawSprite(manager, this->Tex, this->Reflect, this->Position, this->Size, this->Rotation, projection, view);
}

//-----------------

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

// Set the model in the correct position depending on its bounds so that the specified side of it is at the pos coordinate
void GameModel::SetSide(GLfloat pos, Side side_local, Side side_model){
    GLfloat newpos;
    switch(side_model){
        case UP_SIDE :
            newpos = pos-(this->model.Span_udb.x*this->Size.y);
            break;
        case DOWN_SIDE :
            newpos = pos-(this->model.Span_udb.y*this->Size.y);
            break;
        case FRONT_SIDE :
            newpos = pos-(this->model.Span_lrf.z*this->Size.x);
            break;
        case BACK_SIDE :
            newpos = pos-(this->model.Span_udb.z*this->Size.x);
            break;
        case LEFT_SIDE :
            newpos = pos-(this->model.Span_lrf.x*this->Size.z);
            break;
        case RIGHT_SIDE :
            newpos = pos-(this->model.Span_lrf.y*this->Size.z);
    }
    switch(side_local){
        case UP_SIDE :
        case DOWN_SIDE :
            this->Position.y = newpos;
            break;
        case FRONT_SIDE :
        case BACK_SIDE :
            this->Position.x = newpos;
            break;
        default :
            this->Position.z = newpos;
    }
}