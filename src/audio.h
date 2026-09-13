#pragma once

#ifndef AUDIO
#define AUDIO

#include <stdbool.h>

typedef struct
{
    bool is_playing;
} Audio;

void AudioInit(Audio *audio);
void AudioPlay(Audio *audio);
void AudioStop(Audio *audio);

#endif /* AUDIO */
