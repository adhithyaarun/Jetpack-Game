#include "coin.h"
#include "main.h"

Coin::Coin(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    
    this->boundary.x = this->position.x;
    this->boundary.y = this->position.y;
    this->boundary.width = 0.01;
    this->boundary.height = 0.01;

    const color_t coin_color = color;

    if (coin_color.r == COLOR_YELLOW.r && coin_color.g == COLOR_YELLOW.g && coin_color.b == COLOR_YELLOW.b)
    {
        this->multiplier = 1.0;
    }
    else if (coin_color.r == COLOR_ORANGE.r && coin_color.g == COLOR_ORANGE.g && coin_color.b == COLOR_ORANGE.b)
    {
        this->multiplier = 2.0;
    }
    else if (coin_color.r == COLOR_GREEN.r && coin_color.g == COLOR_GREEN.g && coin_color.b == COLOR_GREEN.b)
    {
        this->multiplier = 3.0;
    }

    const float poly_angle = 360.0 / 20;
    const float poly_rad = (poly_angle * 3.14159) / 180.0;

    GLfloat vertex_buffer_data[9 * 20];

    float x_coord = 0.1;
    float y_coord = 0.0;
    float temp_x = 0.0;
    float temp_y = 0.0;

    for (int i = 0; i < 3 * 20; ++i)
    {
        if (i < 3 * 20)
        {
            if (i % 3 == 0)
            {
                vertex_buffer_data[3 * i] = 0.0;
                vertex_buffer_data[3 * i + 1] = 0.0;
                vertex_buffer_data[3 * i + 2] = 0.0;
            }
            else
            {
                vertex_buffer_data[3 * i] = x_coord;
                vertex_buffer_data[3 * i + 1] = y_coord;
                vertex_buffer_data[3 * i + 2] = 0.0;
                if ((i + 1) % 3 != 0)
                {
                    temp_x = (x_coord * cos(poly_rad)) - (y_coord * sin(poly_rad));
                    temp_y = (x_coord * sin(poly_rad)) + (y_coord * cos(poly_rad));
                    x_coord = temp_x;
                    y_coord = temp_y;
                }
            }
        }
    }

    this->object = create3DObject(GL_TRIANGLES, 20 * 3, vertex_buffer_data, color, GL_LINE);
}

void Coin::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    /* No need as coordinates centered at (0, 0, 0) of the cube around which we want to rotate. */
    // rotate = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Coin::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Coin::tick()
{
    this->rotation += 2.0;
}