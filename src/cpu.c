#include "cpu.h"
#include "audio.h"
#include "display.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CpuInit(Cpu *cpu, Memory *memory, Display *display, Input *input, Audio *audio)
{
    if (cpu == NULL || memory == NULL || display == NULL || input == NULL || audio == NULL)
    {
        return;
    }

    memset(cpu->V, 0, sizeof(cpu->V));
    cpu->IR = 0;
    cpu->PC = ROM_START;

    memset(cpu->stack, 0, sizeof(cpu->stack));
    cpu->SP = 0;

    cpu->delay_timer = 0;
    cpu->sound_timer = 0;

    cpu->memory = memory;
    cpu->display = display;
    cpu->input = input;
    cpu->audio = audio;
}

void CpuCycle(Cpu *cpu)
{
    if (cpu == NULL)
    {
        return;
    }

    uint16_t opcode = (cpu->memory->bytes[cpu->PC] << 8) | cpu->memory->bytes[cpu->PC + 1];
    cpu->PC += 2;

    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    uint8_t N = (opcode & 0x000F);
    uint8_t NN = (opcode & 0x00FF);
    uint16_t NNN = (opcode & 0x0FFF);

    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (NN)
        {
        case 0xE0:
            DisplayClear(cpu->display);
            break;
        case 0xEE:
            cpu->SP -= 1;
            cpu->PC = cpu->stack[cpu->SP];
            break;
        }
        break;
    case 0x1000:
        cpu->PC = NNN;
        break;
    case 0x2000:
        cpu->stack[cpu->SP] = cpu->PC;
        cpu->SP += 1;
        cpu->PC = NNN;
        break;
    case 0x3000:
        if (cpu->V[X] == NN)
        {
            cpu->PC += 2;
        }
        break;
    case 0x4000:
        if (cpu->V[X] != NN)
        {
            cpu->PC += 2;
        }
        break;
    case 0x5000:
        if (cpu->V[X] == cpu->V[Y])
        {
            cpu->PC += 2;
        }
        break;
    case 0x6000:
        cpu->V[X] = NN;
        break;
    case 0x7000:
        cpu->V[X] += NN;
        break;
    case 0x8000:
        switch (N)
        {
        case 0x00:
            cpu->V[X] = cpu->V[Y];
            break;
        case 0x01:
            cpu->V[X] |= cpu->V[Y];
            break;
        case 0x02:
            cpu->V[X] &= cpu->V[Y];
            break;
        case 0x03:
            cpu->V[X] ^= cpu->V[Y];
            break;
        case 0x04: {
            uint16_t result = cpu->V[X] + cpu->V[Y];
            cpu->V[X] = (uint8_t)result;
            if (result > 0xFF)
            {
                cpu->V[0xF] = 1;
            }
            else
            {
                cpu->V[0xF] = 0;
            }
            break;
        }
        case 0x05:
            if (cpu->V[X] >= cpu->V[Y])
            {
                cpu->V[0xF] = 1;
            }
            else
            {
                cpu->V[0xF] = 0;
            }
            cpu->V[X] -= cpu->V[Y];
            break;
        case 0x06:
            cpu->V[0xF] = cpu->V[X] & 0x1;
            cpu->V[X] >>= 1;
            break;
        case 0x07:
            if (cpu->V[Y] >= cpu->V[X])
            {
                cpu->V[0xF] = 1;
            }
            else
            {
                cpu->V[0xF] = 0;
            }
            cpu->V[X] = cpu->V[Y] - cpu->V[X];
            break;
        case 0x0E:
            cpu->V[0xF] = (cpu->V[X] & 0x80) >> 7;
            cpu->V[X] <<= 1;
            break;
        }
        break;
    case 0x9000:
        if (cpu->V[X] != cpu->V[Y])
        {
            cpu->PC += 2;
        }
        break;
    case 0xA000:
        cpu->IR = NNN;
        break;
    case 0xB000:
        cpu->PC = NNN + cpu->V[0x0];
        break;
    case 0xC000:
        cpu->V[X] = (rand() % 256) & NN;
        break;
    case 0xD000: {
        uint8_t start_x = cpu->V[X] % DISPLAY_WIDTH;
        uint8_t start_y = cpu->V[Y] % DISPLAY_HEIGHT;

        cpu->V[0xF] = 0;

        for (uint8_t row = 0; row < N; row++)
        {
            uint8_t sprite_byte = cpu->memory->bytes[cpu->IR + row];
            for (uint8_t col = 0; col < 8; col++)
            {
                if ((sprite_byte & (0x80 >> col)) == 0)
                {
                    continue;
                }

                uint8_t screen_x = start_x + col;
                uint8_t screen_y = start_y + row;
                if (screen_x >= DISPLAY_WIDTH || screen_y >= DISPLAY_HEIGHT)
                {
                    continue;
                }

                int idx = (screen_y * DISPLAY_WIDTH) + screen_x;
                if (cpu->display->pixels[idx] == true)
                {
                    cpu->V[0xF] = 1;
                }
                cpu->display->pixels[idx] ^= 1;
            }
        }
        cpu->display->draw_flag = true;
    }
    break;
    case 0xE000:
        switch (NN)
        {
        case 0x9E:
            if (cpu->input->keys[cpu->V[X]] == true)
            {
                cpu->PC += 2;
            }
            break;
        case 0xA1:
            if (cpu->input->keys[cpu->V[X]] == false)
            {
                cpu->PC += 2;
            }
            break;
        }
        break;
    case 0xF000:
        switch (NN)
        {
        case 0x07:
            cpu->V[X] = cpu->delay_timer;
            break;
        case 0x15:
            cpu->delay_timer = cpu->V[X];
            break;
        case 0x18:
            cpu->sound_timer = cpu->V[X];
            break;
        case 0x1E:
            cpu->IR += cpu->V[X];
            break;
        case 0x0A:
            bool pressed = false;
            for (int i = 0; i < sizeof(cpu->input->keys); i++)
            {
                if (cpu->input->keys[i] == true)
                {
                    cpu->V[X] = i;
                    pressed = true;
                    break;
                }
            }

            if (!pressed)
            {
                cpu->PC -= 2;
            }
            break;
        case 0x29:
            cpu->IR = cpu->V[X] * 5;
            break;
        case 0x33: {
            uint8_t num = cpu->V[X];
            cpu->memory->bytes[cpu->IR] = num / 100;
            cpu->memory->bytes[cpu->IR + 1] = (num / 10) % 10;
            cpu->memory->bytes[cpu->IR + 2] = num % 10;
            break;
        }
        case 0x55:
            for (uint8_t i = 0; i <= X; i++)
            {
                cpu->memory->bytes[cpu->IR + i] = cpu->V[i];
            }
            break;
        case 0x65:
            for (uint8_t i = 0; i <= X; i++)
            {
                cpu->V[i] = cpu->memory->bytes[cpu->IR + i];
            }
            break;
        }
        break;
    default:
        fprintf(stderr, "ERROR : Unknown opcode 0x%04X\n", opcode);
    }
}

void CpuTickTimers(Cpu *cpu)
{
    if (cpu == NULL)
    {
        return;
    }

    if (cpu->delay_timer > 0)
    {
        cpu->delay_timer--;
    }

    if (cpu->sound_timer > 0)
    {
        cpu->sound_timer--;
        AudioPlay(cpu->audio);
    }
    else
    {
        AudioStop(cpu->audio);
    }
}
