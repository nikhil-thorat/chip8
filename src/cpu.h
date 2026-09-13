#pragma once

#ifndef CPU
#define CPU

#include "audio.h"
#include "display.h"
#include "input.h"
#include "memory.h"
#include <stdint.h>

typedef struct
{
    uint8_t V[16];
    uint16_t IR;
    uint16_t PC;

    uint16_t stack[16];
    uint8_t SP;

    uint8_t delay_timer;
    uint8_t sound_timer;

    Memory *memory;
    Display *display;
    Input *input;
    Audio *audio;
} Cpu;

void CpuInit(Cpu *cpu, Memory *memory, Display *display, Input *input, Audio *audio);
void CpuCycle(Cpu *cpu);
void CpuTickTimers(Cpu *cpu);

#endif /* CPU */
