#include "powerup.h"
#include "main.h"
#include "shape.h"

const double BASE_SPEED_Y = 0.01;

Multiplier::Multiplier(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->powerup = Circle(x, y, 0.2, COLOR_YELLOW);
    this->collected = false;

    this->speed_x = 0.05;
    this->speed_y = BASE_SPEED_Y;

    this->boundary.x = this->position.x;
    this->boundary.y = this->position.y;
    this->boundary.width = 0.2;
    this->boundary.height = 0.2;
}

void Multiplier::draw(glm::mat4 VP)
{
    this->powerup.draw(VP);
}

void Multiplier::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->powerup.set_position(x, y);
    this->boundary.x = this->position.x;
    this->boundary.y = this->position.y;
}

void Multiplier::tick()
{
    if ((this->speed_y > 0.0 && this->position.y <= -2.0) || (this->speed_y < 0.0 && this->position.y >= 2.0))
    {
        this->speed_y = this->speed_y > 0.0 ? (-1 * BASE_SPEED_Y) : BASE_SPEED_Y;
    }

    if (this->speed_y > 0.0 && this->position.y > -2.0)
    {
        set_position(this->position.x - this->speed_x, this->position.y - this->speed_y);
        this->speed_y += 0.001;
    }
    else if(this->speed_y < 0.0 && this->position.y < 2.0)
    {
        set_position(this->position.x - this->speed_x, this->position.y - this->speed_y);
        this->speed_y -= 0.001;
    }
}

Shield::Shield(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->powerup = Circle(x, y, 0.2, COLOR_PURPLE);
    this->collected = false;

    this->speed_x = 0.05;
    this->speed_y = BASE_SPEED_Y;

    this->boundary.x = this->position.x;
    this->boundary.y = this->position.y;
    this->boundary.width = 0.2;
    this->boundary.height = 0.2;
}

void Shield::draw(glm::mat4 VP)
{
    this->powerup.draw(VP);
}

void Shield::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->powerup.set_position(x, y);
    this->boundary.x = this->position.x;
    this->boundary.y = this->position.y;
}

void Shield::tick()
{
    if ((this->speed_y > 0.0 && this->position.y <= -2.0) || (this->speed_y < 0.0 && this->position.y >= 2.0))
    {
        this->speed_y = this->speed_y > 0.0 ? (-1 * BASE_SPEED_Y) : BASE_SPEED_Y;
    }

    if (this->speed_y > 0.0 && this->position.y > -2.0)
    {
        set_position(this->position.x - this->speed_x, this->position.y - this->speed_y);
        this->speed_y += 0.001;
    }
    else if(this->speed_y < 0.0 && this->position.y < 2.0)
    {
        set_position(this->position.x - this->speed_x, this->position.y - this->speed_y);
        this->speed_y -= 0.001;
    }
}