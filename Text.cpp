//
// Created by julie on 04/03/17.
//

#include "Text.h"

Text::Text(std::string t, glm::vec2 position, GLfloat size_factor, glm::vec4 color)
        : text(t), Position(position), Size_factor(size_factor), Color(color){ }

void Text::Draw(State_Manager &manager, Text_Renderer &renderer){
    renderer.RenderText(manager, this->text, this->Position.x, this->Position.y, this->Size_factor, this->Color);
}