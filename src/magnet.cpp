#include "magnet.h"
#include "main.h"
#include "shape.h"

const double BASE_SPEED_Y = 0.01;

Magnet::Magnet(float x, float y)
{
    this->position = glm::vec3(x, y, 0);

    this->magnet_l = Rectangle(x, y, 0.4, 0.2, 0.0, COLOR_RED);
    this->magnet_r = Rectangle(x + 0.4, y, 0.4, 0.2, 0.0, COLOR_BLUE);

    this->power = 0.2;

    this->boundary.x = this->position.x;
    this->boundary.y = this->position.y;
    this->boundary.width = 0.4;
    this->boundary.height = 0.2;
}

void Magnet::draw(glm::mat4 VP)
{
    this->magnet_l.draw(VP);
    this->magnet_r.draw(VP);
}

void Magnet::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->boundary.x = this->position.x;
    this->boundary.y = this->position.y;
}

void Magnet::tick()
{
    this->power += BASE_SPEED_Y;
}