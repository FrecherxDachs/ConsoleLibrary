#include "../ConsoleLibrary/Console.h"
#include "../ConsoleLibrary/Menu.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <stdlib.h>

#define WIDTH 50
#define HEIGHT 40

#define BUFFER_SIZE 1024

#ifndef WIN32
#define strncpy_s(src, size, dest, length) strncpy(src, dest, length);
#endif

using namespace std;


void keyFunction(WORD keyCode);
void timerFunction();

char* copyString(const char* str);


vector<string*> programPaths;

Console* console;
Menu* menu;

COLOR_ID defaultColor;


int main(int argc, char* argv[])
{
    //ofstream logFile;

    int* a = nullptr;

    ifstream manifestFile;
    char buffer[BUFFER_SIZE];
    
    manifestFile.open("manifest.txt", ios::in);

    if (manifestFile.good())
    {
        console = new Console("Program Launcher", WIDTH, HEIGHT, WHITE, BLACK);

        COLOR_ID defaultColor = console->createColor(WHITE, BLACK);
        COLOR_ID selectedItemColor = console->createColor(YELLOW, RED);

        menu = new Menu(console, selectedItemColor);

        manifestFile.seekg(0L, ios::beg);

        int i = 0;
        while (!manifestFile.eof())
        {
            manifestFile.getline(buffer, BUFFER_SIZE);
            string line(buffer);
            string::size_type index = line.find_first_of(':');

            if (index != string::npos)
            {
                menu->addItem(copyString(line.substr(0, index).c_str()), 20, 15 + i++ * 3, defaultColor);
                programPaths.push_back(new string(line.substr(index + 1)));
            }
        }

        manifestFile.close();

        menu->addItem("Exit", 20, 15 + menu->getLength() * 3, defaultColor);
        menu->select(0);

        console->registerKeyEvent(&keyFunction);
        console->registerTimerEvent(&timerFunction, 50);

        console->run();

        delete menu;
        delete console;
    }
    else
    {
        cout << "There is a problem with your 'manifest.txt'!\n";
        getchar();
    }

    return 0;
}


void keyFunction(WORD keyCode)
{
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
        WORD index = menu->getSelectedItem();

        if (index == menu->getLength() - 1)
        {
            console->stop();
            return;
        }
        else
        {
            const char* path = (*(programPaths.begin() + index))->c_str();
        
            system(path);
        }
    }
    else if (keyCode == VK_ESCAPE)
    {
        console->stop();
    }
}


void timerFunction()
{
    console->clearConsole();

    console->printText(6, 4, "#    ##  #  # #  #  ## #  # #### ###", defaultColor);
    console->printText(6, 5, "#   #  # #  # ## # #   #  # #    #  #");
    console->printText(6, 6, "#   #### #  # ## # #   #### ###  ###");
    console->printText(6, 7, "#   #  # #  # # ## #   #  # #    # #");
    console->printText(6, 8, "### #  #  ##  #  #  ## #  # #### #  #");
    
    menu->show();

    console->setTile(18, 15 + menu->getSelectedItem() * 3, '>', defaultColor);

    console->printText(6, 19, " \\  |  /", defaultColor);
    console->printText(6, 20, "  *****", defaultColor);
    console->printText(6, 21, " *     *");
    console->printText(6, 22, "*  o o  *");
    console->printText(6, 23, "*       *");
    console->printText(6, 24, "*   =   *");
    console->printText(6, 25, " *     *");
    console->printText(6, 26, "  *****");

    console->redraw();
}

char* copyString(const char* str)
{
    size_t length =  strlen(str) + 1;
    char* newString = new char[length];

    strncpy_s(newString, length, str, length);

    return newString;
}