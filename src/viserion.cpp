#include "viserion.h"
#include "main.h"

Viserion::Viserion(float x, float y) 
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->shots = 6;
    this->counter = 1;
    speed_x = 0.07;
    speed_y = 0.07;

    GLfloat vertex_buffer_data[]= {
        0.4, 0.4, 0.0,
        0.2, 0.0, 0.0,
        0.4, 0.0, 0.0,

        0.8, 0.4, 0.0,
        0.6, 0.0, 0.0,
        0.8, 0.0, 0.0,

        0.3, 0.9, 0.0,
        0.0, 0.5, 0.0,
        0.3, 0.5, 0.0,

        0.25, 0.5, 0.0,
        0.25, 0.2, 0.0,
        1.05, 0.2, 0.0,
        
        1.05, 0.5, 0.0,
        0.25, 0.5, 0.0,
        1.05, 0.2, 0.0,

        1.35, 0.5, 0.0,
        1.05, 0.5, 0.0,
        1.05, 0.4, 0.0,
    };

    this->object = create3DObject(GL_TRIANGLES, 6 * 3, vertex_buffer_data, COLOR_SPECIAL_GREEN, GL_FILL);
}

void Viserion::draw(glm::mat4 VP) 
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
    this->shot.draw(VP);
}

void Viserion::set_position(float x, float y) 
{
    this->position = glm::vec3(x, y, 0);
}

void Viserion::tick() 
{
    if(this->shots > 0 && counter == 100)
    {
        this->shot = Circle(this->position.x, this->position.y, 0.2, COLOR_SPECIAL_GREEN);
        this->shot.set_position(this->shot.position.x + 0.1, this->shot.position.y);  
        this->shots--;
        this->counter = 0;
    }
    else if(this->shots > 0)
    {
        this->counter++;
        this->shot.set_position(this->shot.position.x - 0.1, this->shot.position.y);  
    }
}