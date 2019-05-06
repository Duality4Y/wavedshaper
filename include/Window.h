#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "SDL2/SDL.h"

typedef void (*OnDraw_t)(void *);

class Window
{
private:
    SDL_Window *window = NULL; 
    SDL_Renderer *renderer = NULL;
    int width = -1;
    int height = -1;
    int shouldExit = false;

    uint32_t previousTick = 0;
    // draw with 25 fps.
    uint32_t interval = 1000 / 60;

    OnDraw_t OnDrawHandler = NULL;

    void TestDrawing();

public:
    Window(int width = 1024, int height = 1024, const char *windowname = "");
    int GetWidth();
    int GetHeight();
    int GetSize();
    void SetOnDrawHandler(OnDraw_t handler);
    void Clear();
    void DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b);
    void DrawLines(SDL_Point *points, int pointcount, int r, int g, int b);
    void Draw();
    void HandleInput(SDL_Event e);
    void HandleEvents();
    void Process();
    int Stop();
};

#endif