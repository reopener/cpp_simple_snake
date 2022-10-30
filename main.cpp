// This is my study project for Aram High School

#include <regfxlib/core.hpp>
#include <regfxlib/io.hpp>

#include <iostream>

using namespace regfxlib;
using namespace std;

// Function for drawing the main game map
// Accepts a screen on which to draw and a completed game map

void drawMap(Screen scr, vector<vector<int>> gameMap)
{
    // Iterate over the map line by line
    for (int i = 0; i < gameMap.size(); i++)
    {
        // Iterate over the map by columns
        for (int j = 0; j < gameMap[i].size(); j++)
        {
            // Empty cell, draw the grid
            if (gameMap[i][j] == 0)
            {
                rectangle(scr, i * 40, j * 40, (i + 1) * 40, (j + 1) * 40, RGB(0, 255, 255), THIN_LINE);
            }
            // Snake, draw the snake
            if (gameMap[i][j] == 1)
            {
                rectangle(scr, i * 40, j * 40, (i + 1) * 40, (j + 1) * 40, RGB(0, 255, 0), FILL0);
            }
            // Apple, draw apple and grid
            if (gameMap[i][j] == 2)
            {
                rectangle(scr, i * 40, j * 40, (i + 1) * 40, (j + 1) * 40, RGB(0, 255, 255), THIN_LINE);
                circle(scr, i * 40, j * 40, 40 / 2, RGB(255, 0, 0), FILL0);
            }
        }
    }
}

int main()
{
    // Create an empty field
    Screen scr = createDrawableScreen(400, 400, 0);

    // Waiting for the player to press F to start the game
    scr.WriteText("PRESS 'F' TO START", scr.width / 2, scr.height / 2, RGB(255, 0, 0), 17, FONT_DEFAULT0);
    createWindowFromScreen("Snake game", 0, 0, scr);

    while (ioKeyPressed() != KEYS_LETTER_F)
    {
        // Magic digit
        wait(33);
    }

    // Declare a snake direction variable, 0 - up, 1 - right, 2 - down, 3 - left
    int direction = 0;
    // The game is too fast, variable to slow down the game, the more the slower
    int speed = 333;

    // Create an empty map
    vector<vector<int>> gameMap = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    // Snake is a vector that contains the x,y coordinate vectors of the segments of the snake
    vector<vector<int>> snake = {{3, 3}};
    // Apple is a vector that contains the x,y coordinate
    vector<int> apple = {rand() % 10, rand() % 10};

    // Timer
    Timer tm;
    tm.start();

    // Main game loop
    while (true)
    {
        // If the direction is up or down, can allow to change the direction to left or right
        if (direction == 0 || direction == 2)
        {
            if (ioKeyPressed() == KEYS_RIGHT_ARROW)
            {
                direction = 1;
            }
            if (ioKeyPressed() == KEYS_LEFT_ARROW)
            {
                direction = 3;
            }
        }
        // If the direction is left or right , can allow to change the direction to up or down
        if (direction == 1 || direction == 3)
        {
            if (ioKeyPressed() == KEYS_UP_ARROW)
            {
                direction = 0;
            }
            if (ioKeyPressed() == KEYS_DOWN_ARROW)
            {
                direction = 2;
            }
        }

        // Clearing the screen from the previous frame
        scr.clear();

        // Get the value of the timer and take the remainder of the division by the speed
        // Dropping frames to slow down the game
        if (tm.getMs() % speed == 0)
        {
            // Snake old head
            vector<int> oldHead = snake[0];
            // Snake empty new head
            vector<int> newHead = {0, 0};

            // If direction UP set new head upper old head
            if (direction == 0)
            {
                newHead[0] = oldHead[0];
                newHead[1] = oldHead[1] - 1;
                // If the new head is behind the screen, move it to the opposite side
                if (newHead[1] < 0)
                {
                    newHead[1] = 9;
                }
            }
            if (direction == 1)
            {
                newHead[0] = oldHead[0] + 1;
                newHead[1] = oldHead[1];
                if (newHead[0] > 9)
                {
                    newHead[0] = 0;
                }
            }
            if (direction == 2)
            {
                newHead[0] = oldHead[0];
                newHead[1] = oldHead[1] + 1;
                if (newHead[1] > 9)
                {
                    newHead[1] = 0;
                }
            }
            if (direction == 3)
            {
                newHead[0] = oldHead[0] - 1;
                newHead[1] = oldHead[1];
                if (newHead[0] < 0)
                {
                    newHead[0] = 9;
                }
            }

            // Adding a new head to the snake
            snake.insert(snake.begin(), newHead);

            // If the new head is in the same place as the apple
            if (newHead[0] == apple[0] && newHead[1] == apple[1])
            {
                int nextApple = 1;
                // Create a new apple in a different place
                // Repeat until set on the snake
                while (nextApple == 1)
                {
                    nextApple = 0;
                    apple = {rand() % 10, rand() % 10};
                    for (int i = 1; i < snake.size(); i++)
                    {
                        if (apple[0] == snake[i][0] && apple[1] == snake[i][1])
                        {
                            nextApple = 1;
                        }
                    }
                }
            }
            else
            {
                // If there was no apple, remove the tail
                gameMap[snake.back()[0]][snake.back()[1]] = 0;
                snake.pop_back();
            }

            // Two section snake can't bite itself
            if (snake.size() > 2)
            {
                for (int i = 1; i < snake.size(); i++)
                {
                    // If the snake bites itself then it's game over.
                    if (newHead[0] == snake[i][0] && newHead[1] == snake[i][1])
                    {
                        scr.clear();
                        scr.WriteText("GAME OVER", scr.width / 2, scr.height / 2, RGB(255, 0, 0), 17, FONT_DEFAULT0);
                        wait(10000);
                        return 0;
                    }
                }
            }
        }

        // Place the apple on the playing field
        gameMap[apple[0]][apple[1]] = 2;

        // Place the snake on the playing field
        for (int i = 0; i < snake.size(); i++)
        {
            gameMap[snake[i][0]][snake[i][1]] = 1;
        }

        // Run draw function
        drawMap(scr, gameMap);
        // Rerender window 
        createWindowFromScreen("Snake game", 0, 0, scr);
    }
    return 0;
}
