#include "fire.h"
#include "main.h"
#include "shape.h"

FireBeam::FireBeam(float x, float y, float length)
{
    std::cout << "Constructor call start" << std::endl;
    this->position = glm::vec3(x, y, 0);
    this->length = length;
    this->endpoint1 = Circle(x, y, 0.1, COLOR_GREY);
    this->endpoint2 = Circle(x + length, y, 0.1, COLOR_GREY);
    this->beam = Rectangle((x + length)/2.0, y - 0.02, length, 0.1, COLOR_RED);
    this->down = true;
    std::cout << "Constructor call end" << std::endl;
}

void FireBeam::draw(glm::mat4 VP)
{
    this->beam.draw(VP);
    this->endpoint1.draw(VP);
    this->endpoint2.draw(VP);
}

void FireBeam::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->beam.set_position((x + length)/2.0, y - 0.02);
    this->endpoint1.set_position(x, y);
    this->endpoint2.set_position(x + this->length, y);
}

void FireBeam::tick()
{
    if(this->down && this->position.y > -3.0)
    {
        set_position(this->position.x, this->position.y - 0.03);
    }
    else if (this->position.y <= -3.0)
    {
        this->down = false;
    }
    if(!this->down && this->position.y < 3.0)
    {
        set_position(this->position.x, this->position.y + 0.03);
    }
    else if(this->position.y >= 3.0)
    {
        this->down = true;
    }
}