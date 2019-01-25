#include "waterballoon.h"
#include "main.h"
#include "shape.h"

WaterBalloon::WaterBalloon(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->balloon = Circle(x, y, 0.2, COLOR_GREY);
    
    this->speed_x = 0.1;
    this->speed_y = 0.01;

    this->boundary.x = this->position.x;
    this->boundary.y = this->position.y;
    this->boundary.width = 0.2;
    this->boundary.height = 0.2;
}

void WaterBalloon::draw(glm::mat4 VP)
{
    this->balloon.draw(VP);
}

void WaterBalloon::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->balloon.set_position(x, y);
    this->boundary.x = this->position.x;
    this->boundary.y = this->position.y;
}

void WaterBalloon::tick()
{
    if(this->position.y > -3.2)
    {
        set_position(this->position.x + this->speed_x, this->position.y - this->speed_y);
        this->speed_y += 0.005; 
    }
}