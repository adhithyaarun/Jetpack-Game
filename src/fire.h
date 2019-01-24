#include "main.h"
#include "shape.h"

#ifndef FIRE_H
#define FIRE_H

class FireLine
{
  public:
    FireLine() {}
    FireLine(float x, float y);
    glm::vec3 position;
    float rotation;
    bounding_box_t boundary;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double length;
    double angle;
    Circle endpoint1;
    Circle endpoint2;
};

class FireBeam
{
  public:
    FireBeam() {}
    FireBeam(float x, float y, float length);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double length;
    bool down;
    Circle endpoint1;
    Circle endpoint2;
    Rectangle beam;
};


#endif
