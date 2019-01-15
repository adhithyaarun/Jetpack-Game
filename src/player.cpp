#include "player.h"
#include "main.h"

Player::Player(float x, float y, color_t color) 
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed_x = 0.07;
    speed_y = 0.07;

    // static const GLfloat vertex_buffer_data[] = {
    //     // Body (top-left)
    //      0.15f, 0.3f, 0.0f, 
    //     -0.15f,-0.2f, 0.0f,
    //     -0.15f, 0.3f, 0.0f,
        
    //     // Body (bottom-right)
    //      0.15f, 0.3f, 0.0f,
    //      0.15f,-0.2f, 0.0f,
    //     -0.15f,-0.2f, 0.0f,

    //     // Legs
    //     -0.1f, 0.0f, 0.0f,
    //     -0.1f,-0.3f, 0.0f,
    //      0.1f,-0.3f, 0.0f, 

    //     // Jetpack
    //     -0.15f, 0.3f, 0.0f,
    //     -0.25f, 0.0f, 0.0f,
    //     -0.15f, 0.0f, 0.0f 
    // };

    const float poly_angle = 360.0 / 20;
    const float poly_rad = (poly_angle * 3.14159) / 180.0;

    GLfloat vertex_buffer_data[9 * 24];

    float x_coord = 0.1;
    float y_coord = 0.0;
    float temp_x = 0.0;
    float temp_y = 0.0;

    for (int i = 0; i <= 3 * 20; ++i)
    {
        if (i < 3 * 20)
        {    
            if (i % 3 == 0)
            {
                vertex_buffer_data[3 * i] = 0.0;
                vertex_buffer_data[3 * i + 1] = 0.4;
                vertex_buffer_data[3 * i + 2] = 0.0;
            }
            else
            {
                vertex_buffer_data[3 * i] = x_coord;
                vertex_buffer_data[3 * i + 1] = 0.4 + (y_coord);
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
        else
        {
            // Body (top-left)
            vertex_buffer_data[(3 * i) + 0] = 0.1f;
            vertex_buffer_data[(3 * i) + 1] = 0.3f;
            vertex_buffer_data[(3 * i) + 2] = 0.0f;

            vertex_buffer_data[(3 * i) + 3] = -0.15f;
            vertex_buffer_data[(3 * i) + 4] = -0.1f;
            vertex_buffer_data[(3 * i) + 5] = 0.0f;
            
            vertex_buffer_data[(3 * i) + 6] = -0.15f;
            vertex_buffer_data[(3 * i) + 7] = 0.3f;
            vertex_buffer_data[(3 * i) + 8] = 0.0f;

            // Body (bottom-right)
            vertex_buffer_data[(3 * i) + 9] = 0.1f;
            vertex_buffer_data[(3 * i) + 10] = 0.3f;
            vertex_buffer_data[(3 * i) + 11] = 0.0f;
            
            vertex_buffer_data[(3 * i) + 12] = 0.1f;
            vertex_buffer_data[(3 * i) + 13] = -0.1f;
            vertex_buffer_data[(3 * i) + 14] = 0.0f;
            
            vertex_buffer_data[(3 * i) + 15] = -0.15f;
            vertex_buffer_data[(3 * i) + 16] = -0.1f;
            vertex_buffer_data[(3 * i) + 17] = 0.0f;

            // Legs
            vertex_buffer_data[(3 * i) + 18] = -0.1f;
            vertex_buffer_data[(3 * i) + 19] = 0.1f;
            vertex_buffer_data[(3 * i) + 20] = 0.0f;

            vertex_buffer_data[(3 * i) + 21] = -0.1f;
            vertex_buffer_data[(3 * i) + 22] = -0.2f;
            vertex_buffer_data[(3 * i) + 23] = 0.0f;

            vertex_buffer_data[(3 * i) + 24] = 0.1f;
            vertex_buffer_data[(3 * i) + 25] = -0.2f;
            vertex_buffer_data[(3 * i) + 26] = 0.0f;

            // Jetpack
            vertex_buffer_data[(3 * i) + 27] = -0.15f;
            vertex_buffer_data[(3 * i) + 28] = 0.3f;
            vertex_buffer_data[(3 * i) + 29] = 0.0f;

            vertex_buffer_data[(3 * i) + 30] = -0.25f;
            vertex_buffer_data[(3 * i) + 31] = 0.0f;
            vertex_buffer_data[(3 * i) + 32] = 0.0f;

            vertex_buffer_data[(3 * i) + 33] = -0.15f;
            vertex_buffer_data[(3 * i) + 34] = 0.0f;
            vertex_buffer_data[(3 * i) + 35] = 0.0f;
        }
    }

    this->object = create3DObject(GL_TRIANGLES, 24 * 3, vertex_buffer_data, color, GL_FILL);
}

void Player::draw(glm::mat4 VP) 
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);    // glTranslatef
    glm::mat4 rotate = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    /* No need as coordinates centered at (0, 0, 0) of the cube around which we want to rotate. */
    // rotate = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Player::set_position(float x, float y) 
{
    this->position = glm::vec3(x, y, 0);
}

void Player::tick() 
{
   if(this->position.y > -3.2)
   {
        this->position.y -= gravity;
   }

    // this->position.x -= speed_x;
    // this->position.y -= speed_y;
    // this->rotation += ((speed_x + speed_y) / 2);
}

