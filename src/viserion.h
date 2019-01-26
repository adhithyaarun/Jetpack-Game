#include "main.h"
#include "shape.h"

#ifndef VISERION_H
#define VISERION_H

class Viserion 
{
    public:
        Viserion() {}
        Viserion(float x, float y);
        glm::vec3 position;
        float rotation;
        void draw(glm::mat4 VP);
        void set_position(float x, float y);
        void tick();
        double speed_x;
        double speed_y;
        int shots;
        int counter;
        Circle shot;
    private:
        VAO* object;
};

#endif
