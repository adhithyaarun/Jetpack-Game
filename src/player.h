#include "main.h"
#include "shape.h"

#ifndef PLAYER_H
#define PLAYER_H

const float GRAVITY = 0.03;
const float GRAVITY_INCR = 0.002;
const float PLAYER_WIDTH = 0.3;
const float PLAYER_HEIGHT = 0.3;

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
        float gravity;
        bool freefall;
        float score;
    private:
        VAO* object;
};

class Propulsion
{
    public:
        Propulsion() {}
        Propulsion(float x, float y);
        glm::vec3 position;
        float rotation;
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick();
        bool disappear;
    private:
        VAO* object;
};

#endif
