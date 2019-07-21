#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <complex.h>
#include <math.h>

#include <fftw3.h>
#include "SDL2/SDL.h"

#include "Window.h"
#include "Audio.h"

using namespace std;

#define PI 3.14159265
#define REAL 0
#define IMAG 1

// buffer size = audiobuffersize * num channels.
int16_t audiobuffer[AUDIOBUFFERSIZE * 2]; // contains both left and right audio data
int16_t leftaudiobuffer[AUDIOBUFFERSIZE];
int16_t rightaudiobuffer[AUDIOBUFFERSIZE];

int FftHeightScaler = 10; // scaledown by this so binvalue / fftscaler.

int N = AUDIOBUFFERSIZE * 2;
fftw_complex *in = NULL;
fftw_complex *out = NULL;

fftw_complex *FftRightChannel = NULL;
fftw_complex *FftLeftChannel = NULL;

fftw_plan p = NULL;

void DrawGridLines(Window *window)
{
    for(int x = 32; x < window->GetWidth(); x += 32)
    {
        window->DrawLine(x, 0, x, window->GetWidth(), 0x40, 0x40, 0x40);
    }

    for(int y = 32; y < window->GetHeight(); y += 32)
    {
        window->DrawLine(0, y, window->GetHeight(), y, 0x40, 0x40, 0x40);
    }
}

void SeperateChannelData()
{

    // seperate the audio data channels.
    for(int i = 0; i < AUDIOBUFFERSIZE * 2; i += 2)
    {
        leftaudiobuffer[i/2] = audiobuffer[i];
        rightaudiobuffer[i/2] = audiobuffer[i + 1];
    }
}

void DrawChannelData(Window *window)
{

    // build lines and draw them.
    static SDL_Point leftPoints[AUDIOBUFFERSIZE];
    static SDL_Point rightPoints[AUDIOBUFFERSIZE];
    for(int i = 0; i < AUDIOBUFFERSIZE; i++)
    {
        leftPoints[i].x  = i;
        leftPoints[i].y = window->GetHeight() * 0.2 + leftaudiobuffer[i];
        rightPoints[i].x  = i;
        rightPoints[i].y = window->GetHeight() * 0.5 + rightaudiobuffer[i];
    }

    window->DrawLines(leftPoints, AUDIOBUFFERSIZE, 0, 0, 0xff);
    window->DrawLines(rightPoints, AUDIOBUFFERSIZE, 0, 0xff, 0);
}

void ComputeFFT()
{
    for(int i = 0; i < AUDIOBUFFERSIZE; i++)
    {
        in[i][REAL] = rightaudiobuffer[i];
        in[i][IMAG] = 0;
    }
    fftw_execute(p);
    memcpy(FftRightChannel, out, AUDIOBUFFERSIZE * sizeof(fftw_complex));

    for(int i = 0; i < AUDIOBUFFERSIZE; i++)
    {
        in[i][REAL] = leftaudiobuffer[i];
        in[i][IMAG] = 0;
    }
    fftw_execute(p);
    memcpy(FftLeftChannel, out, AUDIOBUFFERSIZE * sizeof(fftw_complex));
}

void DrawRawFFT(Window *window)
{
    static SDL_Point fftPoints[AUDIOBUFFERSIZE];

    // Draw for the Right audio channel.
    for(int i = 0; i < AUDIOBUFFERSIZE; i++)
    {
        fftPoints[i].x = i;

        std::complex<double> binvalue = std::complex<double>(FftRightChannel[i][REAL], FftRightChannel[i][IMAG]);
        binvalue = std::abs(binvalue);
        binvalue /= FftHeightScaler;

        fftPoints[i].y = window->GetHeight() - real(binvalue);
    }
    window->DrawLines(fftPoints, AUDIOBUFFERSIZE, 0xFF, 0xFF, 0);

    // Draw for the Left audio channel.
    for(int i = 0; i < AUDIOBUFFERSIZE; i++)
    {
        fftPoints[i].x = i;

        std::complex<double> binvalue = std::complex<double>(FftLeftChannel[i][REAL], FftLeftChannel[i][IMAG]);
        binvalue = std::abs(binvalue);
        binvalue /= FftHeightScaler;

        fftPoints[i].y = window->GetHeight() - real(binvalue);
    }
    window->DrawLines(fftPoints, AUDIOBUFFERSIZE, 0, 0xFF, 0xFF);
}

void OnDrawHandler(void *parent)
{
    Window *window = (Window *)parent;

    DrawGridLines(window);

    DrawChannelData(window);

    DrawRawFFT(window);
}

class FFT
{
    FFT(){}
};

int main()
{
    AudioReader audioreader = AudioReader(44100, 2);
    Window window = Window(1024, 1024, "First test");
    window.SetOnDrawHandler(OnDrawHandler);

    // allocate fftw buffers.
    in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    FftRightChannel = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * AUDIOBUFFERSIZE);
    FftLeftChannel = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * AUDIOBUFFERSIZE);
    // create a fftw plan
    // p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_MEASURE);
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_MEASURE);

    // double complex z = 1.0 + 2.0 * I;
    complex<double> z(0.0, 0.0);
    printf("%f%+fi\n", real(z), imag(z));

    while(window.Stop() != true)
    {
        audioreader.Read(audiobuffer);

        SeperateChannelData();
        
        ComputeFFT();

        window.Process();
    }

    fftw_destroy_plan(p);
    fftw_free(out);
    fftw_free(in);
    fftw_free(FftRightChannel);
    fftw_free(FftLeftChannel);

    return 0;
}
