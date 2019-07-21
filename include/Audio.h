#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <pulse/simple.h>


#define AUDIOBUFFERSIZE (1024)

class AudioReader
{
public:
    AudioReader(int samplerate = 44100, int channels = 1, const char *appname = "AudioReader", const char *description = "AudioReader");
    void Read(void *);
private:
    pa_sample_spec soundspec;
    pa_simple *s = NULL;
    pa_sample_format_t format = PA_SAMPLE_S16LE;
    int error = -1;

};

class AudioWriter
{

public:
    AudioWriter(int samplerate = 44100, int channels = 1, const char *appname = "AudioWriter", const char *description = "AudioWriter");
    void Write(void *);
private:
    pa_sample_spec soundspec;
    pa_simple *s = NULL;
    pa_sample_format_t format = PA_SAMPLE_S16LE;
    int error = -1;
};

#endif