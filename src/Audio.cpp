#include "Audio.h"

#include <stdio.h>
#include <stdlib.h>

#include <pulse/error.h>

AudioReader::AudioReader(int samplerate, int channels, const char *appname, const char *description)
{

    this->soundspec.format = this->format;
    this->soundspec.rate = samplerate;
    this->soundspec.channels = channels;

    this->s = pa_simple_new(NULL,
                           appname,
                           PA_STREAM_RECORD,
                           NULL,
                           description,
                           &(this->soundspec),
                           NULL,
                           NULL,
                           &(this->error));
    if(!s)
    {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(this->error));
        exit(1);
    }
}

void AudioReader::Read(void *buffer)
{

    if(pa_simple_read(this->s, buffer, AUDIOBUFFERSIZE * 2 * this->soundspec.channels, &(this->error)) < 0)
    {
        fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(this->error));
        exit(1);
    }
}

AudioWriter::AudioWriter(int samplerate, int channels, const char *appname, const char *description)
{
    this->soundspec.format = this->format;
    this->soundspec.rate = samplerate;
    this->soundspec.channels = channels;

    this->s = pa_simple_new(NULL,
                            appname,
                            PA_STREAM_PLAYBACK,
                            NULL,
                            description,
                            &(this->soundspec),
                            NULL,
                            NULL,
                            &(this->error));

    if(!s)
    {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(this->error));
        exit(1);
    }
}

void AudioWriter::Write(void *buffer)
{
    if(pa_simple_write(this->s, buffer, AUDIOBUFFERSIZE, &(this->error)) < 0)
    {
        fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(this->error));
        exit(1);
    }
}