#include "main.h"
#include "player.h"
#include "platform.h"
#include "coin.h"
#include "shape.h"
#include "fire.h"
#include "waterballoon.h"
#include "timer.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow* window;

/**************************
* Customizable functions *
**************************/

float SCREEN_CENTER_X = 0.0;
float SCREEN_CENTER_Y = 0.0;

Player player;
Platform platform;
Cieling cieling;
vector<Coin> coins;
vector<FireBeam> firebeams;
vector<FireLine> firelines;
vector<WaterBalloon> waterballoons;

bounding_box_t player_box;

// Window Coordinate Dimensions for Player
float RIGHT_EDGE;
float LEFT_EDGE;
float TOP_EDGE;
float BOTTOM_EDGE;

// Movement Restriction Dimensions for Player
// float MOVE_EDGE_R;
// float MOVE_EDGE_L;
const float MOVE_EDGE_T = 3.0;

// Start Coordinates of Player
const float START_X = -2.7;
const float START_Y = -2.95;

bool MOVE_EYE = true;
int PREV_FIRE;
int WATER_REFILL = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() 
{
    // clear the color and depth in the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram(programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye(player.position.x, 0, 5);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target(player.position.x, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up(0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt(eye, target, up); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    platform.draw(VP);
    cieling.draw(VP);
    // Draw coins
    for(int i=0; i<coins.size(); ++i)
    {
        coins[i].draw(VP);
    }
    
    for(int i=0; i<firebeams.size(); ++i)
    {
        for(int i=0; i<firebeams.size(); ++i)
        {
            firebeams[i].draw(VP);
        }    
    }

    for(int i=0; i<firelines.size(); ++i)
    {
        for(int i=0; i<firelines.size(); ++i)
        {
            firelines[i].draw(VP);
        }    
    }
    
    for(int i=0; i<waterballoons.size(); ++i)
    {
        for(int i=0; i<waterballoons.size(); ++i)
        {
            waterballoons[i].draw(VP);
        }    
    }

    player.draw(VP);
}

void tick_input(GLFWwindow* window) 
{
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int a_key = glfwGetKey(window, GLFW_KEY_A);

    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int d_key = glfwGetKey(window, GLFW_KEY_D);
    
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int w_key = glfwGetKey(window, GLFW_KEY_W);
    
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int s_key = glfwGetKey(window, GLFW_KEY_S);

    int x_key = glfwGetKey(window, GLFW_KEY_X);

    if((left || a_key) && player.position.x > -5.0/*&& player.position.x > MOVE_EDGE_L*/) 
    {
        player.position.x -= player.speed_x;
    }
    if((right || d_key) /*&& player.position.x < MOVE_EDGE_R*/)
    {
        player.position.x += player.speed_x; 
    }
    if((up || space || w_key) && player.position.y < MOVE_EDGE_T)
    {
        player.freefall = false;
        player.position.y += player.speed_y;
    }
    else
    {
        player.freefall = true;
    }

    if(!right && !left)
    {
        player.position.x += 0.02;
    }

    if(x_key && (WATER_REFILL == 0))
    {
        WaterBalloon new_balloon = WaterBalloon(player.position.x, player.position.y + 0.2);
        waterballoons.push_back(new_balloon);
        WATER_REFILL = 40;
    }
}

