#include "boomerang.h"
#include "main.h"
#include "shape.h"

const double BASE_SPEED_Y = 0.01;
const double STEP = 0.005;

Boomerang::Boomerang(float x, float y)
{
    this->position = glm::vec3(x, y, 0);

    this->start_x = x;
    this->step = 0.005;

    this->top    = Rectangle(x + ((0.3 / 2.0) * cos(M_PI / 4.0)), y + ((0.3 / 2.0) * sin(M_PI / 4.0)), 0.3, 0.2, 0.0, COLOR_BROWN);
    this->bottom = Rectangle(x + ((0.3 / 2.0) * cos(M_PI / 4.0)), y - ((0.3 / 2.0) * sin(M_PI / 4.0)), 0.3, 0.2, 0.0, COLOR_BROWN);

    this->boundary.x = this->position.x;
    this->boundary.y = this->position.y;
    this->boundary.width = 0.3 * cos(M_PI / 4.0);
    this->boundary.height = 2 * 0.3 * sin(M_PI / 4.0);
}

void Boomerang::draw(glm::mat4 VP)
{
    this->top.draw(VP);
    this->bottom.draw(VP);
}

void Boomerang::set_position(double x, double y)
{
    this->position = glm::vec3(x, y, 0);
    this->boundary.x = this->position.x;
    this->boundary.y = this->position.y;
}

void Boomerang::tick()
{
    if(this->position.x == this->start_x && this->position.y < 0)
    {
        ;
    }
    else if(this->position.y > 0)
    {
        this->set_position(this->position.x - 0.1, this->position.y - BASE_SPEED_Y - this->step);
        this->step += STEP;
    }
    else if(this->position.y == 0)
    {
        this->set_position(this->position.x, this->position.y - BASE_SPEED_Y);
        this->step = 0;
    }
    else
    {
        this->set_position(this->position.x + 0.1, this->position.y - BASE_SPEED_Y - this->step);
        this->step += STEP;
    }
}