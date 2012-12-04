#pragma once

#include <Windows.h>
#include <vector>

typedef int COLOR_ID;

using namespace std;

#define CALC_NEXT_TICK nextTickEvent = GetTickCount() + intervallTime;
#define COLOR(foreground, background) foreground | FOREGROUND_INTENSITY | background << 4


enum Colors { BLACK = 0, RED = 4, GREEN = 2, BLUE = 1, YELLOW = 6, MAGENTA = 5, CYAN = 3, WHITE = 7 };


typedef void (*keyEvent) (WORD keyCode);
typedef void (*timerEvent) ();


class Console
{
private:
    bool running;

    keyEvent keyDown;
    timerEvent timer;

    DWORD intervallTime;
    DWORD nextTickEvent;

    WORD color;
    WORD clearColor;
    vector<WORD>* colorPairs;

    HANDLE wHnd;
    HANDLE rHnd;

    CONSOLE_CURSOR_INFO info;
    SMALL_RECT windowSize;
    COORD topLeft;
    COORD bufferSize;
    CHAR_INFO* buffer;


public:
    // Konstruktor
    // Initialisiert die Konsole mit einem Titel und der H�he und Breite des Fenster + Puffers
    // Initialisiert die Schrift einer quadratischen Schriftart (8x8)
    // Blendet den Text-Cursor aus
    Console(const char* title, int width, int height, Colors clearForeground, Colors clearBackground);

    // Destruktor
    ~Console();

    // L�scht die Konsole zu Leerzeichen
    void clearConsole();

    // �ndert den Konsolentitel
    void setTitle(const char* title);

    // Eintragen einer Callback-Funktion f�r KeyDown-Events
    void registerKeyEvent(keyEvent event) { keyDown = event; }

    // Eintragen einer Callback-Funktion f�r Timer-Events
    void registerTimerEvent(timerEvent event, DWORD intervall);

    // Startet den Lebenszyklus der Abarbeitung der Events
    void run();

    // Stoppt den Lebenszyklus
    void stop();
    
    COLOR_ID createColor(Colors foreground, Colors background);

    void setColor(COLOR_ID colorId);

    void setBgColor(COLOR_ID colorId);
    
    // Setzt einen einzelnen Character in der Konsole am angegebenen Punkt
    void setTile(int x, int y, char tile);

    void setTile(int x, int y, char tile, COLOR_ID colorId);

    // Schreibt einen Text auf die Konsole am angegebenen Punkt
    void printText(int x, int y, const char* text);

    void printText(int x, int y, const char* text, COLOR_ID colorId);
    
    // Zeigt alle �nderungen an
    void redraw();
};