#include "LinuxConsole.h"

Console::Console(UNICODE_STR title, int width, int height, Colors clearForeground, Colors clearBackground)
{
  //std::cout << "\x1b]50;" << "Courier New" << "\a" << std::flush;

  //printf("\033[8;%d;%dt", height, width);

    colors = 1;
    initscr();
    //resizeterm(height, width);
    //resize_term(width, height);
    noecho();
    keypad(stdscr,TRUE);
    //raw();
    //nonl();
    start_color();
    setClearColor(clearForeground, clearBackground);
    bkgd(COLOR_PAIR(1));
    setTitle(title);
}

void Console::clearConsole()
{
    clear();
}

void Console::setTitle(UNICODE_STR title)
{
    
    // ?
}

void Console::registerTimerEvent(timerEvent event, DWORD intervall)
{
    if (intervall > 0)
    {
        timer = event;
        intervallTime = intervall;

        CALC_NEXT_TICK
    }
}

void Console::run()
{
  running = true;

  //sleep(10);
  /*DWORD numEvents = 0;
  DWORD numEventsRead = 0;
    */
    if (timer != NULL)
    {
        CALC_NEXT_TICK
    }
    
    while (running)
    {
      int in;
      int i = 10;
      timeout(1);
      in = getch();
      keyUp(in, 0);
      if (timer != NULL) {
        if (clock() >= nextTickEvent)
        {
          timer();

          CALC_NEXT_TICK
        }
      }
    }
}

void Console::stop()
{
    running = false;
}

int Console::createColor(Colors forground, Colors background)
{
    init_pair(colors, forground, background);
    return colors++;
}

void Console::setColor(int color) {
  color_set(color, 0);
}

void Console::setBgColor(int color) {
  bkgd(COLOR_PAIR(color));
}

void Console::setColor(Colors forground, Colors background)
{
    init_pair(color, forground, background);
    color_set(color++, 0);
}

void Console::setClearColor(Colors forground, Colors background)
{
    init_pair(color, forground, background);
    color_set(color++, 0);
}

void Console::setTile(int x, int y, char c)
{
    char* zero_str = new char[2];
    zero_str[0] = c;
    zero_str[1] = 0;
    printText(x, y, zero_str);
}

void Console::setTile(int x, int y, char c, int colorId)
{
  color_set(colorId, 0);
  setTile(x, y,c);
}

void Console::printText(int x, int y, char* text)
{
    mvaddstr(y, x, text);
    //mvprintw(y, x, text);
}

void Console::printText(int x, int y, char* text, int colorId)
{
    color_set(colorId, 0);
    mvaddstr(y, x, text);
    //mvprintw(y, x, text);
}

void Console::redraw()
{
    refresh();
}

Console::~Console()
{
    endwin();
}