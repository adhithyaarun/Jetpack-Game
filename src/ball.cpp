#include "ball.h"
#include "main.h"

Player::Player(float x, float y, color_t color) 
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed_x = 0.07;
    speed_y = 0.07;

    /*  
        VERTICES: 
        Three consecutive float values give a 3D vertex. 
        Three consecutive vertices give a triangle.
        A cube has 6 faces with 2 triangles each.
        Therefore, 6 * 2 = 12 triangles and 12 * 3 = 36 vertices  
    */
    
    static const GLfloat vertex_buffer_data[] = {
        -0.3f,-0.3f,-0.3f, 
        -0.3f,-0.3f, 0.3f,
        -0.3f, 0.3f, 0.3f, 
         0.3f, 0.3f,-0.3f, 
        -0.3f,-0.3f,-0.3f,
        -0.3f, 0.3f,-0.3f,
         0.3f,-0.3f, 0.3f,
        -0.3f,-0.3f,-0.3f,
         0.3f,-0.3f,-0.3f,
         0.3f, 0.3f,-0.3f,
         0.3f,-0.3f,-0.3f,
        -0.3f,-0.3f,-0.3f,
        -0.3f,-0.3f,-0.3f,
        -0.3f, 0.3f, 0.3f,
        -0.3f, 0.3f,-0.3f,
         0.3f,-0.3f, 0.3f,
        -0.3f,-0.3f, 0.3f,
        -0.3f,-0.3f,-0.3f,
        -0.3f, 0.3f, 0.3f,
        -0.3f,-0.3f, 0.3f,
         0.3f,-0.3f, 0.3f,
         0.3f, 0.3f, 0.3f,
         0.3f,-0.3f,-0.3f,
         0.3f, 0.3f,-0.3f,
         0.3f,-0.3f,-0.3f,
         0.3f, 0.3f, 0.3f,
         0.3f,-0.3f, 0.3f,
         0.3f, 0.3f, 0.3f,
         0.3f, 0.3f,-0.3f,
        -0.3f, 0.3f,-0.3f,
         0.3f, 0.3f, 0.3f,
        -0.3f, 0.3f,-0.3f,
        -0.3f, 0.3f, 0.3f,
         0.3f, 0.3f, 0.3f,
        -0.3f, 0.3f, 0.3f,
         0.3f,-0.3f, 0.3f
    };

    this->object = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color, GL_FILL);
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
   if(this->position.y > -3.0)
   {
        this->position.y -= 0.03;
   }
    
    // this->position.x -= speed_x;
    // this->position.y -= speed_y;
    // this->rotation += ((speed_x + speed_y) / 2);
}