void tick_elements() 
{
    player_box.x = player.position.x;
    player_box.y = player.position.y;

    player.tick();
    platform.tick();
    cieling.tick();
    
    if(WATER_REFILL > 0)
    {
        WATER_REFILL--;
    }

    // Player-Firebeam collision detection
    for(int i=0; i<firebeams.size(); ++i)
    {
        firebeams[i].tick();
        if(!firebeams[i].disabled && detect_collision(firebeams[i].beam.boundary, player_box))
        {
            player.position.y = -3.2;
        }
        else if(firebeams[i].position.x < (player.position.x - 10.0))
        {
            firebeams.erase(firebeams.begin() + i);
            --i;
        }
    }
    
    // Player-Fireline collision detection
    for(int i=0; i<firelines.size(); ++i)
    {
        firelines[i].tick();
        if(!firelines[i].disabled && detect_collision(firelines[i].beam.boundary, player_box))
        {
            player.position.y = -3.2;
        }
        else if(firelines[i].position.x < (player.position.x - 10.0))
        {
            firelines.erase(firelines.begin() + i);
            --i;
        }
    }

    // Player-Coin collision detection
    for (int i = 0; i < coins.size(); ++i)
    {
        if(detect_collision(coins[i].boundary, player_box))
        {
            player.score += 1 * coins[i].multiplier;
            coins.erase(coins.begin() + i);
            --i;
        }
        else if(coins[i].position.x < (player.position.x - 8.0))
        {
            coins.erase(coins.begin() + i);
            --i;
        }
    }
    
    bool water_complete = false;

    for (int i = 0; i < waterballoons.size(); ++i)
    {
        water_complete = false;
        waterballoons[i].tick();
        if(waterballoons[i].position.y <= -3.2)
        {
            waterballoons.erase(waterballoons.begin() + i);
        }
        for(int j = 0; j < firebeams.size(); ++j)
        {
            if(detect_collision(waterballoons[i].boundary, firebeams[j].beam.boundary))
            {
                firebeams[j].disabled = true;
                water_complete = true;
                break;
            }
        }
        if(water_complete)
        {
            waterballoons.erase(waterballoons.begin() + i);
            --i;
        }
        else
        {
            for (int j = 0; j < firelines.size(); ++j)
            {
                if (detect_collision(waterballoons[i].boundary, firelines[j].beam.boundary))
                {
                    firelines[j].disabled = true;
                    water_complete = true;
                    break;
                }
            }
            if(water_complete)
            {
                waterballoons.erase(waterballoons.begin() + i);
                --i;
            }
        }
    }

    // Generate Coins
    Coin new_coin;
    int sign_determinor_x;
    int sign_determinor_y;
    int random_colour;

    srand(time(0));
    while(coins.size() < 30)
    {
        sign_determinor_x = (rand() % 2 == 0) ? 1 : -1;
        sign_determinor_y = (rand() % 2 == 0) ? 1 : -1;
        random_colour = rand() % 3;

        switch (random_colour)
        {
        case 0:
            new_coin = Coin(((rand() % 6) * sign_determinor_x) + player.position.x + 12.0, (rand() % 4) * sign_determinor_y, COLOR_YELLOW);
            break;
        case 1:
            new_coin = Coin(((rand() % 6) * sign_determinor_x) + player.position.x + 12.0, (rand() % 4) * sign_determinor_y, COLOR_ORANGE);
            break;
        case 2:
            new_coin = Coin(((rand() % 6) * sign_determinor_x) + player.position.x + 12.0, (rand() % 4) * sign_determinor_y, COLOR_GREEN);
            break;
        default:
            break;
        }

        coins.push_back(new_coin);
    }

    FireBeam new_beam;
    FireLine new_line;
    int new_x;
    int rand_length;
    int rand_angle;

    srand(time(0));
    while(firebeams.size() + firelines.size() < 12)
    {
        new_x = PREV_FIRE + 8.0 + (rand() % 5);
        sign_determinor_x = (rand() % 2 == 0) ? 1 : -1;
        sign_determinor_y = (rand() % 2 == 0) ? 1 : -1;
        rand_length = (rand() % 3) + 1;

        switch (sign_determinor_x)
        {
        case 1:
            new_beam = FireBeam(new_x, (rand() % 3) * sign_determinor_y, rand_length);
            firebeams.push_back(new_beam);
            break;
        case -1:
            rand_angle = rand() % 50;
            new_line = FireLine(new_x, (rand() % 2) * sign_determinor_y, rand_length, rand_angle);
            firelines.push_back(new_line);
            break;
        default:
            break;
        }

        PREV_FIRE = new_x;
    }

    cout << "Score: " << player.score << endl;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow* window, int width, int height) 
{
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    player = Player(START_X, START_Y, COLOR_BLUE);
    platform = Platform(0.0, -3.7, COLOR_GREY);
    cieling = Cieling(0.0, 3.8, COLOR_GREY);

    // Generate Coins
    Coin new_coin;
    int sign_determinor_x;
    int sign_determinor_y;
    int random_colour;

    srand(time(0));
    while (coins.size() < 30)
    {
        sign_determinor_x = (rand() % 2 == 0) ? 1 : -1;
        sign_determinor_y = (rand() % 2 == 0) ? 1 : -1;
        random_colour = rand() % 3;

        switch (random_colour)
        {
        case 0:
            new_coin = Coin((rand() % 6) * sign_determinor_x, (rand() % 3) * sign_determinor_y, COLOR_YELLOW);
            break;
        case 1:
            new_coin = Coin((rand() % 6) * sign_determinor_x, (rand() % 3) * sign_determinor_y, COLOR_ORANGE);
            break;
        case 2:
            new_coin = Coin((rand() % 6) * sign_determinor_x, (rand() % 3) * sign_determinor_y, COLOR_GREEN);
            break;
        default:
            break;
        }

        coins.push_back(new_coin);
    }
    
    // Generating Fire beams
    PREV_FIRE = player.position.x + 10.0;
    FireBeam new_beam;
    FireLine new_line;
    int new_x;
    int rand_length;
    int rand_angle;

    srand(time(0));
    while(firebeams.size() + firelines.size() < 12)
    {
        new_x = PREV_FIRE + 8.0 + (rand()%5);
        sign_determinor_x = (rand() % 2 == 0) ? 1 : -1;
        sign_determinor_y = (rand() % 2 == 0) ? 1 : -1;
        rand_length = (rand() % 3) + 1;

        switch(sign_determinor_x)
        {
            case 1:
                new_beam = FireBeam(new_x, (rand() % 3) * sign_determinor_y, rand_length);
                firebeams.push_back(new_beam);
                break;
            case -1:
                rand_angle = rand() % 50;
                new_line = FireLine(new_x, (rand() % 2) * sign_determinor_y, rand_length, rand_angle);
                firelines.push_back(new_line);
                break;
            default:
                break;
        }

        PREV_FIRE = new_x;
    }

    // Player boundary box
    player_box.width = 0.6;
    player_box.height = 1.2;
    player_box.x = player.position.x;
    player_box.y = player.position.y;

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv) 
{
    srand(time(0));
    int width = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL(window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) 
    {
        // Process timers

        if(t60.processTick()) 
        {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) 
{
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() 
{
    float top = SCREEN_CENTER_Y + 4 / SCREEN_ZOOM;
    float bottom = SCREEN_CENTER_Y - 4 / SCREEN_ZOOM;
    float left = SCREEN_CENTER_X - 4 / SCREEN_ZOOM;
    float right = SCREEN_CENTER_X + 4 / SCREEN_ZOOM;

    RIGHT_EDGE = right - PLAYER_WIDTH;
    LEFT_EDGE = left + PLAYER_WIDTH;
    TOP_EDGE = top - PLAYER_HEIGHT;
    BOTTOM_EDGE = bottom + PLAYER_HEIGHT;

    // MOVE_EDGE_R = RIGHT_EDGE - PLAYER_WIDTH;
    // MOVE_EDGE_L = LEFT_EDGE + PLAYER_WIDTH;

    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
