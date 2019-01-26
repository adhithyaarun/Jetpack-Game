#include "boomerang.h"
#include "main.h"
#include "shape.h"

const double BASE_SPEED_Y = 0.01;
const double STEP = 0.003;

Boomerang::Boomerang(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0.0;
    this->rotation_speed = 25.0;

    this->start_x = x;
    this->start_y = y;
    this->step = 0.005;

    this->boundary.x = this->position.x;
    this->boundary.y = this->position.y;
    this->boundary.width = 0.35;
    this->boundary.height = 0.4;

    GLfloat vertex_buffer_data[] = { 
        0.35, 0.2, 0.0,
         0.0, 0.0, 0.0,
         0.1, 0.0, 0.0,
        
         0.1, 0.0, 0.0,
         0.0, 0.0, 0.0,
        0.35,-0.2, 0.0
    };

    this->object = create3DObject(GL_TRIANGLES, 2 * 3, vertex_buffer_data, COLOR_BROWN, GL_FILL);
}

void Boomerang::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);    // glTranslatef
    glm::mat4 rotate = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    /* No need as coordinates centered at (0, 0, 0) of the cube around which we want to rotate. */
    // rotate = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Boomerang::set_position(double x, double y)
{
    this->position = glm::vec3(x, y, 0);
    this->boundary.x = this->position.x;
    this->boundary.y = this->position.y;
}

void Boomerang::tick()
{
    if(this->position.x == this->start_x && this->position.y < 0)
    {
        ;
    }
    else if(this->position.y <= -1 * this->start_y)
    {
        this->set_position(this->position.x + 0.1, this->position.y);
    }
    else if(this->position.y > 0)
    {
        this->set_position(this->position.x - 0.1, this->position.y - BASE_SPEED_Y - this->step);
        this->step += STEP;
    }
    else if(this->position.y == 0)
    {
        this->set_position(this->position.x, this->position.y - BASE_SPEED_Y);
        this->step = 0;
    }
    else
    {
        this->set_position(this->position.x + 0.1, this->position.y - BASE_SPEED_Y - this->step);
        this->step += STEP;
    }
    this->rotation += this->rotation_speed;
}