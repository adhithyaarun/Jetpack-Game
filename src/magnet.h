#include "main.h"
#include "shape.h"

#ifndef MAGNET_H
#define MAGNET_H

class Magnet
{
  public:
    Magnet() {}
    Magnet(float x, float y);
    glm::vec3 position;
    float rotation;
    bounding_box_t boundary;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float power;
    Rectangle magnet_l;
    Rectangle magnet_r;
};

#endif
