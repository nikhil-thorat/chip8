#include "audio.h"

#include <stdbool.h>
#include <stdio.h>

void AudioInit(Audio *audio)
{
    if (audio == NULL)
    {
        return;
    }
    audio->is_playing = false;
}

void AudioPlay(Audio *audio)
{
    if (audio == NULL)
    {
        return;
    }

    if (audio->is_playing == false)
    {
        printf("\a");
        fflush(stdout);
        audio->is_playing = true;
    }
}

void AudioStop(Audio *audio)
{
    if (audio == NULL)
    {
        return;
    }
    audio->is_playing = false;
}
