#include <stdio.h>

#include <pulse/simple.h>
#include <pulse/error.h>

const int BUFSIZE = 1024;

int main(int argc, char **argv)
{
    static pa_sample_spec ss;
    ss.format = PA_SAMPLE_S16LE;
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

    while(1)
    {
        uint8_t buf[BUFSIZE];
        ssize_t r;

        for(int i = 0; i < BUFSIZE; i++)
        {
            buf[i] = i % 255;
        }
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
