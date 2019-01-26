#include "main.h"
#include "shape.h"

#ifndef POWERUP_H
#define POWERUP_H

class Multiplier
{
  public:
    Multiplier() {}
    Multiplier(float x, float y);
    glm::vec3 position;
    float rotation;
    bounding_box_t boundary;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed_x;
    double speed_y;
    bool collected;
    Circle powerup;
};

class Shield
{
  public:
    Shield() {}
    Shield(float x, float y);
    glm::vec3 position;
    float rotation;
    bounding_box_t boundary;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed_x;
    double speed_y;
    bool collected;
    Circle powerup;
};

#endif
