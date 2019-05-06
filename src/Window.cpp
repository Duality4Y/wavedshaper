#include "Window.h"

Window::Window(int width, int height, const char *windowname)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        exit(1);

    this->window = SDL_CreateWindow(windowname,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    width,
                                    height,
                                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    this->width = width;
    this->height = height;
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
}

void Window::TestDrawing()
{
    this->DrawLine(0, 0, this->width, this->height, 0xff, 0xff, 0);
    static SDL_Point points[] = { {0, 0}, {this->width, this->height}, {100, 200}, {123, 876}};
    this->DrawLines(points, 4, 0xff, 0, 0xff);
}

int Window::GetWidth()
{
    return this->width;
}

int Window::GetHeight()
{
    return this->height;
}

int Window::GetSize()
{
    return (this->width * this->height);
}

void Window::SetOnDrawHandler(OnDraw_t handler)
{
    this->OnDrawHandler = handler;
}

void Window::Clear()
{
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(this->renderer);
}

void Window::DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b)
{
    SDL_SetRenderDrawColor(this->renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(this->renderer, x1, y1, x2, y2);
}

void Window::DrawLines(SDL_Point *points, int pointcount, int r, int g, int b)
{
    SDL_SetRenderDrawColor(this->renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(renderer, points, pointcount);
}

void Window::Draw()
{
    Clear();
    if(this->OnDrawHandler != NULL)
    {
        this->OnDrawHandler(this);
    }
    SDL_RenderPresent(this->renderer);
}

void Window::HandleInput(SDL_Event e)
{
    if(e.type == SDL_KEYUP)
    {
        if(e.key.keysym.sym == SDLK_q)
            this->shouldExit = true;
    }
}

void Window::HandleEvents()
{
    SDL_Event e;
    if(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
        {
            this->shouldExit = true;
            return;
        }
        this->HandleInput(e);
    }
}

void Window::Process()
{
    this->HandleEvents();
    if((SDL_GetTicks() - this->previousTick >= this->interval))
    {
        this->previousTick = SDL_GetTicks();
        this->Draw();
    }
}

int Window::Stop()
{
    return shouldExit;
}