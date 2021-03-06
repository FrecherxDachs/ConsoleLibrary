

#include "../ConsoleLibrary/Console.h"
#include "Shape.h"

#include <vector>
#include <time.h>
#include <stdlib.h>
#include <cstdio>


#define WIDTH 50
#define HEIGHT 40

#define MATRIX_WIDTH 10
#define MATRIX_HEIGHT 20

#define DEFAULT_SPEED 10
#define SPEED_INCREASE_PER_LEVEL 2

#define LINES_TO_INCREASE_LEVEL 10

enum States { RUN, PAUSE };


void keyFunction(WORD keyCode);
void timerFunction();

void initLevel();
void checkForFullLines();
void calcScore(int combinedLines);
void createNewBrick();
void render();
void renderPanel();


Console* console;
Matrix* matrix;
Shape* shape;
Shape* nextShape;

COLOR_ID defaultColor;
COLOR_ID infoColor;
COLOR_ID textColor;

States state;

int currentLevel;
int currentSpeed;
int score;
int deletedLines;


int main(int argc, char* argv[])
{
    srand( (unsigned int) time(NULL) );

    console = new Console("Tetris", WIDTH, HEIGHT, WHITE, BLACK);
    matrix = new Matrix(MATRIX_WIDTH, MATRIX_HEIGHT);

    defaultColor = console->createColor(WHITE, BLACK);
    infoColor = console->createColor(WHITE, RED);
    textColor = console->createColor(BLUE, RED);

    state = RUN;

    console->registerKeyEvent(&keyFunction);
    console->registerTimerEvent(&timerFunction, 50);

    initLevel();

    console->run();

    delete nextShape;
    delete matrix;
    delete console;
}


void initLevel()
{
    currentSpeed = DEFAULT_SPEED;
    currentLevel = 1;

    deletedLines = 0;
    score = 0;

    matrix->reset();

    // init current and next shape
    createNewBrick();
    createNewBrick();
}


void keyFunction(WORD keyCode)
{
    if (state == RUN)
    {
        if (keyCode == VK_LEFT)
        {
            shape->moveLeft();
        }
        else if (keyCode == VK_RIGHT)
        {
            shape->moveRight();
        }
        else if (keyCode == VK_DOWN)
        {
            shape->moveDown();
        }
        else if (keyCode == VK_UP)
        {
            shape->rotateLeft();
        }
        else if (keyCode == VK_SPACE)
        {
            //shape->rotateRight();
            console->registerTimerEvent(nullptr, 0);
            state = PAUSE;
            render();
            return;
        }
        else if (keyCode == VK_ESCAPE)
        {
            console->stop();
        }
    }
    else if (state == PAUSE)
    {
        if (keyCode == VK_SPACE)
        {
            state = RUN;
            console->registerTimerEvent(&timerFunction, 50);
            render();
            return;
        }
    }
}


void timerFunction()
{
    // is shape already placed on the matrix
    if (shape->checkForCollision(shape->x, shape->y))
    {
        delete shape;
        initLevel();
    }

    // if the brick wasn't updated create a new one
    if (!shape->update())
    {
        createNewBrick();
    }

    checkForFullLines();

    render();
}


void checkForFullLines()
{
    int combinedLineCounter = 0;

    for (int y = MATRIX_HEIGHT - 1; y >= 0; y--)
    {
        // search for a full line
        int x;
        for (x = 0; x < MATRIX_WIDTH; x++)
        {
            if (!matrix->field[y][x])
            {
                break;
            }
        }

        // delete the found line and move all bricks above downwards
        if (x == MATRIX_WIDTH)
        {
            combinedLineCounter++;

            for (int cy = y - 1; cy > 0; cy--)
            {
                for (int cx = 0; cx < MATRIX_WIDTH; cx++)
                {
                    matrix->field[cy + 1][cx] = matrix->field[cy][cx];
                }
            }

            for (x = 0; x < MATRIX_WIDTH; x++)
            {
                matrix->field[0][x] = false;
            }

            // do not ignore the next line
            y++;
        }
        
        // update score and level
        else if (combinedLineCounter > 0)
        {
            deletedLines += combinedLineCounter;
            calcScore(combinedLineCounter);
            combinedLineCounter = 0;
        }
    }

    // cgeck again, update score and level
    if (combinedLineCounter > 0)
    {
        deletedLines += combinedLineCounter;
        calcScore(combinedLineCounter);
        combinedLineCounter = 0;
    }

    // every x removed lines increase the level
    if (deletedLines >= currentLevel * LINES_TO_INCREASE_LEVEL)
    {
        currentLevel++;
        currentSpeed += SPEED_INCREASE_PER_LEVEL;
    }
}


void calcScore(int combinedLines)
{
    switch (combinedLines)
    {
    case 1:
        score += currentLevel * 40;
        break;

    case 2:
        score += currentLevel * 100;
        break;

    case 3:
        score += currentLevel * 300;
        break;

    case 4:
        score += currentLevel * 1200;
        break;
    }
}


void render()
{
    console->clearConsole();

    if (state == RUN)
    {
        int frameWidth = (WIDTH - 2 * MATRIX_WIDTH) / 2;

        // draw interface
        console->setColor(infoColor);
        for (int x = 0; x < frameWidth; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                console->setTile(x, y, ' ');
            }
        }
        for (int x = WIDTH - frameWidth; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                console->setTile(x, y, ' ');
            }
        }


        // brick matrix
        console->setColor(defaultColor);
        for (int y = 0; y < MATRIX_HEIGHT; y++)
        {
            for (int x = 0; x < MATRIX_WIDTH; x++)
            {
                if (matrix->field[y][x])
                {
                    console->setTile(frameWidth + 2 * x,     2 * y,     '#');
                    console->setTile(frameWidth + 2 * x + 1, 2 * y,     '#');
                    console->setTile(frameWidth + 2 * x,     2 * y + 1, '#');
                    console->setTile(frameWidth + 2 * x + 1, 2 * y + 1, '#');
                }
            }
        }

        for (Point &point : *shape->blocks)
        {
            int x = frameWidth + 2 * (shape->x + point.x);
            int y = 2 * (shape->y + point.y);
            console->setTile(x,     y,     '+');
            console->setTile(x + 1, y,     '+');
            console->setTile(x,     y + 1, '+');
            console->setTile(x + 1, y + 1, '+');
        }

        renderPanel();
    }
    else
    {
        console->printText(20, 19, "#########", defaultColor);
        console->printText(20, 20, "# PAUSE #");
        console->printText(20, 21, "#########");
    }

    console->redraw();
}


void createNewBrick()
{
    shape = nextShape;
    nextShape = new Shape(matrix, currentSpeed);
}


void renderPanel()
{
    int rightPanel = WIDTH - (WIDTH / 2 - MATRIX_WIDTH);

    // render next brick
    console->printText(rightPanel + 2, 2, "NEXT:", textColor);

    for (Point &point : *nextShape->blocks)
    {
        console->setTile(rightPanel + 2 + point.x, 4 + point.y, '#');
    }


    // render score
    console->printText(rightPanel + 2, 10, "SCORE:");

    char scoreString[20];
    sprintf_s<20>(scoreString, "%d", score);

    console->printText(rightPanel + 2, 12, scoreString);


    // render level
    char levelString[20];
    sprintf_s<20>(levelString, "Level: %d", currentLevel);
    console->printText(rightPanel + 2, 15, levelString);
}