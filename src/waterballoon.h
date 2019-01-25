#include "main.h"
#include "shape.h"

#ifndef WATERBALLOON_H
#define WATERBALLOON_H

class WaterBalloon
{
  public:
    WaterBalloon() {}
    WaterBalloon(float x, float y);
    glm::vec3 position;
    float rotation;
    bounding_box_t boundary;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed_x;
    double speed_y;
    Circle balloon;
};

#endif
