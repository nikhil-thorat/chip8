#include "input.h"

#include <stdbool.h>
#include <string.h>
#include <unistd.h>

void InputInit(Input *input)
{
    if (input == NULL)
    {
        return;
    }
    memset(input->keys, 0, sizeof(input->keys));
}
void InputPollTerminal(Input *input)
{
    if (input == NULL)
    {
        return;
    }

    static int key_timers[18] = {0};

    for (int i = 0; i < 18; i++)
    {
        if (key_timers[i] > 0)
        {
            key_timers[i]--;
            if (key_timers[i] == 0)
            {
                input->keys[i] = false;
            }
        }
    }

    char c;
    while (read(STDIN_FILENO, &c, 1) > 0)
    {
        int k = -1;
        switch (c)
        {
        case '1':
            k = 0x1;
            break;
        case '2':
            k = 0x2;
            break;
        case '3':
            k = 0x3;
            break;
        case '4':
            k = 0xC;
            break;
        case 'q':
            k = 0x4;
            break;
        case 'w':
            k = 0x5;
            break;
        case 'e':
            k = 0x6;
            break;
        case 'r':
            k = 0xD;
            break;
        case 'a':
            k = 0x7;
            break;
        case 's':
            k = 0x8;
            break;
        case 'd':
            k = 0x9;
            break;
        case 'f':
            k = 0xE;
            break;
        case 'z':
            k = 0xA;
            break;
        case 'x':
            k = 0x0;
            break;
        case 'c':
            k = 0xB;
            break;
        case 'v':
            k = 0xF;
            break;
        case 27:
            k = 16;
            break;
        }

        if (k != -1)
        {
            input->keys[k] = true;
            key_timers[k] = 75;
        }
    }
}
