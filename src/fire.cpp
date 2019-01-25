#include "fire.h"
#include "main.h"
#include "shape.h"

// Firebeams
FireBeam::FireBeam(float x, float y, float length)
{
    this->position = glm::vec3(x, y, 0);
    this->length = length;
    this->endpoint1 = Circle(x, y, 0.1, COLOR_GREY);
    this->endpoint2 = Circle(x + length, y, 0.1, COLOR_GREY);
    this->beam = Rectangle(x + (length/2.0), y - 0.02, length, 0.1, 0.0, COLOR_RED);
    this->down = true;
    this->disabled = false;
}

void FireBeam::draw(glm::mat4 VP)
{
    if(!this->disabled)
    {
        this->beam.draw(VP);
    }
    this->endpoint1.draw(VP);
    this->endpoint2.draw(VP);
}

void FireBeam::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    if(!this->disabled)
    {
        this->beam.set_position(x + (length/2.0), y - 0.02);
    }
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

// Firelines
FireLine::FireLine(float x, float y, float length, float angle)
{
    this->position = glm::vec3(x, y, 0);
    this->length = length;
    this->angle = angle;
    this->endpoint1 = Circle(x, y, 0.1, COLOR_GREY);
    this->endpoint2 = Circle(x + (length * cos(angle * M_PI / 180.0)), y + (length * sin(angle * M_PI / 180.0)), 0.1, COLOR_GREY);
    this->beam = Rectangle(x + ((length / 2.0) * cos(angle * M_PI / 180.0)), y + ((length / 2.0) * sin(angle * M_PI / 180.0)), length, 0.1, angle, COLOR_RED);
    this->disabled = false;
}

void FireLine::draw(glm::mat4 VP)
{
    if(!this->disabled)
    {
        this->beam.draw(VP);
    }
    this->endpoint1.draw(VP);
    this->endpoint2.draw(VP);
}

void FireLine::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->endpoint1.set_position(x, y);
    this->endpoint2.set_position(x + (this->length * cos(this->angle * M_PI / 180.0)), y + (this->length * sin(angle * M_PI / 180.0)));
    if(!this->disabled)
    {
        this->beam.set_position(x + ((this->length / 2.0) * cos(this->angle * M_PI / 180.0)), y + ((this->length / 2.0) * sin(angle * M_PI / 180.0)));
    }
}

void FireLine::tick()
{
   ;
}