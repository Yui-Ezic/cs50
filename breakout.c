//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// width of a paddle
#define PADDLE_WIDTH 60

// height of a paddle
#define PADDLE_HEIGHT 10

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    double yspeed = 1.5;
    double xspeed = drand48() + 0.5;

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {      
        // get the mouse or action event
        GEvent event = getNextEvent(MOUSE_EVENT + ACTION_EVENT);
        
        if(event != NULL)
        {
            // if window was closed
            if(getEventType(event) == WINDOW_CLOSED)
            {
                break;
            }
            
            // if mouse moved
            if(getEventType(event) == MOUSE_MOVED)
            {
                // move the paddle for mouse
                double x = getX(event) - getWidth(paddle) / 2;
                
                if (x > 0 && x < WIDTH - getWidth(paddle))
                {
                    setLocation(paddle, x, getY(paddle));
                }
            }
            
            // Releases the storage associated with the event
            freeEvent(event);
        }
        
        // move the ball
        move(ball, xspeed, yspeed);
        
        // detect collision with edge of window
        // left edge
        if(getX(ball) <= 0)
        {
            xspeed = -xspeed;
        } 
        // right edge
        else if(getX(ball) + getWidth(ball) >= WIDTH)
        {
            xspeed = -xspeed;
        }
        
        if(getY(ball) + getHeight(ball) >= HEIGHT)
        {
            lives--;
            if(lives > 0)
            {
                setLocation(ball, WIDTH / 2 - RADIUS, HEIGHT / 2 - RADIUS); 
                waitForClick();
            }
            else
            {
                break;
            }
        }
        else if(getY(ball) <= 0)
        {
            yspeed = -yspeed;
        }
        
        GObject object = detectCollision(window, ball);
 
        if (object != NULL)
        {
            // detect the collision with paddle
            if (object == paddle)
            {
                yspeed = -yspeed;
            }
            // detect the collision with brick
            else if (strcmp(getType(object), "GRect") == 0)
            {
                removeGWindow(window, object);
                yspeed = -yspeed;
                bricks--;
                points++;
                //update the Scoreboard
                updateScoreboard(window, label, points);
            }
        }
        
        pause(5);
    }

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // first brick location
    double x = 2;
    double y = 10;
    
    // size of brick
    double width = WIDTH / COLS - 4;
    double height = 10;
    
    for(int row = 0; row < ROWS; row++)
    {
        x = 2;
        for(int col = 0; col < COLS; col++)
        {
            // create new brick
            GRect brick = newGRect(x, y, width, height);
            
            // set the color of brick
            switch(row % 4)
            {
                case 0:
                    setColor(brick, "RED");
                    break;
                case 1:
                    setColor(brick, "BLUE");
                    break;
                case 2:
                    setColor(brick, "MAGENTA");
                    break;
                default:    
                    setColor(brick, "YELLOW");
                    break;
            }
            setFilled(brick, true);
            
            // display the brick
            add(window, brick);
            
            x += width + 4;
        }
        y += height + 5;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // start location of ball
    double x = WIDTH / 2 - RADIUS;
    double y = HEIGHT / 2 - RADIUS;
    
    // create the ball
    GOval ball = newGOval(x, y, RADIUS*2, RADIUS*2);
    
    // set the color of ball
    setColor(ball, "BLACK");    
    setFilled(ball, true);
    
    // display ball
    add(window, ball);
    
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // the paddle start location
    double x = WIDTH / 2 - PADDLE_WIDTH / 2;
    double y = HEIGHT - PADDLE_HEIGHT - 10;
    
    // create the paddle
    GRect paddle = newGRect(x, y, PADDLE_WIDTH, PADDLE_HEIGHT);
    
    // set the color of the paddle
    setColor(paddle, "GREEN");
    setFilled(paddle, true);
    
    // display the paddle
    add(window, paddle);
    
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // create Scoreboard
    GLabel Scoreboard = newGLabel("0");
    setFont(Scoreboard, "SansSerif-64");
    setColor(Scoreboard, "GRAY");
    
    updateScoreboard(window, Scoreboard, 0);
    
    add(window, Scoreboard);
    
    return Scoreboard;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
