#include "main.h"

#ifndef BALL_H
#define BALL_H

class Player 
{
    public:
        Player() {}
        Player(float x, float y, color_t color);
        glm::vec3 position;
        float rotation;
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick();
        double speed_x;
        double speed_y;
    private:
        VAO* object;
};

#endif
