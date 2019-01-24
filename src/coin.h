#include "main.h"

#ifndef COIN_H
#define COIN_H

class Coin
{
  public:
    Coin() {}
    Coin(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float multiplier;
    bounding_box_t boundary;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
  private:
    VAO *object;
};

#endif
