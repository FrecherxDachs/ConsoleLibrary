#include <iostream>
#include "../ConsoleLibrary/Console.h"
#include "../ConsoleLibrary/Menu.h"
#include "Breakout.h"

#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
using namespace std;

#define WIDTH 50
#define HEIGHT 40

enum States { MENU, RUN, PAUSE, SCORES };

void keyFunction(WORD keyCode);
void timerFunction();

void initMenu();
void render();

Console* console;
Breakout* breakout;

int titleColor;
int pauseColor;

States state = MENU;
Menu* menu;

vector<int> scores;


// DEBUG
//ofstream logFile;

int main(int argc, char **argv) {
    srand ( time_t(NULL) );

    //logFile.open("breakout.log");
    scores = vector<int>();

    console = new Console("Breakout", WIDTH, HEIGHT, WHITE, BLACK);

    titleColor = console->createColor(BLUE, BLACK);
    pauseColor = console->createColor(WHITE, BLACK);

    initMenu();
    render();

    console->registerKeyEvent(&keyFunction);
    console->run();

    //logFile.close();
    
    delete console;
    delete breakout;
    
    return 0;
}

void keyFunction(WORD keyCode)
{
    switch (state)
    {
    case MENU:

        if (keyCode == VK_UP)
        {
            menu->selectPrevious();
        }
        else if (keyCode == VK_DOWN)
        {
            menu->selectNext();
        }
        else if (keyCode == VK_RETURN)
        {
            switch (menu->getSelectedItem())
            {
            case 0:
                state = RUN;
                breakout = new Breakout(HEIGHT, WIDTH, console);
                console->registerTimerEvent(&timerFunction, 50);
                break;

            case 1:
                state = SCORES;
                break;

            case 2:
                console->stop();
                return;
            }
        }
        else if (keyCode == VK_ESCAPE)
        {
            console->stop();
            return;
        }
        render();

        break;


    case RUN:

        if (keyCode == VK_RIGHT)
        {
            breakout->movePaddle(RIGHT);
        }
        else if (keyCode == VK_LEFT)
        {
            breakout->movePaddle(LEFT);
        }
        else if (keyCode == VK_ESCAPE)
        {
            if(breakout->score > 0)
                scores.push_back(breakout->score);
            state = MENU;
            console->registerTimerEvent(NULL, 0);
            render();
        }
        else if (keyCode == VK_SPACE)
        {
            state = PAUSE;
            console->registerTimerEvent(NULL, 0);
            render();
        }
        else
        {
            return;
        }
        break;

    case SCORES:
        if (keyCode == VK_ESCAPE)
        {
            state = MENU;
            render();
        }
        break;

    case PAUSE:

        if (keyCode == VK_SPACE)
        {
            state = RUN;
            console->registerTimerEvent(&timerFunction, 50);
        }
        render();
        break;
    }
}

void timerFunction()
{
    if(!breakout->running) {
        if(breakout->score > 0)
            scores.push_back(breakout->score);
        state = MENU;
        console->registerTimerEvent(NULL, 0);
    }
    breakout->tick();
    render();
}


void initMenu()
{
    COLOR_ID selColor = console->createColor(YELLOW, RED);
    COLOR_ID itemColor = console->createColor(WHITE, BLACK);
    menu = new Menu(console, selColor);
    menu->addItem("Start", 22, 20, itemColor);
    menu->addItem("Scores", 22, 25, itemColor);
    menu->addItem("Quit", 22, 30, itemColor);

    menu->select(0);
}

void render()
{
    console->clearConsole();
    switch (state)
    {
    case MENU:

        console->printText(7, 3, "***  ***  ****  **  *  *  ***  *   * *****", titleColor);
        console->printText(7, 4, "*  * *  * *    *  * * *  *   * *   *   *  ");
        console->printText(7, 5, "***  ***  **** **** **   *   * *   *   *  ");
        console->printText(7, 6, "*  * * *  *    *  * * *  *   * *   *   *  ");
        console->printText(7, 7, "***  *  * **** *  * *  *  ***   ***    *  ");

        menu->show();
        break;

    case RUN:
        breakout->render();
        break;

    case SCORES:
        console->printText(10, 3, " ***  ***  ***  ***  ****  ***", titleColor);
        console->printText(10, 4, "*    *    *   * *  * *    *   ");
        console->printText(10, 5, " **  *    *   * ***  ****  ** ");
        console->printText(10, 6, "   * *    *   * * *  *       *");
        console->printText(10, 7, "***   ***  ***  *  * **** *** ");

        for(int i=0; i<scores.size(); i++) {
            console->setTile(15, 10 + i *2, 48 + (i + 1));
            console->setTile(16, 10 + i *2, ':');
            ostringstream outStream;
            outStream << scores.at(i);
            console->printText(18, 10 + i *2, outStream.str().c_str());
        }
        break;

    case PAUSE:

        //console->setColor(WHITE, BLACK);
        console->printText(20, 19, "#########", pauseColor);
        console->printText(20, 20, "# PAUSE #");
        console->printText(20, 21, "#########");

        break;
    }

    console->redraw();
}