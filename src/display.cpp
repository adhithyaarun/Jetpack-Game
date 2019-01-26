#include "display.h"
#include "main.h"

//  1     ___ 
//  2    |___|    3
//  5    |_4_|    6
//         7 

Digit::Digit(float x, float y, int digit)
{
    this->position = glm::vec3(x, y, 0);
    this->digit = 0;

    this->segment1 = Rectangle(x - 3.0, 4.0, 0.2, 0.02,   0, COLOR_WHITE);
    this->segment2 = Rectangle(x - 3.1, 3.9, 0.2, 0.02, -90, COLOR_WHITE);
    this->segment3 = Rectangle(x - 2.9, 3.9, 0.2, 0.02, -90, COLOR_WHITE);
    this->segment4 = Rectangle(x - 3.0, 3.8, 0.2, 0.02,   0, COLOR_WHITE);
    this->segment5 = Rectangle(x - 3.1, 3.7, 0.2, 0.02, -90, COLOR_WHITE);
    this->segment6 = Rectangle(x - 2.9, 3.7, 0.2, 0.02, -90, COLOR_WHITE);
    this->segment7 = Rectangle(x - 3.0, 3.6, 0.2, 0.02,   0, COLOR_WHITE);
}

void Digit::draw(glm::mat4 VP)
{
    switch(this->digit)
    {
        case 0:
            this->segment1.draw(VP);
            this->segment2.draw(VP);
            this->segment3.draw(VP);
            this->segment5.draw(VP);
            this->segment6.draw(VP);
            this->segment7.draw(VP);
            break;
        case 1: 
            this->segment3.draw(VP);
            this->segment6.draw(VP);
            break;
        case 2: 
            this->segment1.draw(VP);
            this->segment3.draw(VP);
            this->segment4.draw(VP);
            this->segment5.draw(VP);
            this->segment7.draw(VP);
            break;
        case 3:
            this->segment1.draw(VP);
            this->segment3.draw(VP);
            this->segment4.draw(VP);
            this->segment6.draw(VP);
            this->segment7.draw(VP);
            break;
        case 4: 
            this->segment2.draw(VP);
            this->segment3.draw(VP);
            this->segment1.draw(VP);
            this->segment6.draw(VP);
            break;
        case 5:
            this->segment1.draw(VP);
            this->segment2.draw(VP);
            this->segment4.draw(VP);
            this->segment6.draw(VP);
            this->segment7.draw(VP);
            break;
        case 6: 
            this->segment1.draw(VP);
            this->segment2.draw(VP);
            this->segment4.draw(VP);
            this->segment5.draw(VP);
            this->segment6.draw(VP);
            this->segment7.draw(VP);
            break;
        case 7: 
            this->segment1.draw(VP);
            this->segment3.draw(VP);
            this->segment6.draw(VP);
            break;
        case 8: 
            this->segment1.draw(VP);
            this->segment2.draw(VP);
            this->segment3.draw(VP);
            this->segment4.draw(VP);
            this->segment5.draw(VP);
            this->segment6.draw(VP);
            this->segment7.draw(VP);
            break;
        case 9:
            this->segment1.draw(VP);
            this->segment2.draw(VP);
            this->segment3.draw(VP);
            this->segment4.draw(VP);
            this->segment6.draw(VP);
            this->segment7.draw(VP);
            break;
        default:
            this->segment1.draw(VP);
            this->segment2.draw(VP);
            this->segment3.draw(VP);
            this->segment5.draw(VP);
            this->segment6.draw(VP);
            this->segment7.draw(VP);
            break;
    }
}

void Digit::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->segment1.set_position(x - 3.0, 4.0);
    this->segment2.set_position(x - 3.1, 3.9);
    this->segment3.set_position(x - 2.9, 3.9);
    this->segment4.set_position(x - 3.0, 3.8);
    this->segment5.set_position(x - 3.1, 3.7);
    this->segment6.set_position(x - 2.9, 3.7);
    this->segment7.set_position(x - 3.0, 3.6);
}

void Digit::tick(float x, int digit)
{
    this->set_position(x, 3.00);
    this->digit = digit;
}

// Display
Display::Display(float x, float y, int score)
{
    this->position = glm::vec3(x, y, 0);
    this->value = score;

    this->d10_0 = Digit(x + 1.8, 4.0, score % 10);
    score /= 10;
    this->d10_1 = Digit(x + 1.5, 4.0, score % 10);
    score /= 10;
    this->d10_2 = Digit(x + 1.2, 4.0, score % 10);
    score /= 10;
    this->d10_3 = Digit(x + 0.9, 4.0, score % 10);
    score /= 10;
    this->d10_4 = Digit(x + 0.6, 4.0, score % 10);
    score /= 10;
    this->d10_5 = Digit(x + 0.3, 4.0, score % 10);
    score /= 10;
    this->d10_6 = Digit(x, 4.0, score % 10);
}

void Display::draw(glm::mat4 VP)
{
    this->d10_0.draw(VP);
    this->d10_1.draw(VP);
    this->d10_2.draw(VP);
    this->d10_3.draw(VP);
    this->d10_4.draw(VP);
    this->d10_5.draw(VP);
    this->d10_6.draw(VP);
}

void Display::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->d10_6.set_position(x, 4.0);
    this->d10_5.set_position(x + 0.3, 4.0);
    this->d10_4.set_position(x + 0.6, 4.0);
    this->d10_3.set_position(x + 0.9, 4.0);
    this->d10_2.set_position(x + 1.2, 4.0);
    this->d10_1.set_position(x + 1.5, 4.0);
    this->d10_0.set_position(x + 1.8, 4.0);
}

void Display::tick(float x, int value)
{
    int temp = value;
    this->value = value;
    this->d10_0.tick(x + 1.8, temp % 10);
    temp /= 10;
    this->d10_1.tick(x + 1.5, temp % 10);
    temp /= 10;
    this->d10_2.tick(x + 1.2, temp % 10);
    temp /= 10;
    this->d10_3.tick(x + 0.9, temp % 10);
    temp /= 10;
    this->d10_4.tick(x + 0.6, temp % 10);
    temp /= 10;
    this->d10_5.tick(x + 0.3, temp % 10);
    temp /= 10;
    this->d10_6.tick(x, temp % 10);
}
