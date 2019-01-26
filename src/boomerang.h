#include "main.h"
#include "shape.h"

#ifndef BOOMERANG_H
#define BOOMERANG_H

class Boomerang
{
  public:
    Boomerang() {}
    Boomerang(float x, float y);
    glm::vec3 position;
    float rotation;
    bounding_box_t boundary;
    void draw(glm::mat4 VP);
    void set_position(double x, double y);
    void tick();
    double start_x;
    double step;
    Rectangle top;
    Rectangle bottom;
};

#endif
