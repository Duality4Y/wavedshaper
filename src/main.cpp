#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "SDL2/SDL.h"

#include "Window.h"
#include "Audio.h"


// buffer size = audiobuffersize * num channels.
int16_t audiobuffer[AUDIOBUFFERSIZE * 2];

void OnDrawHandler(void *parent)
{
    Window *window = (Window *)parent;

    for(int x = 32; x < window->GetWidth(); x += 32)
    {
        window->DrawLine(x, 0, x, window->GetWidth(), 0x40, 0x40, 0x40);
    }
    for(int y = 32; y < window->GetHeight(); y += 32)
    {
        window->DrawLine(0, y, window->GetHeight(), y, 0x40, 0x40, 0x40);
    }

    static SDL_Point leftPoints[AUDIOBUFFERSIZE / 2];
    static SDL_Point rightPoints[AUDIOBUFFERSIZE / 2];
    for(int i = 0; i < AUDIOBUFFERSIZE; i += 2)
    {
        leftPoints[i/2].x = i / 2;
        leftPoints[i/2].y = window->GetHeight() * 0.2 + audiobuffer[i];

        rightPoints[i/2].x = i / 2;
        rightPoints[i/2].y = window->GetHeight() * 0.5 + audiobuffer[i + 1];
    }
    
    window->DrawLines(leftPoints,  AUDIOBUFFERSIZE / 2, 0, 0,    0xFF);
    window->DrawLines(rightPoints, AUDIOBUFFERSIZE / 2, 0, 0xFF, 0);
}

int main()
{
    AudioReader audioreader = AudioReader(44100, 2);

    Window window = Window(1024, 1024, "First test");
    window.SetOnDrawHandler(OnDrawHandler);
    while(window.Stop() != true)
    {
        audioreader.Read(audiobuffer);
        window.Process();
    }
    /*
    SDL_Window *window = SDL_CreateWindow("This test.",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800,
                                          600,
                                          SDL_WINDOW_OPENGL);
    if(window == NULL)
    {
        printf("Could not create SDL window: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(3000);

    SDL_DestroyWindow(window);

    SDL_Quit();
    */

    return 0;
}

/*
#define PI 3.14159265
const int BUFSIZE = 1024;

int main(int argc, char **argv)
{
    static double buf[BUFSIZE];
    static pa_sample_spec ss;
    ss.format = PA_SAMPLE_FLOAT32LE;
    ss.rate = 44100;
    ss.channels = 2;

    pa_simple *s = NULL;
    int ret = 1;
    int error = 0;

    if(!(s= pa_simple_new(NULL, argv[0] , PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error)))
    {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        return 1;
    }

    for(int i = 0; i < (BUFSIZE / 2) - 1; i++)
    {
        buf[i] = 1;
    }

    while(1)
    {
        ssize_t r;

        if(pa_simple_write(s, buf, BUFSIZE, &error) < 0)
        {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
            return 1;
        }
    }

    if(s)
        pa_simple_free(s);
    
    return 0;
}
*/

// int main()
// {
    
//     // AudioReader audioreader = AudioReader(44100, 1);
//     // static int16_t buffer[AUDIOBUFFERSIZE];
//     // while(1)
//     // {
//     //     audioreader.Read(buffer);
//     //     for(int i = 0; i < AUDIOBUFFERSIZE / 2; i++)
//     //     {
//     //         printf("%d\r\n", buffer[i]);
//     //     }
//     // }
    
    
    
//     // AudioWriter audiowriter = AudioWriter(44100, 1);
//     // static int16_t buffer[AUDIOBUFFERSIZE];
//     // for(int i = 0; i < AUDIOBUFFERSIZE / 4; i++)
//     //     buffer[i] = 32767;
//     // while(1)
//     // {
//     //     audiowriter.Write(buffer);
//     // }
    

    
//     AudioReader audioreader = AudioReader(44100, 1);
//     AudioWriter audiowriter = AudioWriter(44100, 1);
//     static int16_t buffer[AUDIOBUFFERSIZE];
//     while(1)
//     {
//         audioreader.Read(buffer);
//         audiowriter.Write(buffer);
//         usleep(1000);
//     }
    
//     return 0;
// }

/*
static ssize_t loop_write(int fd, const void *data, size_t size)
{
    ssize_t ret = 0;
    while(size > 0)
    {
        ssize_t r = -1;

        if((r - write(fd, data, size)) < 0)
            return r;
        if(r == 0)
            break;
        
        ret += r;
        data = (const uint8_t *)data + r;
        size -= (size_t) r;
    }

    return ret;
}*/


// int main(int argc, char *argv[])
// {
//     static pa_sample_spec ss;
//     ss.format = PA_SAMPLE_S16LE;
//     ss.rate = 44100;
//     ss.channels = 2;

//     pa_simple *s = NULL;
//     int ret = 1;
//     int error;

//     if(!(s = pa_simple_new(NULL, argv[0], PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error)))
//     {
//         fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
//         goto finish;
//     }

//     while(1)
//     {
//         int16_t buffer[BUFFERSIZE];
//         if(pa_simple_read(s, buffer, sizeof(buffer), &error) < 0)
//         {
//             goto finish;
//         }
//         for(int i = 0; i < BUFFERSIZE; i++)
//         {
//             for(int r = 0; r < (134 / 2) + buffer[i]; r++)
//                 printf(" ");
//             printf("*\r\n");
//         }
//         // if(loop_write(STDOUT_FILENO, buffer, sizeof(buffer)) != sizeof(buffer))
//         // {
//         //     goto finish;
//         // }
//     }

// finish:
//     if(s)
//         pa_simple_free(s);
//     return ret;
// }
