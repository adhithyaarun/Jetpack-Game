#include "main.h"
#include "player.h"
#include "platform.h"
#include "coin.h"
#include "shape.h"
#include "fire.h"
#include "waterballoon.h"
#include "powerup.h"
#include "display.h"
#include "magnet.h"
#include "boomerang.h"
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
Multiplier multiplier;
Shield shield;
Display display;
Digit stage;
Magnet magnet;
Boomerang boom;

bounding_box_t player_box;

int STAGE = 1;

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
bool POWER_UP = false;
bool MULTIPLIER = false;
bool SHIELD = false;
bool MAGNET = false;
bool BOOMERANG = false;

Timer t60(1.0 / 60);
Timer power_up;
Timer magnet_timer;

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
    if(MULTIPLIER && !multiplier.collected)
    {
        multiplier.draw(VP);
    }
    else if(SHIELD && !shield.collected)
    {
        shield.draw(VP);
    }

    if(MAGNET)
    {
        magnet.draw(VP);
    }

    if(BOOMERANG)
    {
        boom.draw(VP);
    }

    display.draw(VP);
    stage.draw(VP);

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
    
    if(player.score < 0)
    {
        player.score = 0;
    }

    if(WATER_REFILL > 0)
    {
        WATER_REFILL--;
    }

    // Collisions
    // Harmful
    // Player-Firebeam collision detection
    for(int i=0; i<firebeams.size(); ++i)
    {
        firebeams[i].tick();
        if(!(SHIELD && shield.collected) && !firebeams[i].disabled && detect_collision(firebeams[i].beam.boundary, player_box))
        {
            // player.position.y = -3.2;
            player.score -= 100;
        }
        else if(firebeams[i].position.x < (player.position.x - 10.0))
        {
            firebeams.erase(firebeams.begin() + i);
            --i;
        }
    }
    
    fireline_box_t fire_box;

    // Player-Fireline collision detection
    for(int i=0; i<firelines.size(); ++i)
    {
        firelines[i].tick();
        fire_box.x = firelines[i].position.x;
        fire_box.y = firelines[i].position.y;
        fire_box.length = firelines[i].length;
        fire_box.angle = firelines[i].angle;

        if (!(SHIELD && shield.collected) && !firelines[i].disabled && detect_fire(player_box, fire_box))
        {
            // player.position.y = -3.2;
            player.score -= 100;
        }
        else if(firelines[i].position.x < (player.position.x - 10.0))
        {
            firelines.erase(firelines.begin() + i);
            --i;
        }
    }

    // Harmless
    // Player-Coin collision detection
    for (int i = 0; i < coins.size(); ++i)
    {
        coins[i].tick();
        if(detect_collision(coins[i].boundary, player_box))
        {
            player.score += coins[i].multiplier;
            if(MULTIPLIER && multiplier.collected)
            {
                player.score += coins[i].multiplier;
            }
            coins.erase(coins.begin() + i);
            --i;
        }
        else if(coins[i].position.x < (player.position.x - 8.0))
        {
            coins.erase(coins.begin() + i);
            --i;
        }
    }
    
    // Waterballoon-Fire collision detection
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
                fire_box.x = firelines[j].position.x;
                fire_box.y = firelines[j].position.y;
                fire_box.length = firelines[j].length;
                fire_box.angle = firelines[j].angle;
                if (detect_fire(waterballoons[i].boundary, fire_box))
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

    // Player-Multiplier collision detection
    if(MULTIPLIER && detect_collision(player_box, multiplier.boundary))
    {
        multiplier.collected = true;
        power_up = Timer(15.0);
    }
    
    // Player-Shield collision detection
    if(SHIELD && detect_collision(player_box, shield.boundary))
    {
        shield.collected = true;
        power_up = Timer(15.0);
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

    // Generate Firebeams & Firelines
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

    // Generate power-up
    int rand_power_decider;
    int rand_power;

    rand_power_decider = rand();
    if(POWER_UP && rand_power_decider % 20 == 0)
    {
        POWER_UP = false;
        rand_power = rand();

        if(rand_power % 2 == 0)
        {
            shield = Shield(player.position.x + 20.0, 1.5);
            SHIELD = true;
        }
        else
        {
            multiplier = Multiplier(player.position.x + 20.0, 1.5);
            MULTIPLIER = true;
        }
    }

    if(!POWER_UP)
    {
        if((multiplier.collected || shield.collected) && power_up.processTick())
        {
            POWER_UP = true;
        }
        if(MULTIPLIER)
        {
            if(!POWER_UP)
            {
                multiplier.tick();
                if(multiplier.position.x < player.position.x && abs(multiplier.position.x - player.position.x) > 10.0)
                {
                    MULTIPLIER = false;
                    POWER_UP = true;
                }
            }
            else
            {
                MULTIPLIER = false;
            }
        }
        else if(SHIELD)
        {
            if(!POWER_UP)
            {
                shield.tick();
                if(shield.position.x < player.position.x && abs(shield.position.x - player.position.x) > 10.0)
                {
                    SHIELD = false;
                    POWER_UP = true;
                }
            }
            else
            {
                SHIELD = false;
            }
        }
    }

    int rand_magnet_decider = rand();
    int rand_magnet_x;
    int rand_magnet_y;
    int magnet_y_sign;

    // Generate Magnet
    if(!MAGNET && rand_magnet_decider % 6 == 0)
    {
        MAGNET = true;
        rand_magnet_x = (rand() % 10) + 5;
        magnet_y_sign = (rand() % 2) == 0 ? 1 : -1;
        rand_magnet_x = (rand() % 3) * magnet_y_sign;
        magnet = Magnet(abs(player.position.x) + rand_magnet_x, rand_magnet_y);
        magnet_timer = Timer(15.0);
    }
    else if(MAGNET)
    {
        if(magnet_timer.processTick())
        {
            MAGNET = false;
        }
        else
        {
            // Player-Magnet attraction
            if( (player.position.x+0.2) >= magnet.position.x && 
                (player.position.x <= (magnet.position.x + 0.8)))
            {
                if(player.position.y > (magnet.position.y + 0.2))
                {
                    player.position.y -= magnet.power;
                }   
                else
                {
                    player.position.y += magnet.power;
                }
            }
        }
    }   

    int boom_decider = rand();
    int boom_height = (rand() % 2) + 1;

    if(!BOOMERANG && boom_decider % 5 == 0)
    {
        BOOMERANG = true;
        boom = Boomerang(abs(player.position.x) + 6.0, boom_height);
    }

    if(BOOMERANG)
    {
        if(boom.position.x == boom.start_x)
        {
            BOOMERANG = false;
        }
        else
        {
            boom.tick();
        }
        
    }

    
    if(STAGE == 1 && player.score >= 1000)
    {
        STAGE = 2;
    }
    else if(STAGE == 2 && player.score >= 5000)
    {
        STAGE = 3;
    }
    else if(STAGE == 3 && player.score >= 15000)
    {
        STAGE = 4;
    }

    display.tick(player.position.x, (int) player.score);
    stage.tick(player.position.x + 5.0, STAGE);

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
    display = Display(player.position.x, 4.0, player.score);
    stage = Digit(player.position.x + 5.0, 4.0, STAGE);

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

bool detect_fire(bounding_box_t _player, fireline_box_t fireline) 
{
    /* 
        Using the equation of a line: 
                y = mx + c
    */
    float radian = fireline.angle * M_PI / 180.0;
    float y_intercept = fireline.y - (fireline.x * tan(radian));

    float x_lower = (_player.y - (_player.height / 2.0) - y_intercept) / tan(radian);
    float x_upper = x_lower + (2.0 * (_player.height / 2.0)) / tan(radian);

    float y_left = y_intercept + tan(radian) * (_player.x - (_player.width / 2.0));
    float y_right = y_left + tan(radian) * 2.0 * (_player.width / 2.0);
    
    // Checking y coordinate range
    if( y_right > fireline.y && 
        y_right < (fireline.y + fireline.length * sin(radian)) &&
        y_right < (_player.y + (_player.height/2.0)) && 
        y_right > (_player.y - (_player.height/2.0)))
    {
       return true;
    }
    if( y_left > fireline.y && 
        y_left < (fireline.y + fireline.length * sin(radian)) && 
        y_left < (_player.y + (_player.height/2.0)) && 
        y_left > (_player.y - (_player.height/2.0)))
    {
       return true;
    }

    // Checking x coordinate range
    if( x_upper > fireline.x &&
        x_upper < (fireline.x + fireline.length * cos(radian)) && 
        x_upper < (_player.x + (_player.width/2.0)) && 
        x_upper > (_player.x - (_player.width/2.0)))
    {
       return true;
    }
    if( 
        x_lower < (fireline.x + fireline.length * cos(radian)) && 
        x_lower > fireline.x && 
        x_lower < (_player.x + (_player.width/2.0)) && 
        x_lower > (_player.x - (_player.width/2.0)))
    {
       return true;
    }

    return false;
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
